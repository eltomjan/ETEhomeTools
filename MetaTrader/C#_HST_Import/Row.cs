using System;
using System.IO;

namespace HstImport
{
    class Row
    {
        public DateTime ctm; // bar start time 8 bytes
        public double open; // open price 8 bytes
        public double high; // highest price 8 bytes
        public double low; // lowest price 8 bytes
        public double close; // close price 8 bytes
        public double volume; // tick count 8 bytes

        static public int Size {
            get
            {
                return 60;
            }
        }

        public void Read(BinaryReader br, ref Row extremes)
        {
            UInt64 tmp = br.ReadUInt64();
            ctm = Symbol.time_t2date(tmp & 0xFFFFFFFF);
            open = br.ReadDouble();
            high = br.ReadDouble();
            low = br.ReadDouble();
            close = br.ReadDouble();
            volume = br.ReadInt32();
            tmp = br.ReadUInt64();
            tmp = br.ReadUInt64();
            if (extremes == null)
            {
                extremes = new Row();
                extremes.open = open;
                extremes.high = high;
                extremes.low = low;
                extremes.volume = volume;
            }
            else
            {
                if(extremes.high < high) extremes.high = high;
                if(extremes.low > low) extremes.low = low;
                if(extremes.volume < volume) extremes.volume = volume;
                extremes.ctm = ctm;
                extremes.close = close;
            }
        }

        public String GetDate()
        {
            return ctm.ToShortDateString() + ' ' + ctm.ToShortTimeString() + '\t';
        }

        public String GetNumbers()
        {
            String retVal = "";
            retVal += open.ToString() + '\t';
            retVal += high.ToString() + '\t';
            retVal += low.ToString() + '\t';
            retVal += close.ToString() + '\t';
            retVal += volume.ToString();
            return retVal;
        }
    }
}
