using System;
using System.Globalization;
using System.IO;
using XorPack;

namespace CSharpWithVSCode.ConsoleApp
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            if(args.Length == 0)
                args = new string[] {"FreeSerifBold24pt7b.h"};
            
            Byte[] bmp;
            Int32[] descr;
            int chars;
            using (FileStream fs = new FileStream(args[0], FileMode.Open, FileAccess.Read)) {
                using (StreamReader sr = new StreamReader(fs)) {
                    string file;
                    file = sr.ReadToEnd();
                    int startPos, endPos;
                    startPos = file.IndexOf("{");
                    endPos = file.IndexOf("}", startPos);
                    string bytes = file.Substring(startPos + 1, endPos - startPos - 1);
                    file = file.Substring(endPos+1);
                    bmp = readHexs(bytes);
                    startPos = file.IndexOf("{");
                    bytes = file.Substring(startPos + 1);
                    descr = readInts(bytes);
                    chars = descr.Length / 6;
                    sr.Close();
                }
                fs.Close();
            }
            string charData, font = "";
            int bitSize, fontBitSize = 0, fixedSize = 0, dynamicSize = 0, GFXsize = 0;//, GFXsquareSize = 0;
            int maxW = 0, maxH = 0, mixedDifference = 0;
            for(int i=0;i<chars-1;i++) {
                if(descr[(i * 6) + 1] > maxW) maxW = descr[(i * 6) + 1];
                if(descr[(i * 6) + 2] > maxH) maxH = descr[(i * 6) + 2];
            }
            //int zeros = 0, zerosHalf = 0;
            for(int i=0;i<chars-1;i++) {
                CharBox b = new CharBox(bmp, descr, i);
                string[] dataRows = GFX2ASCII(bmp, descr, i);
                if(dataRows == null) bitSize = 0; else bitSize = string.Join("", dataRows).Length;
                if((bitSize & 7) > 0) bitSize += 8-(bitSize & 7);
                fontBitSize += bitSize;
                //bitSize = b.getFixedBitSize(maxW, maxH);
                //bitSize = b.getGFXbitSize();
                b.Xor();
                // if(b.box != null) {
                //     zeros += b.countZeros(b.getGFXmap()); // 4111
                //     zerosHalf += b.countHalfByte(); // 5171
                // }
                string[] dataRowsX = b.getASCII();
                bitSize = b.getFixedBitSize(-1, -1);
                if(b.box != null) {
                    int commonSize = bitSize;
                    font += b.box[0].Width + "x" + b.box.Length + " byte width size:" + bitSize;
                    dynamicSize += bitSize;
                    bitSize = b.getFixedBitSize(maxW, maxH);
                    font += " fixed size:" + bitSize;
                    fixedSize += bitSize;
                    bitSize = b.getGFXbitSize();
                    font += " GFX size:" + bitSize;
                    GFXsize += bitSize;
                    if(bitSize - commonSize > 0)
                        mixedDifference += bitSize - commonSize;
                // if(b.setSquareMode()) {
                //     int bitSizeSq = b.getGFXbitSize();
                //     if(bitSizeSq < bitSize) {
                //         bitSize = bitSizeSq;
                //         font += " GFX square size:" + bitSize;
                //         descr[i*6] = 0; // reset char pos
                //         CharBox bSquare = new CharBox(b.getGFXmap(), descr, i);
                //         string[] dataRowsXsq = bSquare.getASCII();
                //         for(int y=0;y<dataRowsX.Length;y++) dataRowsX[y] += "||" + dataRowsXsq[y];
                //     }
                // }
                    charData = "";
                    if(dataRows != null) for(int idx=0;idx<dataRows.Length;idx++) {
                        charData += dataRows[idx] + "|" + dataRowsX[idx] + Environment.NewLine;
                    }
                    //GFXsquareSize += bitSize;
                    font += Environment.NewLine + charData + Environment.NewLine;
                }
            }
            font += "Chars: " + chars + " bits: " + fontBitSize + Environment.NewLine +
            "Max X * Y: " + maxW + "x" + maxH + " bits " + maxW * maxH * chars + " " + Perc(fontBitSize, maxW * maxH * chars) + "% of fixed" + Environment.NewLine +
            "Byte width xored data size:" + dynamicSize + " fixed(" + maxW + "," + maxH + "):" + fixedSize + " GFX size: " + GFXsize +
            //" GFX square size: " + GFXsquareSize + 
            " of " + bmp.Length + " => "
            + Perc(dynamicSize, bmp.Length) + "% " + Perc(fixedSize, bmp.Length) + "% " + Perc(GFXsize, bmp.Length) + "% "
            //+ Perc(GFXsquareSize, bmp.Length) + "%"
            ;
            using(FileStream fs = new FileStream(args[0].Substring(0,args[0].Length-4)+".txt", FileMode.Create, FileAccess.Write)) {
                using(StreamWriter sw = new StreamWriter(fs)) {
                    sw.Write(font);
                }
                fs.Close();
            }
        }

        public static double Perc(long x, long y) {
            return ((long)100000 * x / y + 5) / 10 / 100.0;
        }
/*        typedef struct { // Data stored PER GLYPH
            uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
            uint8_t  width, height;    // Bitmap dimensions in pixels
            uint8_t  xAdvance;         // Distance to advance cursor (x axis)
            int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
        } GFXglyph;

        typedef struct { // Data stored for FONT AS A WHOLE:
            uint8_t  *bitmap;      // Glyph bitmaps, concatenated
            GFXglyph *glyph;       // Glyph array
            uint8_t   first, last; // ASCII extents
            uint8_t   yAdvance;    // Newline distance (y axis)
        } GFXfont;*/

        public static string[] GFX2ASCII(Byte[] data, Int32[] descr, int idx)
        {
            string raw = "";
            int bmpIdx = descr[(idx * 6)],
                width = descr[(idx * 6) + 1],
                height = descr[(idx * 6) + 2];
            Byte mask = 128;
            for(int y=0;y<height;y++) {
                for(int x=0;x<width;x++) {
                    if((data[bmpIdx] & mask) != 0) {
                        raw += '*';
                    } else {
                        raw += ' ';
                    }
                    if(mask > 1) {
                        mask >>= 1;
                    } else {
                        mask = 128;
                        bmpIdx++;
                    }
                }
                raw += '\n';
            }
            if(raw.Length == 0) return null;
            return raw.Split('\n');
        }
        private static Byte[] readHexs(string hexs)
        {
            Byte[] result = new Byte[hexs.Length / 5];
            int startPos = 0, endPos = 0, idx = 0;
            while(endPos >= 0) {
                startPos = hexs.IndexOf("x", startPos) + 1;
                endPos = hexs.IndexOf(",", startPos);
                if(endPos < 0) break;
                result[idx++] = Byte.Parse(hexs.Substring(startPos, endPos - startPos), NumberStyles.HexNumber);
            }
            Byte[] resultSmall = new Byte[idx];
            Array.Copy(result, resultSmall, idx);
            return resultSmall;
        }
        private static Int32[] readInts(string data)
        {
            Int32[] result = new Int32[data.Length / 5];
            int startPos = 0, endPos = 0, bracketPos = 0, idx = 0, semiPos;
            startPos = data.IndexOf("{", startPos) + 1;
            do {
                bracketPos = data.IndexOf("}", startPos);
                while(endPos >= 0 && endPos != bracketPos) {
                    endPos = data.IndexOf(",", startPos);
                    if(endPos > 0 && bracketPos < endPos) {
                        endPos = bracketPos;
                    }
                    //Console.WriteLine(data.Substring(startPos, endPos - startPos) + "..." + data.Substring(endPos,10));
                    result[idx++] = Int32.Parse(data.Substring(startPos, endPos - startPos), NumberStyles.Integer);
                    startPos = endPos + 1;
                }
                startPos = endPos;
                int commentPos = data.IndexOf("//", startPos),
                    nlPos = data.IndexOf("\n", startPos);
                semiPos = data.IndexOf(";", startPos);
                if(commentPos > 0 && commentPos < nlPos) {
                    startPos = data.IndexOf("{", nlPos) + 1;
                } else {
                    startPos = data.IndexOf("{", startPos) + 1;
                }
                if(semiPos > commentPos && semiPos < nlPos) {
                    semiPos = startPos + 1;
                }
            } while (startPos > 0 && startPos < semiPos);
            Int32[] resultSmall = new Int32[idx];
            Array.Copy(result, resultSmall, idx);
            return resultSmall;
        }
    }
}
