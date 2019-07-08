using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace Gason
{
    public class JsonNode
    { // 16B
        public JsonNode next; // 4B
        protected P_ByteLnk keyIdxes;

        public P_ByteLnk doubleOrString;
        public JsonNode node;

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public JsonNode() {}
        public String Key(Byte[] src)
        {
            if (keyIdxes.pos + keyIdxes.length < src.Length)
                return Encoding.UTF8.GetString(src, keyIdxes.pos, keyIdxes.length);
            else return "Key index out of string !";
        }
        public String KeyView(Byte[] src)
        {
            if (keyIdxes.length == 0) return $"Empty:{keyIdxes.pos},{keyIdxes.length}";
            else return Encoding.UTF8.GetString(src, keyIdxes.pos, keyIdxes.length);
        }
        public Boolean HasKey  { get { return (keyIdxes.pos != 0) || (keyIdxes.length != 0); } }
        public String Key2str(Byte[] src)
        {
            if (keyIdxes.length == 0) return "";
            else return Encoding.UTF8.GetString(src, keyIdxes.pos, keyIdxes.length) + " :";
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void InsertAfter(JsonNode orig)
        {
            if (orig == null)
            {
                next = this;
                return;
            }
            next = orig.next;
            orig.next = this;
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void InsertAfter(JsonNode orig, ref P_ByteLnk key)
        {
            keyIdxes.data = key.data;
            key.length = -1;
            if (orig == null)
            {
                next = this;
                return;
            }
            next = orig.next;
            orig.next = this;
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void ListToValue(JsonTag tag, JsonNode tail)
        {
            Tag = tag;
            JsonNode head = tail;
            if (null != head)
            {
                head = tail.next;
                tail.next = null;
                node = head;
                doubleOrString = head.doubleOrString;
            }
        }
        public JsonTag Tag = JsonTag.JSON_NULL;
        public ByteString GetFatData(Byte[] src) { return new ByteString(src, doubleOrString); }
        public string ToString(Byte[] src)
        {
            switch (Tag)
            {
                case JsonTag.JSON_NUMBER:
                    return ToNumber().ToString(System.Globalization.CultureInfo.InvariantCulture);
                case JsonTag.JSON_NUMBER_STR:
                    return Encoding.ASCII.GetString(src, doubleOrString.pos, doubleOrString.length);
                case JsonTag.JSON_FALSE:
                    return "false";
                case JsonTag.JSON_TRUE:
                    return "true";
                case JsonTag.JSON_NULL:
                    return "null";
                case JsonTag.JSON_STRING:
                    return Encoding.UTF8.GetString(src, doubleOrString.pos, doubleOrString.length);
                case JsonTag.JSON_ARRAY:
                    return "Array";
                case JsonTag.JSON_OBJECT:
                    return "Object";
            }
            return "#N/A";
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public int String2double(ref int refPos, Byte[] src, bool minus = false)
        { // -[0-9]*.[0-9]+[eE][0-9]+
            doubleOrString.data = 0; // number = 0
#if !SKIP_VALIDATION
            if(src[refPos] == '0' && SearchTables.valTypes[src[refPos+1]] == 4) return refPos;
#endif
            while (SearchTables.valTypes[src[refPos]] == 4)
                doubleOrString.number = (doubleOrString.number * 10) + (src[refPos++] - '0');

            if (src[refPos] == '.')
            {
                ++refPos;

                double fraction = 1;
                while (SearchTables.valTypes[src[refPos]] == 4)
                {
                    fraction *= 0.1;
                    doubleOrString.number += (src[refPos++] - '0') * fraction;
                }
            }

            if (SearchTables.valTypes[src[refPos]] == 5)
            {
                ++refPos;

                double vbase = 10;
                if (src[refPos] == '+')
                    ++refPos;
                else if (src[refPos] == '-')
                {
                    ++refPos;
                    vbase = 0.1;
                }

                uint exponent = 0;
#if !SKIP_VALIDATION
                if (SearchTables.valTypes[src[refPos]] != 4) return --refPos;
#endif
                while (SearchTables.valTypes[src[refPos]] == 4)
                    exponent = (exponent * 10) + ((uint)src[refPos++] - '0');

                double power = 1;
                for (; exponent != 0; exponent >>= 1, vbase *= vbase)
                    if ((exponent & 1) != 0)
                        power *= vbase;

                doubleOrString.number *= power;
            }

            if (minus) doubleOrString.number = -doubleOrString.number;
            Tag = JsonTag.JSON_NUMBER;
            return refPos;
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public int String2decimal(ref int refPos, Byte[] src, bool minus = false)
        { // String2double copy
            if (minus) doubleOrString.pos = refPos - 1; else doubleOrString.pos = refPos;
#if !SKIP_VALIDATION
            if (src[refPos] == '0' && SearchTables.valTypes[src[refPos + 1]] == 4) return refPos;
#endif
            while (SearchTables.valTypes[src[refPos]] == 4) refPos++;

            if (src[refPos] == '.')
            {
                ++refPos;

                while (SearchTables.valTypes[src[refPos]] == 4) refPos++;
            }

            if (SearchTables.valTypes[src[refPos]] == 5)
            {
                ++refPos;

                if (src[refPos] == '+' || src[refPos] == '-')
                    ++refPos;
#if !SKIP_VALIDATION
                if (SearchTables.valTypes[src[refPos]] != 4) return --refPos;
#endif
                while (SearchTables.valTypes[src[refPos]] == 4) refPos++;
            }

            Tag = JsonTag.JSON_NUMBER_STR;
            doubleOrString.length = refPos - doubleOrString.pos;
            return refPos;
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public JsonErrno GetString(ref int refPos, Byte[] src)
        {
            doubleOrString.pos = refPos;
            int end = refPos;
            while (src[end] != '"')
            {
                if (src[end] == '\\')
                {
                    end++;
                    if (src[end] == 'u')
                    {
                        end += 4;
#if !SKIP_VALIDATION
                    }
                    else
                    { // or \\,\"./,\b\f\n\r\t
                        if ("\\\"/bfnrt".IndexOf((char)src[end]) < 0)
                            return JsonErrno.BAD_STRING;
#endif
                    }
                }
#if !SKIP_VALIDATION
                else if (src[end] < ' ')
                {
                    return JsonErrno.BAD_STRING;
                }
#endif
                end++;
            }
            doubleOrString.length = end - refPos;
            refPos = end + 1;
            this.Tag = JsonTag.JSON_STRING;
            return JsonErrno.OK;
        }
        public double ToNumber()
        {
            return doubleOrString.number;
        }
        public JsonNode ToNode()
        {
            return node;
        }
    }
}
