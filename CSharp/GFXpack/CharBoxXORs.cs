using System;

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

        public String encode(int charWidth) {
            String retVal = "", map = getGFXstream(), compare = new String('0', charWidth);
            int i;
            for(i=0;i<map.Length;i+=charWidth) {
                String block;
                if(i+charWidth <= map.Length) {
                    block = map.Substring(i, charWidth);
                } else {
                    block = map.Substring(i);
                    compare = compare.Substring(0, block.Length);
                }
                if(block == compare) {
                    retVal += '0';
                } else {
                    retVal += '1' + block;
                }
            }
            return retVal;
        }
    }
}
