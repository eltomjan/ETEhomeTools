using System;

namespace FontReader
{
    class Storage
    {
        public static string readFont(byte []font)
        {
            string data = "";
            for(byte i=0;i<font[2];i++) {
                data += readChar((byte)(i + 33), font);
            }
            return data;
        }
        public static string readChar(byte c, byte[] font)
        {
            byte size = 1;
            string data = "";
	        c-=(byte)'!';
            bool m_proportional = true;
            int cursor_x = 0;

            byte[] begin = font;
	        byte chars = begin[2], width = begin[3], height = begin[4];
	        if(c==255) {
		        //fillRect(x, y, width*8, height, bg);
		        if(m_proportional) cursor_x += width*8*5/8;
		        return data;
	        }
	        int rowTabIdx = 5;
	        int rowCollTabIdx = 5 + ((chars*height+7)>>3); // 423 pro 94*36/8
            int charDataTabIdx = rowCollTabIdx + begin[1] + ((begin[0]) << 8);
	        int skipRows = c*height;
	        byte byteRows, byteColls = begin[rowCollTabIdx];
	        rowCollTabIdx++;
	        byte rowTabMask;
	        byte rowCollTabMask = 128;
	        rowTabMask = 128;
	        byteRows = begin[rowTabIdx++];
	        while(skipRows-- > 0) {
		        if((byteRows & rowTabMask) > 0) {
			        for(int i=0;i<width;i++) {
				        if((byteColls & rowCollTabMask) > 0) charDataTabIdx++;
				        rowCollTabMask >>= 1;
				        if(rowCollTabMask == 0) {
					        rowCollTabMask = 128;
					        byteColls =  begin[rowCollTabIdx];
					        rowCollTabIdx++;
				        }
			        }
		        }
		        rowTabMask >>= 1;
		        if(rowTabMask == 0) {
			        rowTabMask = 128;
			        byteRows = begin[rowTabIdx];
			        rowTabIdx++;
		        }
	        }
	        byte[] dataBuf = new byte[height*width];
	        for(byte j=0;j<height;j++) { // read data
		        if((byteRows & rowTabMask) != 0) {
			        for(byte i=0;i<width;i++) {
				        if((byteColls & rowCollTabMask) != 0) { dataBuf[j*width+i] = begin[charDataTabIdx]; charDataTabIdx++; }
				        rowCollTabMask >>= 1;
				        if(rowCollTabMask == 0) {
					        rowCollTabMask = 128;
					        byteColls = begin[rowCollTabIdx];
					        rowCollTabIdx++;
				        }
			        }
		        }
		        rowTabMask >>= 1;
		        if(rowTabMask == 0) {
			        rowTabMask = 128;
			        byteRows = begin[rowTabIdx];
			        rowTabIdx++;
		        }
	        }
            string chr0, chr1;
            byte b;
	        for(byte j=0;j<height;j++) { // unxor data
                chr0 = ""; chr1 = "";
		        b=0; // optional row XOR >> 1 decoding
		        for(byte i=0;i<width;i++) {
			        if((b&1) != 0) b = 128; else b = 0;
			        b ^= dataBuf[j*width+i];
			        // 7 76 65 54 43 32 21 10 ... 07
			        //   7  76 65 54 43 32 21 ... 07
			        // 7  6 75 64 53 42 31 20
			        b ^= (byte)(b >> 1);
			        // 7  6 75 64 53 42 31 20
			        //      7  6  75 64 53 42
			        // 7  6  5  4 73 62 51 40
                    b ^= (byte)(b >> 2);
			        // 7  6  5  4 73 62 51 40
			        //            7  6  5  4
			        // 7  6  5  4  3  2  1  0 ...  0^07=7
                    b ^= (byte)(b >> 4);
			        dataBuf[j*width+i] = b;
		        } // optional row XOR >> 1 decoding
		        for(byte i=0;i<width;i++) {
			        int mask=128, dato=dataBuf[j*width+i];
			        while(mask != 0) {
        				chr0 += (dato & mask) != 0?'X':' ';
                        mask >>= 1;
			        }
                    if (j != 0) dataBuf[j * width + i] ^= dataBuf[(j - 1) * width + i];
			        mask=128;
			        dato=dataBuf[j*width+i];
			        while(mask != 0) {
                        chr1 += (dato & mask) != 0 ? 'X' : ' ';
				        mask >>= 1;
			        }
		        }
                for (byte i = 0; i < width; i++)
                {
                    data += String.Format("0x{0:X2}",dataBuf[j * width + i]) + ",";
                }
                data += " // " + chr1 + " || " + chr0 + Environment.NewLine;
            } // unxor data
        //	int sums[,result;
	        if(size == 2) {
        /*		for(char j=0;j<height-1;j+=2) { // unxor data
			        for(char i=0;i<width;i++) {
				        sum = 0,result = 0;
				        for(int t=0;t<8;t++) {
					        dataBuf[j*width+i]
				        }
			        }
		        }
        */	}
	        int skipLeft = 0, realWidth = width*8-1;
	        if(m_proportional) {
		        byte[] rowOr = new byte[width];
		        for(byte j=0;j<height-1;j++) { // or data
			        for(byte i=0;i<width;i++) {
				        rowOr[i] |= dataBuf[j*3+i];
			        }
		        }
		        while((skipLeft < realWidth) && 0 == (rowOr[(skipLeft>>3)] & (128>>(skipLeft&7))))  skipLeft++;
		        while(0 == (rowOr[(realWidth>>3)] & (128>>(realWidth&7))))  realWidth--;
		        cursor_x += realWidth - skipLeft + 4;

		        byte rowPtr = 0;
		        skipLeft--;
		        /*if(skipLeft > 0) for(byte j=0;j<height;j++) { // or data
			        for(byte i=0;i<width-((j==height-1)?1:0);i++) {
				        dataBuf[rowPtr++] <<= skipLeft;
                        dataBuf[rowPtr - 1] |= (byte)(dataBuf[rowPtr] >> (8 - skipLeft));
			        }
		        }*/
	        }
            if (skipLeft > 0)
                return String.Format("// {0} skipLeft:{1} realWidth:{2}\r\n{3}", (char)(c + '!'), skipLeft, (realWidth - skipLeft), data);
            else
                return String.Format("// {0} realWidth:{1}\r\n{2}", (char)(c + '!'), realWidth, data);
        }

    }
}
