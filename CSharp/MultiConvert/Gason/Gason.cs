using System;

namespace Gason
{
    public class Parser
    {
        readonly bool FloatAsDecimal = false;
        readonly int JSON_STACK_SIZE;
#if DEBUGGING
        public VisualNode3[] tails;
        public LinkedByteString[] keys;
#else
        public JsonNode[] tails;
        public P_ByteLnk[] keys;
#endif
        public JsonTag[] tags;
        JsonNode o;
#if DEBUGGING
        public VisualNode3 root;
#endif
        public int pos = -1;
        public bool separator = true;
        public Byte prevType = 255;
        public int len;
        public int strPos;
        Byte type;
#if KEY_SPLIT
        Boolean insideLimitBlock, bubbleOut;
#endif
        public void Init(ref Byte[] s, bool nextInit)
        {
            o = new JsonNode();
#if DEBUGGING
            tails = new VisualNode3[JSON_STACK_SIZE];
            LinkedByteString.storage = s;
            keys = new LinkedByteString[JSON_STACK_SIZE];
            root = new VisualNode3(ref o, s, 3000); // Predefined JSON preview size limit (interactive then, also indent 0/-1 or m_Shift_Width)
#else
        tails = new JsonNode[JSON_STACK_SIZE];
        keys = new P_ByteLnk[JSON_STACK_SIZE];
#endif
            tags = new JsonTag[JSON_STACK_SIZE];
            pos = -1;
#if !SKIP_VALIDATION
            separator = true;
            prevType = 255;
#endif
            type = 255;
            len = s.Length;
            strPos = 0;
#if KEY_SPLIT
            insideLimitBlock = false;
            bubbleOut = false;
#endif
        }
        public Parser(bool FloatAsDecimal, int JSON_STACK_SIZE = 32)
        {
            this.FloatAsDecimal = FloatAsDecimal;
            this.JSON_STACK_SIZE = JSON_STACK_SIZE;
        }
        public JsonErrno Parse(Byte[] s, ref int endPos, out JsonNode value
#if KEY_SPLIT
            , ByteString[] keysLog, int level, int startPos, int length
#endif
            )
        {
            int endPosMem = endPos;
            Init(ref s, (endPos > 0));
            value = null;
            while (strPos < len)
            {
#if !SKIP_VALIDATION
                if(type > 1) prevType = type;
#endif
                type = SearchTables.valTypes[s[strPos]];
                if (type <= 1)
                {
                    strPos++;
                    continue; // white space
                }
                endPos = strPos++;
                switch (type) // switch (**endptr) {
                {
                    case 2: // case '-':
                        if(FloatAsDecimal) {
                            endPos = o.String2decimal(ref strPos, s, true);
                        } else {
                            endPos = o.String2double(ref strPos, s, true);
                        }
#if !SKIP_VALIDATION
                        if (0 == (SearchTables.specialTypes[s[endPos]] & 3)) // isdelim
                        {
                            endPos = strPos;
                            return JsonErrno.BAD_NUMBER;
                        }
#endif
                        break;
                    case 4: // 0-9
                        strPos--;
                        if(FloatAsDecimal) {
                            endPos = o.String2decimal(ref strPos, s);
                        } else {
                            endPos = o.String2double(ref strPos, s);
                        }
#if !SKIP_VALIDATION
                        if (0 == (SearchTables.specialTypes[s[endPos]] & 3)) // isdelim
                        {
                            endPos = strPos;
                            return JsonErrno.BAD_NUMBER;
                        }
#endif
                        break;
                    case 3: // case '"':
                        JsonErrno e = o.GetString(ref strPos, s);
                        if (e != JsonErrno.OK) return e;
#if !SKIP_VALIDATION
                        if (0 == (SearchTables.specialTypes[s[strPos]] & 3)) // !isdelim
                        {
                            endPos = strPos;
                            return JsonErrno.BAD_STRING;
                        }
#endif
                        break;
                    case 7: // 't'
                        if ((SearchTables.specialTypes[s[strPos + 3]] & 3) != 0 // isdelim
                        && (s[strPos + 0] == 'r')
                        && (s[strPos + 1] == 'u')
                        && (s[strPos + 2] == 'e'))
                        {
                            o.Tag = JsonTag.JSON_TRUE;
                            strPos += 3;
#if !SKIP_VALIDATION
                        }
                        else {
                            return JsonErrno.BAD_IDENTIFIER;
#endif
                        }
                        break;
                    case 6: // 'f'
                        if ((SearchTables.specialTypes[s[strPos + 4]] & 3) != 0 // isdelim
                        && (s[strPos + 0] == 'a')
                        && (s[strPos + 1] == 'l')
                        && (s[strPos + 2] == 's')
                        && (s[strPos + 3] == 'e'))
                        {
                            o.Tag = JsonTag.JSON_FALSE;
                            strPos += 4;
#if !SKIP_VALIDATION
                        }
                        else {
                            return JsonErrno.BAD_IDENTIFIER;
#endif
                        }
                        break;
                    case 8: // 'n'
#if !SKIP_VALIDATION
                        if (prevType == 3 && !separator) { // {"Missing colon" null} + fail19.json
                            return JsonErrno.UNEXPECTED_CHARACTER;
                        }
#endif
                        if ((SearchTables.specialTypes[s[strPos + 3]] & 3) != 0 // isdelim
                        && (s[strPos + 0] == 'u')
                        && (s[strPos + 1] == 'l')
                        && (s[strPos + 2] == 'l'))
                        {
                            o.Tag = JsonTag.JSON_NULL;
                            strPos += 3;
#if !SKIP_VALIDATION
                        }
                        else {
                            return JsonErrno.BAD_IDENTIFIER;
#endif
                        }
                        break;
                    case 12: // ']'
#if !SKIP_VALIDATION
                        if (pos == -1)
                            return JsonErrno.STACK_UNDERFLOW;
                        if (tags[pos] != JsonTag.JSON_ARRAY)
                            return JsonErrno.MISMATCH_BRACKET;
                        if (separator && prevType != 11) // '['
                            return JsonErrno.UNEXPECTED_CHARACTER; // fail4
#endif
#if DEBUGGING
                        o.ListToValue(JsonTag.JSON_ARRAY, tails[pos] != null ? tails[pos].m_JsonNode : null);
                        pos--;
#else
                        o.ListToValue(JsonTag.JSON_ARRAY, tails[pos--]);
#endif
#if !SKIP_VALIDATION
                        if (type > 1) prevType = type;
#endif
                        break;
                    case 13: // '}'
#if !SKIP_VALIDATION
                        if (pos == -1)
                            return JsonErrno.STACK_UNDERFLOW;
                        if (tags[pos] != JsonTag.JSON_OBJECT)
                            return JsonErrno.MISMATCH_BRACKET;
                        if (keys[pos].length != -1)
                            return JsonErrno.UNEXPECTED_CHARACTER;
                        if (separator && prevType != 10) // '{'
                            return JsonErrno.UNEXPECTED_CHARACTER;
#endif
#if DEBUGGING
                        o.ListToValue(JsonTag.JSON_OBJECT, tails[pos] != null ? tails[pos].m_JsonNode : null);
                        pos--;
#else
                        o.ListToValue(JsonTag.JSON_OBJECT, tails[pos--]);
#endif
#if KEY_SPLIT
                        if (insideLimitBlock && (level == pos + 1))
                        {
                            if (length == 1) {
                                bubbleOut = true;
                            } else if (length > 1){
                                length--;
                            }
                        }
#endif
                        break;
                    case 11: // '['
#if !SKIP_VALIDATION
                        if (++pos == JSON_STACK_SIZE)
                            return JsonErrno.STACK_OVERFLOW;
#else
                        pos++;
#endif
                        tails[pos] = null;
                        tags[pos] = JsonTag.JSON_ARRAY;
                        keys[pos].length = -1;
#if !SKIP_VALIDATION
                        separator = true;
#endif
                        continue;
                    case 10: // '{'
#if !SKIP_VALIDATION
                        if (++pos == JSON_STACK_SIZE)
                            return JsonErrno.STACK_OVERFLOW;
#else
                        pos++;
#endif
#if KEY_SPLIT
                        if (pos == level && !insideLimitBlock)
                        {
                            int i = level - 1;
                            while (i >= 0)
                            {
                                if (keys[i].length != -1)
                                {
#if DEBUGGING
                                    if (keysLog[i].Equals(s, keys[i].idxes)) i--;
#else
                                    if (keysLog[i].Equals(s, keys[i])) i--;
#endif
                                    else break;
                                } else if (keysLog[i] == null) i--; else break;
                            }
                            if(i == -1)
                            { // Keys & level match
                                insideLimitBlock = true;
                                if (startPos > 0) strPos = endPosMem;
                            }
                        }
#endif
                        tails[pos] = null;
                        tags[pos] = JsonTag.JSON_OBJECT;
                        keys[pos].length = -1;
#if !SKIP_VALIDATION
                        separator = true;
#endif
                        continue;
                    case 14: // ':'
#if !SKIP_VALIDATION
                        if (separator || keys[pos].length == -1)
                            return JsonErrno.UNEXPECTED_CHARACTER;
                        separator = true;
#endif
                        continue;
                    case 15: // ','
#if !SKIP_VALIDATION
                        if (separator || keys[pos].length != -1)
                            return JsonErrno.UNEXPECTED_CHARACTER;
                        separator = true;
#endif
                        continue;
#if !SKIP_VALIDATION
                    default:
                        return JsonErrno.UNEXPECTED_CHARACTER;
#endif
                }
#if !SKIP_VALIDATION
                separator = false;
#endif

                if (pos == -1)
                {
                    value = o;
#if !SKIP_VALIDATION
                    while (strPos < len - 1)
                    {
                        if (type == 13) { // '}' / {"Extra value after close": true} "misplaced quoted value"
                            type = 0;
                            while (strPos < len - 1) {
                                type = SearchTables.valTypes[s[strPos]];
                                if (type <= 1) {
                                    strPos++;
                                    continue;
                                }
                                if (type == 3)
                                {
                                    strPos++;
                                    JsonNode trash = new JsonNode();
                                    JsonErrno e = trash.GetString(ref strPos, s);
                                    if (e != JsonErrno.OK) return e;
                                    endPos = strPos;
                                    if (strPos != len -1 || 0 == (SearchTables.specialTypes[s[strPos]] & 3)) // !isdelim
                                    {
                                        return JsonErrno.BAD_STRING;
                                    } else return JsonErrno.OK;
                                }
                                else return JsonErrno.UNEXPECTED_CHARACTER;
                            }
                        }
                        if ((strPos < len -1) && SearchTables.valTypes[s[strPos]] <= 1) strPos++;
                        else
                        {
                            if (s[strPos] == 0) break;
                            if (prevType != 12 || s[strPos] != ',') // ']'
                                return JsonErrno.BREAKING_BAD;
                            else strPos++;
                        }
                    }
#endif
                    return JsonErrno.OK;
                }
#if KEY_SPLIT
                do
                {
#endif
                if (tags[pos] == JsonTag.JSON_OBJECT)
                    {
                        if (keys[pos].length == -1)
                        {
#if !SKIP_VALIDATION
                            if (o.Tag != JsonTag.JSON_STRING)
                                return JsonErrno.UNQUOTED_KEY;
#endif
#if DEBUGGING
                            keys[pos] = new LinkedByteString(o.doubleOrString);
#else
                            keys[pos].data = o.doubleOrString.data;
#endif
#if KEY_SPLIT
                            if (bubbleOut) continue;
                            else break;
#else
                            continue;
#endif
                    }
#if DEBUGGING
                    o.InsertAfter(tails[pos] != null ? tails[pos].m_JsonNode : null, ref keys[pos].idxes);
#else
                    o.InsertAfter(tails[pos] != null ? tails[pos] : null, ref keys[pos]);
#endif
                    }
                    else
                    {
#if DEBUGGING
                        o.InsertAfter(tails[pos] != null ? tails[pos].m_JsonNode : null);
#else
                        o.InsertAfter(tails[pos]);
#endif
                    }
                    tails[pos] =
#if DEBUGGING
                        new VisualNode3(ref o, s, 3000);
#else
                        o;
#endif
                    o = new JsonNode();
#if DEBUGGING
                    root.ChangeNode(o);
#endif

#if KEY_SPLIT
                    if (bubbleOut)
                    {
                        if (tags[pos] == JsonTag.JSON_ARRAY
                        || tags[pos] == JsonTag.JSON_OBJECT)
                        { // lists close brackets
#if DEBUGGING
                            o.ListToValue(tags[pos], tails[pos] != null ? tails[pos].m_JsonNode : null);
#else
                            o.ListToValue(tags[pos], tails[pos]);
#endif
                        }
                        if (pos-- == 0)
                        {
                            while ((strPos < len) && s[strPos++] != ',') ; // find array separator
                            while ((strPos < len) && ((SearchTables.specialTypes[s[strPos]] & 3) != 0)) strPos++; // skip delims
                            while ((strPos < len) && (s[strPos] != '{')) strPos++; // array start
                            if (strPos < len) strPos++;
                            endPos = strPos;
                            value = o;
                            return JsonErrno.OK;
                        }
                    }
                    else break;
                } while (true); // exit by breaks
#endif
            }
            return JsonErrno.BREAKING_BAD;
        }
    }
}
