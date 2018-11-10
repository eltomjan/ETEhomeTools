using System;
using System.Collections.Generic;

namespace XorPack
{
    partial class CharBox
    {
        public CharRow[] box;

        public void UnXor() // Direction Up -> Down
        {
            if(box == null) return;
            int height = box.Length;
            int width = box[0].Row.Length;
            box[0].UpdateNonZeros();
            for (int j = 0; j < height - 1; j++) // 0 -> last-1
            {
                byte[] row0 = box[j].Row;
                byte[] row1 = box[j + 1].Row;
                for (int i = 0; i < width; i++)
                {
                    row1[i] ^= row0[i]; // all columns
                }
                box[j + 1].Row = row1; // xor each row with previous one - first untouched, rest XORed
            }
        }

        public void Xor() // Direction Up <- Down
        {
            if(box == null) return;
            int height = box.Length;
            int width = box[0].Row.Length;
            for (int j = height - 1; j > 0; j--) // last -> 1
            {
                byte[] row0 = box[j - 1].Row;
                byte[] row1 = box[j].Row;
                for (int i = 0; i < width; i++)
                {
                    row1[i] ^= row0[i]; // all columns
                }
                box[j].Row = row1; // last row with previous, etc.
            }
        }

        public String encode(Byte[] bitArray) {
            byte mask = 128;
            Byte[] bitArrayOut = new Byte[bitArray.Length];
            string source = "", dest = "";
            for (int pos = 0; pos < bitArray.Length; )
            {
                if ((bitArray[pos] & mask) > 0)
                {
                    source += '1';
                    dest += '1';
                    mask >>= 1;
                    if (mask == 0)
                    {
                        mask = 128;
                        pos++;
                    }
                    continue;
                }
                else
                {
                    int sum = 0;
                    do
                    {
                        source += '0';
                        mask >>= 1;
                        if (mask == 0)
                        {
                            mask = 128;
                            pos++;
                        }
                        if ((sum == 15) || (pos >= bitArray.Length) || ((bitArray[pos] & mask) != 0))
                        {
                            dest += '0';
                            for (int j = 0; j < 4; j++) // save 0 & 4 bits - # of zeros
                            {
                                if ((sum & 8) == 8)
                                {
                                    dest += '1';
                                }
                                else
                                {
                                    dest += '0';
                                }
                                sum <<= 1;
                            }
                            sum = -1;
                        }
                        sum++;
                    } while ((pos < bitArray.Length) && (bitArray[pos] & mask) == 0 && (sum < 16));
                }
            }
            return dest;
        }

        public String decode(String bitArray)
        {
            String bitArrayOut = "";
            for (int i = 0; i < bitArray.Length; i++)
            {
                if (bitArray[i] == '1')
                {
                    bitArrayOut += '1';
                }
                else
                {
                    int no = 0;
                    for (int j = 0; j < 5; j++)
                    {
                        no <<= 1;
                        if (bitArray[i] == '1') no++;
                        i++;
                    }
                    i--;
                    for (int j = 0; j <= no; j++)
                    {
                        bitArrayOut += '0';
                    }
                }
            }
            return bitArrayOut;
        }
    }
}
