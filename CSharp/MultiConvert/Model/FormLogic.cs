using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Gason;

namespace MultiConvert.Model
{
    class FormLogic
    {
        private Parser jsonParser;
        public VisualNode3 JSON { get; private set; }
        public Byte[] JSONdata { get; private set; }
        public FormLogic()
        {
            jsonParser = new Parser(true);
        }
        public void LoadJSON(String fileName)
        {
            JSONdata = File.ReadAllBytes(fileName);
            JsonNode result;
            int endPos = -1;

            JsonErrno e = jsonParser.Parse(JSONdata, ref endPos, out result);
            if (JsonErrno.OK == e) JSON = new VisualNode3(ref result, JSONdata, 10000);
        }
        public void Save(String fileName, String ext, MemoryStream mem)
        {
            String FileName = fileName.Substring(0, fileName.LastIndexOf('.')) + ext;
            FileStream file = new FileStream(FileName, FileMode.Create, FileAccess.Write);
            mem.WriteTo(file);
            file.Close();
        }
        public void Save(String fileName, String ext, StringBuilder res)
        {
            if (fileName.Length == 0) return;
            String FileName = fileName.Substring(0, fileName.LastIndexOf('.')) + ext;
            File.WriteAllText(FileName, res.ToString());
        }
        internal String GetTag(VisualNode3 node, Boolean end = false)
        {
            if(JsonTag.JSON_OBJECT == node.Tag_Viewer)
            {
                if (node.m_JsonNode.HasKey) {
                    if (end) return $"</{node.Key_Viewer}>";
                    else return $"<{node.Key_Viewer}>";
                }
            } else if(JsonTag.JSON_STRING == node.Tag_Viewer)
            {
                if (node.m_JsonNode.HasKey) return $"<{node.Key_Viewer}>{node.Value_Viewer}</{node.Key_Viewer}>";
            }
            return "";
        }
        internal StringBuilder Convert2XML()
        {
            if (null == JSON) return new StringBuilder("No JSON data !");
            int orig = JSON.m_Indent;
            JSON.m_Indent = 0;
            StringBuilder retVal = JSON.DumpXMLValueIterative(JSON.m_JsonNode);
            JSON.m_Indent = orig;
            return retVal;
        }
        internal StringBuilder Convert2CSVvisual(Boolean horizontal, Boolean sortLast, String id)
        {
            if (null == JSON) return new StringBuilder("No JSON data !");
            VisualNode3 node = JSON.Node_Viewer;
            Stack<VisualNode3> s = new Stack<VisualNode3>();
            s.Push(node);
            List<String> path = new List<String>();
            StringBuilder text = new StringBuilder();
            int keyNo = 1;
            List<String> rows = new List<String>();
            StringBuilder rowBd = null;
            //if (id.Length == 0) sortLast = false;
            if (sortLast) rowBd = new StringBuilder();
            while (s.Count > 0)
            {
                node = s.Pop();
                if (null != node.m_JsonNode.node)
                {
                    while (path.Count - s.Count > 1) {
                        path.RemoveAt(0);
                        keyNo = 1;
                    }
                    if (node.m_JsonNode.HasKey) path.Add(node.Key_Viewer);
                    else {
                        if (sortLast) path.Add(""); // path.Add($"No{keyNo++:00000000}");
                        else path.Add($"No{keyNo++}");
                    }
                    if (null != node.m_JsonNode.next) s.Push(node.Next_Viewer);
                    s.Push(node.Node_Viewer);
                }
                else if (null != node.m_JsonNode.next)
                {
                    int row = 0;
                    if (sortLast) {
                        SortedList<String, String> lastChildrens = new SortedList<String, String>();
                        VisualNode3 keyNode = node;
                        while (null != keyNode)
                        {
                            if (JsonTag.JSON_ARRAY == keyNode.Tag_Viewer || JsonTag.JSON_OBJECT == keyNode.Tag_Viewer) {
                                s.Push(keyNode);
                                if (null != keyNode.m_JsonNode.next) break;
                            } else {
                                if (id == keyNode.Key_Viewer) {
                                    if (path.Count > 0) path[path.Count - 1] = keyNode.Value_Viewer;
                                    else path.Add(keyNode.Value_Viewer);
                                }
                                else if (sortLast) lastChildrens[keyNode.Key_Viewer] = keyNode.Value_Viewer;
                            }
                            keyNode = keyNode.Next_Viewer;
                        }
                        String key = "";
                        key = String.Join("\\", path.ToArray());
                        if (0 == row++ && horizontal) rowBd.Append(key).Append('\\');
                        foreach (KeyValuePair<String, String> item in lastChildrens)
                        {
                            if (horizontal) rowBd.Append('\t').Append(item.Key).Append('\t').Append(item.Value);
                            else
                            {
                                rowBd.Append(key).Append('\\').Append('\t').Append(item.Key).Append('\t').Append(item.Value).Append('\n');
                            }
                        }
                        if (horizontal) rowBd.Append('\n');
                        rows.Add(rowBd.ToString());
                        rowBd.Length = 0;
                    } else {
                        JsonNode keyNode = node.m_JsonNode;
                        if(id.Length > 0) {
                            while (null != keyNode) {
                                if (id == keyNode.Key(JSONdata)) {
                                    if(path.Count > 0) path[path.Count - 1] = keyNode.ToString(JSONdata);
                                    else path.Add(keyNode.ToString(JSONdata));
                                    break;
                                }
                                keyNode = keyNode.next;
                            }
                        }
                        do {
                            if (JsonTag.JSON_ARRAY == node.Tag_Viewer || JsonTag.JSON_OBJECT == node.Tag_Viewer) {
                                s.Push(node);
                                if (null != node.m_JsonNode.next) break;
                            } else {
                                if (id.Length == 0 || id != node.Key_Viewer) {
                                    if (0 == row++ || !horizontal) text.Append(String.Join("\\", path.ToArray())).Append("\\\t");
                                    else
                                    {
                                        if (horizontal) text.Append('\t'); else text.Append('\n');
                                    }
                                    text.Append(node.Key_Viewer).Append('\t').Append(node.Value_Viewer);
                                    if (!horizontal) text.Append('\n');
                                }
                            }
                            node = node.Next_Viewer;
                        } while (null != node);
                        if (horizontal) text.Append('\n');
                    }
                    if(path.Count > 0)
                        path.RemoveAt(path.Count-1);
                }
            }
            if (sortLast) {
                rows.Sort();
                foreach (var item in rows)
                {
                    text.Append(item);
                }
            }
            return text;
        }
        internal void Convert2CSVquick(StreamWriter print2, Boolean horizontal, Boolean sortLast, String id)
        {
            if (null == JSON)
            {
                print2.WriteLine("No JSON data !");
                return;
            }
            JsonNode node = JSON.m_JsonNode.node;
            Stack<JsonNode> s = new Stack<JsonNode>();
            s.Push(node);
            List<String> path = new List<String>();
            int keyNo = 1;
            List<String> rows = new List<String>();
            StringBuilder rowBd = null;
            //if (id.Length == 0) sortLast = false;
            if (sortLast) rowBd = new StringBuilder();
            while (s.Count > 0)
            {
                node = s.Pop();
                if (null != node.node)
                {
                    while (path.Count - s.Count > 1)
                    {
                        path.RemoveAt(0);
                        keyNo = 1;
                    }
                    if (node.HasKey) path.Add(node.Key(JSONdata));
                    else {
                        if (sortLast) path.Add(""); // path.Add($"No{keyNo++:00000000}");
                        else path.Add($"No{keyNo++}");
                    }
                    if (null != node.next) s.Push(node.next);
                    s.Push(node.node);
                }
                else if (null != node.next)
                {
                    int row = 0;
                    JsonNode keyNode = node;
                    if (sortLast)
                    {
                        SortedList<String, String> lastChildrens = new SortedList<String, String>();
                        while (null != keyNode)
                        {
                            if (JsonTag.JSON_ARRAY == keyNode.Tag || JsonTag.JSON_OBJECT == keyNode.Tag) s.Push(keyNode);
                            else {
                                if (id == keyNode.Key(JSONdata)) {
                                    if (path.Count > 0) path[path.Count - 1] = keyNode.ToString(JSONdata);
                                    else path.Add(keyNode.ToString(JSONdata));
                                } else if (sortLast) lastChildrens[keyNode.Key(JSONdata)] = keyNode.ToString(JSONdata);
                            }
                            keyNode = keyNode.next;
                        }
                        String key = "";
                        key = String.Join("\\", path.ToArray());
                        if (0 == row++ && horizontal) rowBd.Append(key).Append('\\');
                        foreach (KeyValuePair<String, String> item in lastChildrens)
                        {
                            if (horizontal) rowBd.Append('\t').Append(item.Key).Append('\t').Append(item.Value);
                            else
                            {
                                rowBd.Append(key).Append('\\').Append('\t').Append(item.Key).Append('\t').Append(item.Value).Append('\n');
                            }
                        }
                        if (horizontal) rowBd.Append('\n');
                        rows.Add(rowBd.ToString());
                        rowBd.Length = 0;
                    } else {
                        do {
                            if(id.Length > 0) {
                                while (null != keyNode) {
                                    if (id == keyNode.Key(JSONdata)) {
                                        if (path.Count > 0) path[path.Count - 1] = keyNode.ToString(JSONdata);
                                        else path.Add(keyNode.ToString(JSONdata));
                                        break;
                                    }
                                    keyNode = keyNode.next;
                                }
                            }
                            if (JsonTag.JSON_ARRAY == node.Tag || JsonTag.JSON_OBJECT == node.Tag) s.Push(node);
                            else {
                                if (id.Length == 0 || id != node.Key(JSONdata)) {
                                    if (0 == row++ || !horizontal) {
                                        print2.Write(String.Join("\\", path.ToArray()));
                                        print2.Write("\\\t");
                                    } else {
                                        if (horizontal) print2.Write('\t'); else print2.Write('\n');
                                    }
                                    print2.Write(node.Key(JSONdata));
                                    print2.Write('\t');
                                    print2.Write(node.ToString(JSONdata));
                                    if (!horizontal) print2.Write('\n');
                                }
                            }
                            node = node.next;
                        } while (null != node);
                        if (horizontal) print2.Write('\n');
                    }
                    if (path.Count > 0)
                        path.RemoveAt(path.Count - 1);
                }
            }
            if (sortLast) {
                rows.Sort();
                foreach (var item in rows)
                {
                    print2.Write(item);
                }
            }
            print2.Flush();
        }
    }
}
