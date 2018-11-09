namespace XorPack
{
    partial class CharRow
    {
        public byte[] Row { get { return row; }
                            set { row = value; UpdateNonZeros(); } }

        /// <summary>
        /// Collect all non-zero (columns) bytes in row as bit array
        /// </summary>
        /// <returns></returns>
        public uint UpdateNonZeros()
        {
            nonZero = 0;
            foreach (var b in row)
            {
                nonZero <<= 1;
                if (b > 0)
                    nonZero++;
            }
            return nonZero;
        }

        /// <summary>
        /// Debug info
        /// </summary>
        /// <returns>
        /// Content converted to ASCII 
        /// </returns>
        public override string ToString()
        {
            if (row == null)
                return "No Data !";
            string val = "";
            int size = row.Length;
            size--;
            int xPos = 0;
            for (int i = 0; i <= size; i++) // raw picture
            {
                for (int j = 7; j >= 0; j--)
                {
// ─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬▀▄█▌▐░▒▓
                    if ((row[i] & (1 << j)) != 0) val += "█";
                    else val += "░";
                    xPos++;
                    if(xPos >= Width)
                        break;
                }
            }

            val += "│";

            foreach (var b in row) // non-zero columns bits
            {
                if (b > 0) val += "█";
                else val += "▒";
            }
            return val;
        }

        public int getNZCount()
        {
            int nZCount = 0;
            foreach (var b in row)
            {
                if (b > 0)
                    nZCount++;
            }
            return nZCount;
        }
    }
}
