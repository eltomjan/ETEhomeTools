using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XorPack
{
    partial class CharBox
    {
        private int m_height;
        private byte m_step;

        // Empty
        public CharBox(byte x, byte y)
        {
            m_step = 1;
            m_height = y;
            box = new CharRow[y];
            for (int i = 0; i < y; i++)
            {
                box[i] = new CharRow(x);
                box[i].Width = x;
            }
        }
        // From byte array
        public CharBox(byte[][] data)
        {
            m_step = 1;
            m_height = data.Length;
            box = new CharRow[m_height];
            for (int i = 0; i < m_height; i++)
            {
                box[i] = new CharRow(data[i]);
            }
        }

        // From GFX
        public CharBox(byte[] data, Int32[] descr, int pos)
        {
            m_step = 1;
            int width = descr[(pos * 6) + 1];
            m_height = descr[(pos * 6) + 2];
            if(m_height + width == 0) return;
            pos = descr[(pos * 6) + 0];
            int startPos = pos;
            box = new CharRow[m_height];
            Byte mask = 128;
            for(int y=0;y<m_height;y++) {
                box[y] = new CharRow(width/8 + Convert.ToByte((width&7) > 0));
                box[y].Width = width;
                for(int x=0;x<width;x++) {
                    if((data[pos] & mask) != 0) {
                        box[y].setBit(x);
                    }
                    if(mask > 1) {
                        mask >>= 1;
                    } else {
                        mask = 128;
                        pos++;
                    }
                }
                box[y].NonZeros();
            }
            Byte[] check = getGFXmap();
            for(int i=0;i<check.Length;i++) {
                if(check[i] != data[startPos+i]) {
                    Console.WriteLine("Difference " + check[i] + data[startPos+i]);
                }
            }
        }

        public CharBox(string[] data, char one)
        {
            m_step = 1;
            if (data.Length == 1)
                data = data[0].Replace("\r","").Split('\n');
            m_height = data.Length;
            box = new CharRow[m_height];
            for (int i = 0; i < m_height; i++)
            {
                int width = data[0].Length;
                if (width % 8 > 0) width += 8;
                byte[] row = new byte[width / 8];
                int pos = 0;
                byte mask = 128;
                foreach (var r in data[i])
                {
                    if (r == one) row[pos] |= mask;
                    mask >>= 1;
                    if (mask == 0)
                    {
                        mask = 128;
                        pos++;
                    }
                }
                box[i] = new CharRow(row);
            }
        }

        // Rotate
        public CharBox(CharBox bR)
        {
            m_step = 1;
            m_height = bR.box[0].Width;
            int width = bR.box.Length;
            box = new CharRow[m_height];
            for (int i = 0; i < m_height; i++)
            {
                int bWidth = (width/8);
                if((width&7) > 0) bWidth++;
                box[i] = new CharRow(bWidth);
                for (int j = 0; j < width; j++)
                {
                    if (bR.box[j].getBit(i))
                    {
                        box[i].setBit(j);
                    }
                }
            }
        }

        public int bitSize()
        {
            int size = box.Length;
            foreach (var r in box)
            {
                uint data = r.NonZeros();
                if (data > 0)
                {
                    for (int i = 0; i < r.Row.Length; i++)
                    {
                        if ((data & 1) > 0) size += 9;
                        else size++;
                        data >>= 1;
                    }
                }
            }
            return size;
        }

        public byte[] getGFXmap() {
            return getGFXmap(box[0].Width, m_height);
        }

        public byte[] getGFXmap(int width, int height) {
            if(m_height == 0) return null;
            int maxWidth = box[0].Width;
            int size = height * width;
            size = (size / 8) + Convert.ToByte((size & 7) > 0);
            byte[] retVal = new byte[size];
            int outPos = 0;
            Byte mask = 128;
            int checkedStep = m_step;
            if(((height & 1) != 0) && m_step == 2) checkedStep = 1; // ignore - will miss last even row
            int checkedWidth = width * checkedStep;
            maxWidth *= checkedStep;
            for(int y=0;y<height;y+=checkedStep) {
                for(int x=0;x<checkedWidth;x++) {
                    if((y < m_height) && (x < maxWidth)) {
                        // if(checkedStep == 2) {
                        //     /*
                        //         We want to compare 1/2 bytes between rows as they are more rectangular
                        //         so we double X coordinate number and use lowest bit of X in Y, so we go thru small 2x2 squares
                        //         current, down, up/right and right/down like
                        //         7 5 3 1  instead of 7 6 5 3 4 2 1 0, where is lower probability to find zero (same bytes in following rows)
                        //         6 4 2 0
                        //         in case height is not odd, there would be a special common(?) case, so for simplicity it is ignored
                        //      */
                        //     int mixedY = y + (x & 1);
                        //     if(box[mixedY].getBit(x >> 1)) {
                        //         retVal[outPos] |= mask;
                        //     }
                        //     /*
                        //         We want to compare 1/2 bytes between rows as they are more rectangular
                        //         so we double X coordinate number and use lowest bit of X in Y
                        //         7 6 5 3  instead of 7 6 5 3 4 2 1 0, where is lower probability to find zero (same bytes in following rows)
                        //         4 2 1 0
                        //         in case height is not odd, there would be a special common(?) case, so for simplicity it is ignored
                        //      */
                        //     // int mixedY = y + ((x & 4) >> 2); // every 4 change row
                        //     // int mixedX = ((x >> 3) << 2) + (x & 3); // 0 1 2 3 0 1 2 3 | 4 5 6 7 4 5 6 7
                        //     // if(box[mixedY].getBit(mixedX)) {
                        //     //     retVal[outPos] |= mask;
                        //     // }
                        // } else
                        { // common old case
                            if(box[y].getBit(x)) {
                                retVal[outPos] |= mask;
                            }
                        }
                    }
                    if(mask > 1) {
                        mask >>= 1;
                    } else {
                        mask = 128;
                        outPos++;
                    }
                }
            }
            if(size - outPos > 1) {
                Console.WriteLine("Error in size ?");
            }
            return retVal;
        }

        public byte[] getGFXmapR(int width, int height)
        {
            if (m_height == 0) return null;
            int maxWidth = box[0].Width;
            int size = height * width;
            size = (size / 8) + Convert.ToByte((size & 7) > 0);
            byte[] retVal = new byte[size];
            int outPos = 0;
            Byte mask = 128;
            int checkedStep = m_step;
            if (((height & 1) != 0) && m_step == 2) checkedStep = 1; // ignore - will miss last even row
            int checkedWidth = width * checkedStep;
            maxWidth *= checkedStep;
            for (int y = 0; y < checkedWidth; y += checkedStep)
            {
                for (int x = 0; x < height; x++)
                {
                    if ((y < maxWidth) && (x < m_height))
                    {
                        if (box[x].getBit(y))
                        {
                            retVal[outPos] |= mask;
                        }
                    }
                    if (mask > 1)
                    {
                        mask >>= 1;
                    }
                    else
                    {
                        mask = 128;
                        outPos++;
                    }
                }
            }
            if (size - outPos > 1)
            {
                Console.WriteLine("Error in size ?");
            }
            return retVal;
        }

        /// <summary>
        /// Debug info
        /// </summary>
        /// <returns>
        /// Content converted to ASCII 
        /// </returns>
        public string[] getASCII()
        {
            if (box == null)
                return null;
            string val = "";
            int size = box.Length;
            for (int i = 0; i < size; i++) // raw picture
            {
                val += box[i].ToString();
                val += '\n';
            }
            return val.Split('\n');
        }

        public int getDynamicBitSize(bool reverse)
        {
            if(box == null)
                return 0;
            return getFixedBitSize(box[0].Width, m_height, reverse);
        }
        public int getFixedBitSize(int w, int h, bool reverse)
        {
            int retVal = 0;
            if(w + h == -2) {
                h = m_height;
                if(h == 0) return 0;
                int byteSize = box[0].Row.Length * h;
                for(int y=0; y<box.Length; y++) {
                    retVal += box[y].getNZCount();
                }
                retVal += byteSize / 8;
                if((byteSize & 7) > 0)
                    retVal++;
                return retVal;
            } else {
                if(box == null) return 0;
                Byte[] bitArray;
                if (reverse) bitArray = getGFXmapR(w, h); else bitArray = getGFXmap(w, h);
                /*String bitArray2 = encode(bitArray);
                String bitArray3 = decode(bitArray2);
                int size = bitArray3.Length;
                if ((size & 7) > 0) return (size / 8) + 1; else return (size / 8);
                */
                int size = bitArray.Length;
                int c=0;
                for(int i=0;i<size;i++) {
                    if((i > 0) && ((i & 7) == 0)) {
                        retVal++;
                        c=0;
                    }
                    if(bitArray[i] > 0) {
                        retVal++;
                    }
                    c++;
                }
                if((size & 7) > 0) retVal++;
                return retVal;
            }
            throw new NotImplementedException();
        }

        public int getGFXbitSize() {
            int retVal = 0;
            if(box == null) return 0;
            byte[] GFXdata = getGFXmap();
            int count = 0;
            if(GFXdata == null) return 0;
            foreach(byte i in GFXdata) {
                if(count > 7) {
                    retVal++;
                    count = 0;
                }
                count++;
                if(i > 0) {
                    retVal++;
                }
            }
            if((count & 7) > 0) {
                retVal++;
            }
            return retVal;
        }
        
        // public int countZeros(Byte[] bitArray)
        // {
        //     int count = 0;
        //     foreach(Byte i in bitArray) if(i == 0) count++;
        //     return count;
        // }

        // public int countHalfByte()
        // {
        //     int zeros = 0;
        //     int oddLen = box.Length;
        //     oddLen = (oddLen | 1) - 1;
        //     for(int y=0;y<oddLen;y+=2)
        //     {
        //         for(int x=0;x<box[y].Row.Length*2;x++) {
        //             int b1_2 = box[y].Row[x/2];
        //             int b2_2 = box[y+1].Row[x/2];
        //             if((x & 1) == 0) {
        //                 if(((b1_2 | b2_2) & 0xF0) == 0) {
        //                     zeros++;
        //                 }
        //             } else {
        //                 if(((b1_2 | b2_2) & 0x0F) == 0) {
        //                     zeros++;
        //                 }
        //             }
        //         }
        //     }
        //     return zeros;
        // }

        public bool setSquareMode() {
            if(m_height == 0) return false;
            m_step = 2;
            return ((m_height & 1) == 0);
        }
    }
}
