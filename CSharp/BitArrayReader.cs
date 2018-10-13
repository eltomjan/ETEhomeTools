using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XorPack
{
    class BitArrayReader
    {
        byte[] srcBuf;
        int pos;
        int? posInside;
        byte mask;
        byte? current;

        public BitArrayReader(byte[] buf, int pos)
        {
            srcBuf = buf;
            this.pos = pos;
        }

        public byte getBit(int pos)
        {
            posInside = pos;
            mask = (byte)(1 << (7 - (pos & 7)));
            current = (byte)(srcBuf[this.pos + (int)posInside]);
            return (byte)(current & mask);
        }

        public byte? getNextBit()
        {
            if (current == null)
                return null;
            mask >>= 1;
            if(mask == 0) {
                mask = 128;
                posInside++;
                current = srcBuf[this.pos + (int)posInside];
            }
            return (byte)(current & mask);
        }
    }
}
