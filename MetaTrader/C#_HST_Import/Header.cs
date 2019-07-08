using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace HstImport
{
    class Header
    {
        public Int32 version; 	// database version - 401	4 bytes
        public String copyright;	// copyright info	64 bytes
        public String symbol;	// symbol name	12 bytes
        public Int32 period; 	// symbol timeframe	4 bytes
        public Int32 digits;	// the amount of digits after decimal point	4 bytes
        public DateTime timesign; 	// timesign of the database creation	4 bytes
        public DateTime last_sync; 	// the last synchronization time	4 bytes
        public Int32 rows;
        public Int32[] unused;	// to be used in future	52 bytes

        public Header(BinaryReader br)
        {
            version = br.ReadInt32();
            copyright = System.Text.Encoding.ASCII.GetString(br.ReadBytes(64)).TrimEnd((Char)0);
            symbol = System.Text.Encoding.ASCII.GetString(br.ReadBytes(12)).TrimEnd((Char)0);
            period = br.ReadInt32();
            digits = br.ReadInt32();
            timesign = Symbol.time_t2date(br.ReadUInt32());
            last_sync = Symbol.time_t2date(br.ReadUInt32());
            rows = br.ReadInt32();
            unused = new Int32[13];
            for (int i = 0; i < 13; i++)
            {
                unused[i] = br.ReadInt32();
            }
        }
    }
}
