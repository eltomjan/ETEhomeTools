namespace XorPack
{
    partial class CharRow
    {
        protected byte[] row; // row data
        protected uint nonZero; // non-zero bytes bits

        public CharRow(int width)
        {
            Row = new byte[width];
        }

        public CharRow(byte[] data)
        {
            row = data;
            UpdateNonZeros();
        }

        public uint NonZeros()
        {
            return nonZero;
        }
    }
}
