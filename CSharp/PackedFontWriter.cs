using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XorPack
{
    class PackedFontWriter
    {
        CharBox[] chars;
        byte[] rowBM;
        ArrayPtr collBM;
        byte[] data;

        public PackedFontWriter(CharBox[] chars)
        {
            this.chars = chars;
            packData();
        }

        public void packData() {
            int byteSize = chars.Length * charHeight(0);
            collBM = new ArrayPtr(byteSize * chars[0].box[0].Row.Length / 8);
            data = new byte[collBM.Capacity() * 8];
            int dataPos = 0;
            if ((byteSize % 8) > 0) byteSize += 8;
            rowBM = new byte[byteSize >> 3];
            int rowBmPos = 0;
            byte mask = 128;
            foreach (var oneChar in chars)
            {
                foreach (var row in oneChar.box)
                {
                    if(row.NonZeros() > 0) {
                        rowBM[rowBmPos] |= mask;
                        foreach (var b in row.Row)
                        {
                            collBM.addBit(b);
                            if (b > 0) data[dataPos++] = b;
                        }
                    }
                    mask >>= 1;
                    if (mask == 0)
                    {
                        mask = 128;
                        rowBmPos++;
                    }
                }
            }
            System.Array.Resize(ref data, dataPos);
            collBM.shrink2pos();
        }

        public string getFormatedData()
        {
            string result;
            int hl = collBM.Capacity();
            int charsNo = chars.Length;
            int withB = chars[0].box[0].Row.Length;
            int height = chars[0].box.Length;
            result = String.Format("0x{0:X2},0x{1:X2}, {2}, {3}, {4},\r\n// RowBMP\r\n",
                hl >> 8, hl & 255, charsNo, withB, height);
            foreach (var rb in rowBM)
            {
                result += String.Format("0x{0:X2},", rb);
            }
            result += "\r\n//CollBMP\r\n" + collBM.getHexBytes();
            result += "\r\n//Non-zero font data\r\n";
            foreach (var rb in data)
            {
                result += String.Format("0x{0:X2},", rb);
            }
            result = result.Substring(0, result.Length - 1) + "\r\n"; // cut last comma
            return result;
        }

        public int charHeight(int pos)
        {
            return chars[pos].box.Length;
        }
    }
}
