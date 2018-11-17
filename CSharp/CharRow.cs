using System;

namespace XorPack
{
    partial class CharRow
    {
        protected byte[] row; // row data
        protected uint nonZero; // non-zero bytes bits
        public int Width { get; set; }

        public CharRow(int width)
        {
            Row = new byte[width];
            Width = width * 8;
        }

        public CharRow(byte[] data)
        {
            row = data;
            Width = (row.Length * 8);
            UpdateNonZeros();
        }

        public uint NonZeros()
        {
            return nonZero;
        }

        public void setBit(int x) {
            row[x/8] |= (byte)(128 >> (x & 7));
            nonZero |= (1u << (row.Length - 1 - (x/8))); // UpdateNonZeros();
        }

        public void resetBit(int x) {
            row[x/8] &= (byte)(255 - (128 >> (x & 7)));
            if(row[x/8] == 0) {
                nonZero &= (1u << (row.Length - 1 - (x/8))); // UpdateNonZeros();
            }
        }

        public bool getBit(int x) {
            return (row[x / 8] & (128 >> (x & 7))) > 0;
        }

        public String getStream() {
            string retVal = "";

            foreach(Byte i in row) {
                retVal += Convert.ToString(i, 2).PadLeft(8, '0');
            }

            return retVal.Substring(0, Width);
        }
    }
}
