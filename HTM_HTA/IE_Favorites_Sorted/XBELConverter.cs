using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Xsl;

namespace CodeProject.IEFavDecon
{
    /// <summary>
    /// Helper class that converts a FavoritesNode tree into a XBEL (XML Bookmark Exchange Language) file.
    /// </summary>
    public class XBELConverter
    {
        #region Private members, properties, methods

        private const string titleElementName = "title";
        private const string descriptionElementName = "desc";

        private int idCounter;
        private XmlDocument xmlDocument;

        private static string FormatDate(DateTime dateTime)
        {
            return dateTime.ToString("yyyy'-'MM'-'dd'T'HH':'mm':'ss");
        }

        /// <summary>
        /// Creates a XBEL document-wide unique ID attribute.
        /// </summary>
        /// <returns></returns>
        private XmlAttribute CreateIdAttribute()
        {
            XmlAttribute result = xmlDocument.CreateAttribute("id");
            result.Value = "xbelid" + idCounter.ToString();
            idCounter++;
            return result;
        }

        /// <summary>
        /// Converts a FavoritesNode into the corresponding XBEL xml-element, depending on its type.
        /// </summary>
        /// <param name="favoritesNode">FavoritesNode instance to convert.</param>
        /// <returns>XBEL XmlElement (folder or bookmark)</returns>
        private XmlElement Transform(FavoritesNode favoritesNode)
        {
            XmlElement result;

            if (favoritesNode.NodeType == FavoritesNodeType.Folder)
            {
                // create a 'folder' element with the attibutes 'id', 'folded' and 'added'
                result = xmlDocument.CreateElement("folder");
                result.Attributes.Append(CreateIdAttribute());
                result.SetAttribute("folded", "yes");
                result.SetAttribute("added", FormatDate(DateTime.Now));
                // add a 'title' child element
                result.AppendChild(xmlDocument.CreateElement(titleElementName)).InnerText = favoritesNode.DisplayName;
                // add the nested children
                foreach (FavoritesNode childNode in favoritesNode)
                {
                    result.AppendChild(Transform(childNode));
                }
            }
            else
            {
                // create a 'bookmark' element with the attributes 'id', 'href' and 'modified'
                result = xmlDocument.CreateElement("bookmark");
                result.Attributes.Append(CreateIdAttribute());
                result.SetAttribute("href", favoritesNode.Url);
                result.SetAttribute("modified", FormatDate(favoritesNode.ModifiedTime));
                result.AppendChild(xmlDocument.CreateElement(titleElementName)).InnerText = favoritesNode.DisplayName;
                result.AppendChild(xmlDocument.CreateElement(descriptionElementName)).InnerText = "";
            }
            return result;
        }

        /// <summary>
        /// Constructor. Converts the passed FavoritesNode instance into an XBEL formatted XmlDocument.
        /// </summary>
        /// <param name="favoritesNode">FavoritesNode instance to convert into XBEL</param>
        private XBELConverter(FavoritesNode favoritesNode)
        {
            // reset the id counter
            idCounter = 0;
            xmlDocument = new XmlDocument();

            // header stuff needed for the XBEL file:
            xmlDocument.AppendChild(xmlDocument.CreateXmlDeclaration("1.0", null, null));
            xmlDocument.AppendChild(xmlDocument.CreateDocumentType("xbel", "+//IDN python.org//DTD XML Bookmark Exchange Language 1.0//EN//XML", "http://pyxml.sourceforge.net/topics/dtds/xbel-1.0.dtd", null));

            // create the 'xbel' root element with 'id', 'added' and 'version' attributes
            XmlElement rootElement = (XmlElement)xmlDocument.CreateElement("xbel");
            rootElement.Attributes.Append(CreateIdAttribute());
            rootElement.SetAttribute("added", FormatDate(DateTime.Now));
            rootElement.SetAttribute("version", "1.0");

            // add the title and description elements to the xbel element. The description could be something more creative
            rootElement.AppendChild(xmlDocument.CreateElement(titleElementName)).InnerText = "Bookmarks of " + Environment.UserName;
            rootElement.AppendChild(xmlDocument.CreateElement(descriptionElementName)).InnerText = "Description";
            
            // walk the children of the main favorites node, and add them too
            foreach (FavoritesNode child in favoritesNode)
            {
                rootElement.AppendChild(Transform(child));
            }
            
            // finally attach the root element to the xml document
            xmlDocument.AppendChild(rootElement);
        }

        private void Write(string filename)
        {
            // UTF-8 please.
            XmlWriterSettings xmlWriterSettings = new XmlWriterSettings();
            xmlWriterSettings.Encoding = Encoding.UTF8;
            xmlWriterSettings.Indent = true;
            XmlWriter xmlWriter = XmlWriter.Create(filename, xmlWriterSettings);
            xmlDocument.Save(xmlWriter);
            xmlWriter.Flush();
            xmlWriter.Close();
        }

        #endregion

        #region Public properties, methods
        /// <summary>
        /// Converts the given Favorites tree into an UTF-8 XBEL file at the specified path.
        /// </summary>
        /// <param name="favoritesNode">Favorites tree to convert</param>
        /// <param name="filename">Destination file</param>
        public static void ConvertFavoritesToXBEL(FavoritesNode favoritesNode, string filename)
        {
            XBELConverter favoritesToXBEL = new XBELConverter(favoritesNode);
            favoritesToXBEL.Write(filename);
        }
        #endregion
    }
}
