using System;
using System.IO;

namespace HcImport
{
    class Row
    {
        public DateTime ctm; // bar start time 8 bytes
        public double open; // open price 8 bytes
        public double high; // highest price 8 bytes
        public double low; // lowest price 8 bytes
        public double close; // close price 8 bytes
        public Int64 volume; // tick count 8 bytes

        static public int Size {
            get
            {
                return 60;
            }
        }

        public void Read(BinaryReader br, int item, ref Row extremes)
        {
            if (item == 0)
            {
                ctm = Symbol.time_t2date(br.ReadUInt64());
            }
            else if (item == 1)
            {
                open = br.ReadDouble();
                extremes.open = open;
            }
            else if (item == 2)
            {
                high = br.ReadDouble();
            }
            else if (item == 3)
            {
                low = br.ReadDouble();
            }
            else if (item == 4)
            {
                close = br.ReadDouble();
            }
            else if (item == 5)
            {
                volume = br.ReadInt64();
            }
            if (extremes == null)
            {
                extremes = new Row();
                extremes.high = Double.MinValue;
                extremes.low = Double.MaxValue;
                extremes.volume = 0;
            }
            else
            {
                if (item == 2 && extremes.high < high) extremes.high = high;
                if (item == 3 && extremes.low > low) extremes.low = low;
                if (item == 5 && extremes.volume < volume) extremes.volume = volume;
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
