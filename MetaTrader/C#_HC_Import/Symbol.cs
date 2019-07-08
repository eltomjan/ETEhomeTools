using System;
using System.IO;

namespace HcImport
{
    class Symbol
    {
        public Header header;
        public Row[] data;
        public Row extremes;
        public int dataPos;

        public Symbol(BinaryReader br, String path)
        {
            dataPos = 0;
            header = new Header(br);
            header.symbol = path;
            for (int j = 0; j < 7; j++)
            {
                Int32 rows = br.ReadInt32();
                if (j == 0)
                {
                    header.rows = rows;
                    data = new Row[rows];
                    for (int i = 0; i < rows; i++) data[i] = new Row();

                }
                for (int i = 0; i < header.rows; i++)
                {
                    data[i].Read(br, j, ref extremes);
                }
                
            }
        }

        static public DateTime time_t2date(UInt64 time_t)
        {
            // https://www.codeproject.com/Tips/694874/Exchange-Date-Time-between-Native-Cplusplus-and-Cs
            // Thursday, January 01, 1970 12:00:00 AM
            long res = (long)(time_t * 10000000 + 0x089f7ff5f7b58000);
            return new DateTime(res);
        }
    }
}
