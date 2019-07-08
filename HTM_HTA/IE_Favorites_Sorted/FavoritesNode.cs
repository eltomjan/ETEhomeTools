using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.IO;

namespace CodeProject.IEFavDecon
{
    public enum FavoritesNodeType
    {
        Folder,
        Url
    };

    /// <summary>
    /// Class that enables you to load and examine the internet explorer favoriter (aka bookmarks) of the current user. 
    /// </summary>
    public class FavoritesNode : IList<FavoritesNode>
    {
        #region Private members, properties, types, methods

        private const string sectionDefault = "DEFAULT";
        private const string sectionInternetShortcut = "InternetShortcut";

        private FavoritesNode parent;
        private FavoritesNodeType nodeType;
        private string displayName;
        private string systemName;
        private List<FavoritesNode> children;
        private int order;
        private string url;
        private DateTime modifiedTime;
        private string iconFile;
        private int iconIndex;

        /// <summary>
        /// Gets the qualified (file-)system name of the folder or url. URLs that are in the favorites folder root have no prefix,
        /// URLs that are organized deeper in the favorites menu are prefixed with the folder path.
        /// </summary>
        private string QualifiedSystemName
        {
            get
            {
                return (parent != null && parent.parent != null) ? parent.QualifiedSystemName + Path.DirectorySeparatorChar + systemName : systemName;
            }
        }

        /// <summary>
        /// Gets the full path to the folder or url file that this node instance points to
        /// </summary>
        public string FilePath
        {
            get
            {
                string result;
                string favoritesPath = Environment.GetFolderPath(Environment.SpecialFolder.Favorites);

                if (nodeType == FavoritesNodeType.Url)
                {
                    result = Path.Combine(favoritesPath, QualifiedSystemName) + ".url";
                }
                else
                {
                    result = Path.Combine(favoritesPath, QualifiedSystemName);
                }
                return result;
            }
        }

        private struct MenuOrderRecord
        {
            public FavoritesNodeType RecordType;
            public string Name83;
            public string LongName;
            public string DisplayName;
            public int Order;

            public static MenuOrderRecord Deserialize(Stream stream)
            {
                MenuOrderRecord result = new MenuOrderRecord();
                BinaryReader br = new BinaryReader(stream);

                long recordPosition = stream.Position;
                UInt32 lengthOfRecord = br.ReadUInt32();

                try
                {
                    result.Order = br.ReadInt32();

                    UInt16 subRecord0Size = br.ReadUInt16();
                    UInt16 flags = br.ReadUInt16();
                    result.RecordType = ((flags & 2) == 2) ? FavoritesNodeType.Url : FavoritesNodeType.Folder;
                    bool isUnicode = ((flags & 4) == 4);
                    UInt32 unknown1 = br.ReadUInt32();
                    UInt32 unknown2 = br.ReadUInt32();
                    UInt16 stringLengthOrFlags = br.ReadUInt16();

                    result.Name83 = Utilities.ReadZeroTerminatedString(stream, isUnicode ? Encoding.Unicode : Encoding.ASCII);
                    if ((stream.Position & 1) > 0) stream.Position++;

                    // jump to the position of the subRecord1Index word
                    stream.Position = recordPosition + lengthOfRecord - 8;
                    UInt32 subRecord1Index = br.ReadUInt16();  // index to subrecord1, which is where the long name is

                    // calculate position of subRecord1
                    stream.Position = recordPosition + 8 + subRecord1Index;

                    // read subrecord1 size
                    UInt16 subRecord1Size = br.ReadUInt16();

                    // skip 18 bytes on xp, 36 bytes on vista
                    if (Environment.OSVersion.Version.Major == 5 && Environment.OSVersion.Version.Minor > 0)
                    {
                        stream.Position += 18;
                    }
                    else
                    {
                        stream.Position += 36;
                    }

                    result.LongName = Utilities.ReadZeroTerminatedString(stream, Encoding.Unicode);
                    result.DisplayName = result.LongName;

                    if ((stringLengthOrFlags & 1) == 1)
                    {
                        string resourceString = Utilities.GetStringResource(Utilities.ReadZeroTerminatedString(stream, Encoding.Unicode));
                        if (resourceString != null && resourceString != "")
                        {
                            result.DisplayName = resourceString;
                        }
                    }

                    // validate the record by checking if the subrecord sizes add up
                    if ((subRecord1Size + subRecord1Index) != subRecord0Size)
                    {
                        throw new Exception("Invalid record format");
                    }

                    //Console.WriteLine("");
                    //Console.WriteLine("flags           : {0:x}", flags);
                    //Console.WriteLine("flags2          : {0:x}", stringLengthOrFlags);
                    //Console.WriteLine("longname        : {0}", result.LongName);
                    //Console.WriteLine("name83          : {0}", result.Name83);
                    //Console.WriteLine("longname        : {0}", result.LongName);
                }
                finally
                {
                    stream.Seek(recordPosition + lengthOfRecord, SeekOrigin.Begin);
                }

                return result;
            }
        };

        private static FavoritesNode LoadURLNodeFromFileSystem(FavoritesNode parent, string name, int order)
        {
            FavoritesNode result = new FavoritesNode();
            result.parent = parent;
            result.order = order;
            result.displayName = name;
            result.systemName = name;
            result.nodeType = FavoritesNodeType.Url;
            IniFile iniFile = new IniFile(result.FilePath);
            result.url = iniFile.GetValue(sectionInternetShortcut, "URL", "");
            result.modifiedTime = Utilities.HexEncodingToDateTime(iniFile.GetValue(sectionInternetShortcut, "Modified", ""));
            result.iconFile = iniFile.GetValue(sectionInternetShortcut, "IconFile", "");
            result.iconIndex = Int32.Parse(iniFile.GetValue(sectionInternetShortcut, "IconIndex", "0"));
            return result;
        }

        private static FavoritesNode LoadFolderNodeFromFileSystem(FavoritesNode parent, string dirName, string displayName, int order)
        {
            string folderPath = Path.Combine(parent.FilePath, dirName);

            if (Directory.Exists(folderPath))
            {
                FavoritesNode result = new FavoritesNode();
                result.parent = parent;
                result.order = order;
                result.displayName = displayName;
                result.systemName = dirName;
                result.nodeType = FavoritesNodeType.Folder;

                DirectoryInfo directoryInfo = new DirectoryInfo(folderPath);

                DirectoryInfo[] directories = directoryInfo.GetDirectories();

                Array.Sort<DirectoryInfo>(
                    directories,
                    delegate(DirectoryInfo a, DirectoryInfo b)
                    {
                        return string.Compare(a.Name, b.Name);
                    }
                );

                int subOrder = 0;
                foreach (DirectoryInfo directory in directories)
                {
                    result.children.Add(LoadFolderNodeFromFileSystem(result, directory.Name, directory.Name, subOrder));
                    subOrder++;
                }

                FileInfo[] files = directoryInfo.GetFiles("*.url", SearchOption.TopDirectoryOnly);

                Array.Sort<FileInfo>(
                    files,
                    delegate(FileInfo a, FileInfo b)
                    {
                        return string.Compare(a.Name, b.Name);
                    }
                );

                foreach (FileInfo file in files)
                {
                    result.children.Add(LoadURLNodeFromFileSystem(result, Path.GetFileNameWithoutExtension(file.Name), subOrder));
                    subOrder++;
                }

                return result;
            }
            else
            {
                throw new Exception(string.Format("could not find {0} in registry or disk", dirName));
            }
        }

        private static FavoritesNode LoadFolderNodeFromRegistry(FavoritesNode parent, string dirName, string displayName, RegistryKey registryKey, int order)
        {
            FavoritesNode result = new FavoritesNode();
            result.parent = parent;
            result.order = order;
            result.displayName = displayName;
            result.systemName = dirName;
            result.nodeType = FavoritesNodeType.Folder;

            byte[] registryData = registryKey.GetValue("Order") as byte[];

            if (registryData != null)
            {
                MemoryStream ms = new MemoryStream(registryData);
                BinaryReader br = new BinaryReader(ms);

                UInt32 headerId0 = br.ReadUInt32();
                UInt32 headerId1 = br.ReadUInt32();
                UInt32 totalSize = br.ReadUInt32();
                UInt32 headerId2 = br.ReadUInt32();
                UInt32 numberOfRecords = br.ReadUInt32();

                if (headerId0 == 0x00000008 &&
                    headerId1 == 0x00000002 &&
                    headerId2 == 0x00000001)
                {
                    //Console.WriteLine("=================");
                    //Console.WriteLine("headerId0       : {0:x}", headerId0);
                    //Console.WriteLine("headerId1       : {0:x}", headerId1);
                    //Console.WriteLine("totalSize       : {0:x}", totalSize);
                    //Console.WriteLine("headerId2       : {0:x}", headerId2);
                    //Console.WriteLine("numberOfRecords : {0:x}", numberOfRecords);

                    for (uint t = 0; t < numberOfRecords; t++)
                    {
                        try
                        {
                            FavoritesNode childNode = null;
                            MenuOrderRecord record = MenuOrderRecord.Deserialize(br.BaseStream);

                            if (record.RecordType == FavoritesNodeType.Folder)
                            {
                                // apparently in the registry it always uses the display name
                                RegistryKey childKey = registryKey.OpenSubKey(record.DisplayName);
                                if (childKey != null)
                                {
                                    childNode = LoadFolderNodeFromRegistry(result, record.LongName, record.DisplayName, childKey, record.Order);
                                    childKey.Close();
                                }
                                else
                                {
                                    childNode = LoadFolderNodeFromFileSystem(result, record.LongName, record.DisplayName, record.Order);
                                }
                            }
                            else
                            {
                                childNode = FavoritesNode.LoadURLNodeFromFileSystem(result, Path.GetFileNameWithoutExtension(record.LongName), record.Order);
                            }
                            result.children.Add(childNode);
                        }
                        catch
                        {
                        }
                    }

                    result.children.Sort(
                        delegate(FavoritesNode a, FavoritesNode b)
                        {
                            if (a.order < 0 && b.order < 0)
                            {
                                // both items are unordered.. sort the items according to their name
                                return string.Compare(a.displayName, b.displayName);
                            }
                            else if (a.order < 0 || b.order < 0)
                            {
                                // one of the items is unordered. The negative one should go after the positive one.
                                return b.order - a.order;
                            }
                            // both items are ordered, sort normally
                            return a.order - b.order;
                        }
                    );
                }
            }
            return result;
        }

        /// <summary>
        /// Node type validation. Throws an exception if the node type doesn't match the requested type.
        /// </summary>
        /// <param name="nodeType">Requested node type.</param>
        private void ValidateNodeType(FavoritesNodeType nodeType)
        {
            if (this.nodeType != nodeType) throw new Exception(string.Format("node is not a {0}", nodeType));
        }

        /// <summary>
        /// Detach a node from its parent, if it has one.
        /// </summary>
        /// <param name="item">Node to detach</param>
        private static void Orphanize(FavoritesNode item)
        {
            if (item.parent != null)
            {
                item.parent.Remove(item);
            }
        }

        #endregion

        #region Public properties

        /// <summary>
        /// Gets this nodes parent node.
        /// </summary>
        public FavoritesNode Parent
        {
            get { return parent; }
            set { parent = value; }
        }

        /// <summary>
        /// Gets the type of this node: Folder or Url
        /// </summary>
        public FavoritesNodeType NodeType
        {
            get
            {
                return nodeType;
            }
        }

        /// <summary>
        /// Gets the display name of the folder or url
        /// </summary>
        public string DisplayName
        {
            get
            {
                return displayName;
            }
        }

        /// <summary>
        /// Gets the system name of the folder or url (as it is represented on disk)
        /// </summary>
        public string SystemName
        {
            get
            {
                return systemName;
            }
        }

        /// <summary>
        /// Gets the qualified display name of the folder or url. URLs that are in the favorites folder root have no prefix,
        /// URLs that are organized deeper in the favorites menu are prefixed with the folder path.
        /// </summary>
        public string QualifiedDisplayName
        {
            get
            {
                return (parent != null && parent.parent != null) ? parent.QualifiedDisplayName + Path.DirectorySeparatorChar + displayName : displayName;
            }
        }

        /// <summary>
        /// Gets the target internet address for this bookmark. 
        /// Throws an exception if this node is not an URL node.
        /// </summary>
        public string Url
        {
            get
            {
                ValidateNodeType(FavoritesNodeType.Url);
                return url;
            }
        }

        /// <summary>
        /// Gets the modified time for this bookmark. 
        /// Throws an exception if this node is not an URL node.
        /// </summary>
        public DateTime ModifiedTime
        {
            get
            {
                ValidateNodeType(FavoritesNodeType.Url);
                return modifiedTime;
            }
        }

        /// <summary>
        /// Gets the path or internet address for the icon related to this bookmark.
        /// Throws an exception if this node is not an URL node.
        /// </summary>
        public string IconFile
        {
            get 
            {
                ValidateNodeType(FavoritesNodeType.Url);
                return iconFile; 
            }
        }

        /// <summary>
        /// Gets the icon index for the icon related to this bookmark.
        /// Throws an exception if this node is not an URL node.
        /// </summary>
        public int IconIndex
        {
            get 
            {
                ValidateNodeType(FavoritesNodeType.Url);
                return iconIndex; 
            }
        }

        #endregion

        #region Public constructors, methods

        /// <summary>
        /// FavoritesNode constructor.
        /// </summary>
        public FavoritesNode()
        {
            parent = null;
            nodeType = FavoritesNodeType.Folder;
            displayName = "";
            systemName = "";
            children = new List<FavoritesNode>();
            order = -1;
            url = "";
            modifiedTime = DateTime.Now;
            iconFile = "";
            iconIndex = -1;
        }

        /// <summary>
        /// Factory method.
        /// </summary>
        /// <returns>Root node of the favorites of the current user.</returns>
        public static FavoritesNode LoadRoot()
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Explorer\MenuOrder\Favorites");
            FavoritesNode result = FavoritesNode.LoadFolderNodeFromRegistry(null, "", "", key, 0);
            key.Close();
            return result;
        }

        #region IList<FavoritesNode> Members

        public int IndexOf(FavoritesNode item)
        {
            return children.IndexOf(item);
        }

        public void Insert(int index, FavoritesNode item)
        {
            Orphanize(item);
            item.parent = this;
            children.Insert(index, item);
        }

        public void RemoveAt(int index)
        {
            children[index].parent=null;
            children.RemoveAt(index);
        }

        public FavoritesNode this[int index]
        {
            get
            {
                return children[index];
            }
            set
            {
                // whats this: replace the existing node by a new one..
                // if the incoming node already has a parent, remove it from the old parent
                Orphanize(value);
                // break the link of the old child to the parent
                children[index].parent = null;
                // put the new node in its place
                children[index] = value;
            }
        }

        #endregion

        #region ICollection<FavoritesNode> Members

        public void Add(FavoritesNode item)
        {
            Insert(Count, item);
        }

        public void Clear()
        {
            while (Count>0)
            {
                RemoveAt(0);
            }
        }

        public bool Contains(FavoritesNode item)
        {
            return children.Contains(item);
        }

        public void CopyTo(FavoritesNode[] array, int arrayIndex)
        {
            children.CopyTo(array, arrayIndex);
        }

        public int Count
        {
            get { return children.Count; }
        }

        public bool IsReadOnly
        {
            get { return false; }
        }

        public bool Remove(FavoritesNode item)
        {
            int index = IndexOf(item);
            if (index >= 0 && index < Count)
            {
                RemoveAt(index);
                return true;
            }
            return false;
        }

        #endregion

        #region IEnumerable<FavoritesNode> Members

        public IEnumerator<FavoritesNode> GetEnumerator()
        {
            return children.GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        #endregion

        #endregion
    }
}
