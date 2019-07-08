using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace Gason
{
    class ValueWriter
    {
        int SHIFT_WIDTH;
#if DEBUGGING
        Stack<VisualNode3> levelStack;
#else
        Stack<JsonNode> levelStack;
#endif
        public ValueWriter(int width = 2)
        {
#if DEBUGGING
            levelStack = new Stack<VisualNode3>();
#else
            levelStack = new Stack<JsonNode>();
#endif
            SHIFT_WIDTH = width;
        }
        protected void BlockEnd(StreamWriter print2, JsonNode o, ref int indent, String newLine)
        {
            if (o.Tag == JsonTag.JSON_OBJECT)
            {
                if (indent > -1) {
                    indent -= SHIFT_WIDTH;
                    print2.Write(new String(' ', indent));
                }
                print2.Write("}" + ((o.next != null ? "," : "") + newLine));
            }
            else if (o.Tag == JsonTag.JSON_ARRAY)
            {
                if (indent > -1)
                {
                    indent -= SHIFT_WIDTH;
                    print2.Write(new String(' ', indent));
                }
                print2.Write("]" + ((o.next != null ? "," : "") + newLine));
            }
        }
        public void DumpValueIterative(StreamWriter print2, JsonNode o, Byte[] src, int indent = 0)
        {
            String space, newLine;
            if (indent > -1) { space = " "; newLine = "\n"; }
              else           { space = "";  newLine = ""; }
            JsonTag startTag;
            do
            {
#if DEBUGGING
                VisualNode3 oV = new VisualNode3(ref o, src, 10000);
#endif
                if (indent > -1) print2.Write(new String(' ', indent)); // Start with indent 
                startTag = o.Tag;
                if (startTag == JsonTag.JSON_OBJECT || startTag == JsonTag.JSON_ARRAY) {
                    String open = "";
                    if (startTag == JsonTag.JSON_ARRAY) open = "[]"; else open = "{}";
                    if (o.ToNode() == null) {
                        if (o.HasKey) print2.Write($"\"{o.Key(src)}\":{space}"); // [] or key: []
                        if (o.next == null) print2.Write($"{open}{newLine}");
                        else print2.Write($"{open},{newLine}");
                        if (o.next == null) o = o.node;
                    } else {
                        open = open.Substring(0, 1);
                        if (o.HasKey) print2.Write($"\"{o.Key(src)}\":{space}{open}");
                        else print2.Write($"{open}");
                        if(o.ToNode() != null) print2.Write(newLine);
                        if (o.ToNode() == null && o.next != null) BlockEnd(print2, o, ref indent, newLine);
                        if (indent > -1) indent += SHIFT_WIDTH;
                    }
                } else if(startTag == JsonTag.JSON_STRING || startTag == JsonTag.JSON_NUMBER || startTag == JsonTag.JSON_NUMBER_STR) {
                    String quote = (startTag == JsonTag.JSON_STRING) ? "\"" : "";
                    if (o.HasKey) {
                        print2.Write($"\"{o.Key(src)}\":{space}{quote}{o.ToString(src)}{quote}{(o.next!=null?",":"")}{newLine}"); // "key": "value"(,)
                    } else print2.Write($"{quote}{o.ToString(src)}{quote}{(o.next!=null?",":"")}{newLine}"); // "value"(,)
                } else if(startTag == JsonTag.JSON_TRUE || startTag == JsonTag.JSON_FALSE || startTag == JsonTag.JSON_NULL) {
                    String word;
                    if (startTag == JsonTag.JSON_TRUE) word = "true";
                    else if (startTag == JsonTag.JSON_FALSE) word = "false";
                    else word = "null";
                    if (o.HasKey) {
                        print2.Write($"\"{o.Key(src)}\":{space}{word}{(o.next!=null?",":"")}{newLine}"); // "key": "value"(,)
                    } else print2.Write($"{word}{(o.next!=null?",":"")}{newLine}"); // "value"(,)
                }
                if(o != null) {
                    if (o.node != null && (startTag == JsonTag.JSON_ARRAY || startTag == JsonTag.JSON_OBJECT))
                    { // move down 2 node of structured object
#if DEBUGGING
                        levelStack.Push(new VisualNode3(ref o, src, 1000));
#else
                        levelStack.Push(o);
#endif
                        o = o.node;
                    } else { // move right to values
                        if (o.next != null) o = o.next;
                        else o = o.node; // always null (4 null || non-structured)
                    }
                }
                while (o == null && levelStack.Count > 0)
                { // return back after iterations
                    do {
#if DEBUGGING
                        o = levelStack.Pop().m_JsonNode;
                        oV.ChangeNode(o);
#else
                        o = levelStack.Pop();
#endif
                        if (o.Tag == JsonTag.JSON_ARRAY || o.Tag == JsonTag.JSON_OBJECT)
                        { // Array / Object end markers
                            BlockEnd(print2, o, ref indent, newLine);
                        } else {
                            BlockEnd(print2, o, ref indent, newLine); // Array / Object end markers
                        }
                    } while ((levelStack.Count > 1) && ((o == null || (o.next == null && (o.node == null || o.node.next == null)))));
                    o = o.next; // move right
                }
            } while (o != null || (levelStack.Count > 0)) ;
        }
        public void DumpValue(StreamWriter print2, JsonNode o, Byte[] src, int indent = 0)
        {
            JsonNode i;
            if (o.Tag == JsonTag.JSON_NUMBER)
            {
                print2.Write(o.ToNumber().ToString(System.Globalization.CultureInfo.InvariantCulture));
            } else if(o.Tag == JsonTag.JSON_NUMBER_STR)
            {
                print2.Write(o.ToString(src));
            } else if(o.Tag == JsonTag.JSON_STRING)
            {
                print2.Write($"\"{ o.ToString(src) }\"");
            } else if (o.Tag == JsonTag.JSON_ARRAY)
            {
                // It is not necessary to use o.toNode() to check if an array or object
                // is empty before iterating over its members, we do it here to allow
                // nicer pretty printing.
                if (null == o.ToNode())
                {
                    print2.Write("[]");
                    return;
                }
                if (indent > -1)
                {
                    print2.Write("[\n");
                }
                else
                {
                    print2.Write('[');
                }
                i = o.node;
                while (null != i)
                {
                    if (indent > -1)
                        print2.Write(new String(' ', indent + SHIFT_WIDTH));
                    DumpValue(print2, i, src, indent > -1 ? indent + SHIFT_WIDTH : indent);
                    if (indent > -1)
                    {
                        print2.Write(i.next != null ? ",\n" : "\n");
                    }
                    else if (i.next != null)
                    {
                        print2.Write(",");
                    }
                    i = i.next;
                }
                print2.Write((indent > -1) ? (new String(' ', indent) + ']') : "]");
            } else if (o.Tag == JsonTag.JSON_OBJECT)
            {
                if (null == o.ToNode())
                {
                    print2.Write("{}");
                    return;
                }
                if (indent > -1)
                {
                    print2.Write("{\n");
                }
                else
                {
                    print2.Write("{");
                }
                i = o.node;
                while (null != i)
                {
                    if (indent > -1)
                        print2.Write(new String(' ', indent + SHIFT_WIDTH));
                    print2.Write($"\"{ i.Key(src) }\"");
                    if (indent > -1)
                    {
                        print2.Write(": ");
                    }
                    else
                    {
                        print2.Write(':');
                    }
                    DumpValue(print2, i, src, indent > -1 ? indent + SHIFT_WIDTH : indent);
                    if (indent > -1)
                    {
                        print2.Write(i.next != null ? ",\n" : "\n");
                    }
                    else if (i.next != null)
                    {
                        print2.Write(",");
                    }
                    i = i.next;
                }
                print2.Write(((indent > -1) ? new String(' ', indent) : "") + '}');
            } else if (o.Tag == JsonTag.JSON_TRUE) {
                print2.Write("true");
            } else if (o.Tag == JsonTag.JSON_FALSE)
            {
                print2.Write("false");
            } else if (o.Tag == JsonTag.JSON_NULL)
            {
                print2.Write("null");
            }
        }
    }
}
