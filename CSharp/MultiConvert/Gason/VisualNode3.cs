using System;
using System.Collections.Generic;
using System.Text;

namespace Gason
{
    public class VisualNode3
    {
        public JsonNode m_JsonNode;
        readonly Byte[] src;
        int m_Shift_Width = 2;
        public int m_Indent { get; set; } = 0;
        public int m_debugModeLimit { get; set; }
        Stack<JsonNode> levelStack = new Stack<JsonNode>();
        List<int> nos;
        public VisualNode3(ref JsonNode my, Byte[] src, int debugModeLimit)
        {
            m_JsonNode = my;
            this.src = src;
            m_debugModeLimit = debugModeLimit;
        }
        public VisualNode3 Next_Viewer
        {
            get {
                if (m_JsonNode != null && m_JsonNode.next != null)
                {
                    return new VisualNode3(ref m_JsonNode.next, src, m_debugModeLimit);
                }
                return null;
            }
        }
        public VisualNode3 Node_Viewer
        {
            get
            {
                if (m_JsonNode != null && m_JsonNode.ToNode() != null)
                {
                    return new VisualNode3(ref m_JsonNode.node, src, m_debugModeLimit);
                }
                else return null;
            }
        }
        public JsonTag Tag_Viewer { get { return m_JsonNode.Tag; } }
        public String Key_Viewer {
            get {
                return m_JsonNode.KeyView(src);
            }
        }
        public String Value_Viewer { get { return new ByteString(src, m_JsonNode.doubleOrString).ToString(); } }
        public void ChangeNode(JsonNode o)
        {
            m_JsonNode = o;
        }
        protected void BlockEnd(JsonNode o, ref String retVal, String newLine)
        {
            if (o.Tag == JsonTag.JSON_OBJECT)
            {
                if (m_Indent > -1) {
                    m_Indent -= m_Shift_Width;
                    retVal += new String(' ', m_Indent);
                }
                retVal += ("}" + ((o.next != null ? "," : "") + newLine));
            }
            else if (o.Tag == JsonTag.JSON_ARRAY)
            {
                if (m_Indent > -1)
                {
                    m_Indent -= m_Shift_Width;
                    retVal += new String(' ', m_Indent);
                }
                retVal += ("]" + ((o.next != null ? "," : "") + newLine));
            }
        }
        protected void BlockEndXML(JsonNode o, ref StringBuilder retVal, String newLine)
        {
            if (o.Tag != JsonTag.JSON_OBJECT && o.Tag != JsonTag.JSON_ARRAY) return;
            if (m_Indent > -1) {
                m_Indent -= m_Shift_Width;
                if(m_Indent > 0) retVal.Append(' ', m_Indent);
            }
            if (o.HasKey) retVal.Append("</").Append(o.Key(src)).Append('>').Append(newLine);
            else {
                int no = nos.Count - 1;
                retVal.Append("</No").Append(nos[no]++).Append('>').Append(newLine);
                if(null == o.next) nos[no] = 1;
            }
        }
        public String DumpValueIterative(JsonNode o, Boolean debugModeLimit)
        {
            JsonNode startNode = o;
            String space, newLine, retVal = "";
            if (m_Indent > -1) { space = " "; newLine = "\n"; }
            else { space = ""; newLine = ""; }
            JsonTag startTag;
            do
            {
                if(debugModeLimit && retVal.Length > m_debugModeLimit)
                {
                    if (m_Indent > -1) m_Indent = 0;
                    return retVal + "\n...";
                }
                if (m_Indent > -1) retVal +=(new String(' ', m_Indent)); // Start with m_Indent 
                startTag = o.Tag;
                if (startTag == JsonTag.JSON_OBJECT || startTag == JsonTag.JSON_ARRAY) {
                    String open = "";
                    if (startTag == JsonTag.JSON_ARRAY) open = "[]"; else open = "{}";
                    if (o.ToNode() == null) {
                        if (o.HasKey) retVal += ($"\"{o.Key(src)}\":{space}"); // [] or key: []
                        if (o.next == null) retVal += ($"{open}{newLine}");
                        else retVal += ($"{open},{newLine}");
                        if (o.next == null) o = o.node;
                    } else {
                        open = open.Substring(0, 1);
                        if (o.HasKey) retVal += ($"\"{o.Key(src)}\":{space}{open}");
                        else retVal +=($"{open}");
                        if(o.ToNode() != null) retVal += (newLine);
                        if (o.ToNode() == null && o.next != null) BlockEnd(o, ref retVal, newLine);
                        if (m_Indent > -1) m_Indent += m_Shift_Width;
                    }
                } else if (startTag == JsonTag.JSON_STRING || startTag == JsonTag.JSON_NUMBER || startTag == JsonTag.JSON_NUMBER_STR) {
                    String quote = (startTag == JsonTag.JSON_STRING) ? "\"" : "";
                    if (o.HasKey) {
                        retVal +=($"\"{o.Key(src)}\":{space}{quote}{o.ToString(src)}{quote}{(o.next != null ? "," : "")}{newLine}"); // "key": "value"(,)
                    } else retVal +=($"{quote}{o.ToString(src)}{quote}{(o.next != null ? "," : "")}{newLine}"); // "value"(,)
                } else if (startTag == JsonTag.JSON_TRUE || startTag == JsonTag.JSON_FALSE || startTag == JsonTag.JSON_NULL) {
                    String word;
                    if (startTag == JsonTag.JSON_TRUE) word = "true";
                    else if (startTag == JsonTag.JSON_FALSE) word = "false";
                    else word = "null";
                    if (o.HasKey) {
                        retVal +=($"\"{o.Key(src)}\":{space}{word}{(o.next != null ? "," : "")}{newLine}"); // "key": "value"(,)
                    } else retVal +=($"{word}{(o.next != null ? "," : "")}{newLine}"); // "value"(,)
                }
                if(o != null) {
                if (o.node != null && (startTag == JsonTag.JSON_ARRAY || startTag == JsonTag.JSON_OBJECT))
                { // move down 2 node of structured object
                    levelStack.Push(o);
                    o = o.node;
                    } else { // move right to values
                    if (o.next != null) o = o.next;
                        else o = o.node; // always null (4 null || non-structured)
                    }
                }
                while (o == null && levelStack.Count > 0)
                { // return back after iterations
                    do {
                        o = levelStack.Pop();
                        if (o.Tag == JsonTag.JSON_ARRAY || o.Tag == JsonTag.JSON_OBJECT)
                        { // Array / Object end markers
                            BlockEnd(o, ref retVal, newLine);
                        } else {
                            BlockEnd(o, ref retVal, newLine); // Array / Object end markers
                        }
                    } while ((levelStack.Count > 1) && ((o == null || (o.next == null && (o.node == null || o.node.next == null)))));
                    o = o.next; // move right
                }
                if (o == startNode)
                {
                    if (m_Indent > -1) m_Indent = 0;
                    return retVal + "\n... cycle here";
                }
            } while (o != null || (levelStack.Count > 0)) ;
            return retVal;
        }
        public StringBuilder DumpXMLValueIterative(JsonNode o)
        {
            JsonNode startNode = o;
            String newLine;
            StringBuilder retVal = new StringBuilder();
            if (m_Indent > -1) { newLine = "\n"; m_Indent = 0; }
            else { newLine = ""; }
            JsonTag startTag;
            nos = new List<int>();
            do
            {
                if (m_Indent > -1) retVal.Append(' ', m_Indent); // Start with m_Indent 
                startTag = o.Tag;
                if (startTag == JsonTag.JSON_OBJECT || startTag == JsonTag.JSON_ARRAY) {
                    String open = "";
                    if (startTag == JsonTag.JSON_ARRAY) open = "<EmptyArray></EmptyArray>"; else open = "<EmptyObject></EmptyObject>";
                    if (o.ToNode() == null) {
                        if (o.HasKey) retVal.Append('<').Append(o.Key(src)).Append('>');
                        if (o.next == null) retVal.Append(open);
                        else retVal.Append(open).Append(newLine);
                        if (o.HasKey) retVal.Append("</").Append(o.Key(src)).Append(">");
                        if (o.next == null) retVal.Append(newLine);
                        if (o.next == null) o = o.node;
                    } else {
                        if (o.HasKey) retVal.Append('<').Append(o.Key(src)).Append(">");
                        else {
                            if (startNode == o) {
                                nos.Add(1);
                                retVal.Append("<ROOT>");
                            } else retVal.Append("<No").Append(nos[nos.Count-1]).Append('>');
                            while (nos.Count < levelStack.Count) nos.Add(1);
                        }
                        if (o.ToNode() != null) retVal.Append(newLine);
                        if (o.ToNode() == null && o.next != null) BlockEndXML(o, ref retVal, newLine);
                        if (m_Indent > -1) m_Indent += m_Shift_Width;
                    }
                } else if (startTag == JsonTag.JSON_STRING || startTag == JsonTag.JSON_NUMBER || startTag == JsonTag.JSON_NUMBER_STR) {
                    String quote = (startTag == JsonTag.JSON_STRING) ? "\"" : "";
                    if (o.HasKey) {
                        retVal.Append('<').Append(o.Key(src)).Append('>').Append(o.ToString(src)).Append("</").Append(o.Key(src)).Append('>').Append(newLine); // "key": "value"(,)
                    } else retVal.Append(quote).Append(o.ToString(src)).Append(quote).Append($"{(o.next != null ? "," : "")}{newLine}"); // "value"(,)
                } else if (startTag == JsonTag.JSON_TRUE || startTag == JsonTag.JSON_FALSE || startTag == JsonTag.JSON_NULL) {
                    String word;
                    if (startTag == JsonTag.JSON_TRUE) word = "true";
                    else if (startTag == JsonTag.JSON_FALSE) word = "false";
                    else word = "null";
                    if (o.HasKey) {
                        retVal.Append('<').Append(o.Key(src)).Append('>').Append(word).Append("</").Append(o.Key(src)).Append('>').Append(newLine); // "key": "value"(,)
                    } else retVal.Append(word).Append($"{(o.next != null ? "," : "")}{newLine}"); // "value"(,)
                }
                if(o != null) {
                    if (o.node != null && (startTag == JsonTag.JSON_ARRAY || startTag == JsonTag.JSON_OBJECT))
                    { // move down 2 node of structured object
                        levelStack.Push(o);
                        o = o.node;
                    } else { // move right to values
                        if (o.next != null) o = o.next;
                        else o = o.node; // always null (4 null || non-structured)
                    }
                }
                while (o == null && levelStack.Count > 0)
                { // return back after iterations
                    do {
                        o = levelStack.Pop();
                        if (o.Tag == JsonTag.JSON_ARRAY || o.Tag == JsonTag.JSON_OBJECT)
                        { // Array / Object end markers
                            if (o == startNode) return retVal.Append("</ROOT>");
                            else BlockEndXML(o, ref retVal, newLine);
                        }
                    } while ((levelStack.Count > 1) && ((o == null || (o.next == null && (o.node == null || o.node.next == null)))));
                    o = o.next; // move right
                }
                if (o == startNode)
                {
                    if (m_Indent > -1) m_Indent = 0;
                    return retVal.Append("\n... cycle here");
                }
            } while (o != null || (levelStack.Count > 0));
            return retVal;
        }
        public override string ToString()
        {
            return _JSON;
        }
        public string _JSON
        {
            get {
                if (m_JsonNode == null) return "NULL";
                return DumpValueIterative(m_JsonNode, m_debugModeLimit > 0);
            }
        }
    }
}
