using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XorPack
{
    partial class CharBox
    {
        // Empty
        public CharBox(byte x, byte y)
        {
            box = new CharRow[y];
            for (int i = 0; i < y; i++)
            {
                box[i] = new CharRow(x);
            }
        }

        // From byte array
        public CharBox(byte[][] data)
        {
            int height = data.Length;
            box = new CharRow[height];
            for (int i = 0; i < height; i++)
            {
                box[i] = new CharRow(data[i]);
            }
        }
        public CharBox(string[] data, char one)
        {
            if (data.Length == 1)
                data = data[0].Replace("\r","").Split('\n');
            int height = data.Length;
            box = new CharRow[height];
            for (int i = 0; i < height; i++)
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
    }
}
