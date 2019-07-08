using System;
using System.IO;

namespace HcImport
{
    class Header
    {
        public Int32 version; 	// database version - 401	4 bytes
        public String copyright;	// copyright info 64 wchar
        public String symbol;	// symbol name 32 bytes
        public DateTime last_sync; 	// the last synchronization time	4 bytes
        public Int32 rows;

        public Header(BinaryReader br)
        {
            version = br.ReadInt32();
            copyright = System.Text.Encoding.ASCII.GetString(br.ReadBytes(64)).TrimEnd((Char)0);
            br.ReadChars(16); // HistoryCache
            last_sync = Symbol.time_t2date(br.ReadUInt32());
            br.ReadChars(256+4);
        }
    }
}
