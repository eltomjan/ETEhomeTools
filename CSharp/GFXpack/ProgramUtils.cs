using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;

namespace XorPack
{
    public static partial class Program
    {
        static List<List<int>> bitCounts = new List<List<int>>();
        static List<string> allChars = new List<string>();
        static int[] blockSizeSums = new int[254];
        static int blockSize;
        static int rounds = 0;
        static List<Int32[]> glyphs;
        static Byte[] bmp;
        static int chars;
        static string packedFont = string.Empty, glyphHead, fontDeclaration;

        private static string EncodeFont()
        {
            StringBuilder font = new StringBuilder();
            blockSize = Array.IndexOf(blockSizeSums, blockSizeSums.Min()) + 2;
            font.Append(blockSize);
            int bitPos = 0, byteSize = 0;
            string bitBmp = string.Empty;
            for (int i = 0; i < allChars.Count; i++)
            {
                string bits = EncodeChar(i);
                bitPos += bits.Length;
                glyphs[i][0] = bitPos;
                bitBmp += bits;
            }
            byteSize = BinaryStringToHexString(ref bitBmp);
            font.Append(bitBmp);
            int averagePos = (8 * ++byteSize) / (glyphs.Count);
            for (int i = 0; i < allChars.Count; i++)
            {
                glyphs[i][0] -= averagePos * i;
            }
            if (glyphs[0][0] == 0)
                glyphs[0][0] = Int16.MinValue; // special mark if (empty) space present

            return " // " + (byteSize + 1) + " of " + bmp.Count() +
                " (" + (1000 * (byteSize + 1) + 5) / bmp.Count() / 10 + "%) " + Environment.NewLine +
                font.ToString();
        }

        private static string EncodeChar(int index)
        {
            int pos = 0;
            int allBits = bitCounts[index].Count;
            if (allChars[index].Length == 0)
                return allChars[index];
            allBits--;
            string encodedData = string.Empty;
            while (pos <= allBits)
            {
                int nextPos = Math.Min(pos + blockSize, allBits);
                if (bitCounts[index][nextPos] - bitCounts[index][pos] > 0)
                {
                    nextPos = Math.Min(pos + blockSize, allChars[index].Length) - pos;
                    encodedData += '1' + allChars[index].Substring(pos, nextPos);
                }
                else
                {
                    encodedData += '0';
                }
                pos += blockSize;
            }
            return encodedData;
        }

        public static void CountBitsAndStats(string asciiMap)
        {
            List<int> charList = new List<int>(asciiMap.Length + 1);
            bitCounts.Add(charList);
            charList.Add(0); // afterBlock - startOfBlock = onesCount
            int sum = 0;

            foreach (var bit in asciiMap)
            {
                sum += bit - '0';
                charList.Add(sum);
            }
            UpdateBlockStats(charList, asciiMap);
        }

        private static void UpdateBlockStats(List<int> charList, string asciiMap)
        {
            for (int blockWidth = 2; blockWidth < blockSizeSums.Length + 2; blockWidth++)
            {
                int pos = 0;
                while (pos < asciiMap.Length)
                {
                    int nextBlock = Math.Min(pos + blockWidth, charList.Count - 1);
                    if (charList[nextBlock] - charList[pos] > 0) blockSizeSums[blockWidth - 2] += blockWidth;
                    blockSizeSums[blockWidth - 2]++; // zero or 1b prefix
                    pos += blockWidth;
                }
            }
        }

        public static int BinaryStringToHexString(ref string binary)
        {
            int bytes = 0;
            if (string.IsNullOrEmpty(binary))
                return 0;

            StringBuilder result = new StringBuilder(binary.Length / 8 + 1);

            // TODO: check all 1's or 0's... throw otherwise

            int mod4Len = binary.Length % 8;
            if (mod4Len != 0)
            {
                // pad to length multiple of 8
                binary = binary.PadRight(((binary.Length / 8) + 1) * 8, '0');
            }

            for (int i = 0; i < binary.Length; i += 8)
            {
                string eightBits = binary.Substring(i, 8);
                result.Append(", ");
                if (bytes % 12 == 0)
                    result.Append(Environment.NewLine);
                result.AppendFormat("0x{0:X2}", Convert.ToByte(eightBits, 2));
                bytes++;
            }

            binary = result.ToString();
            return bytes;
        }

        private static void ReadFontH(string path)
        {
            using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read))
            {
                using (StreamReader sr = new StreamReader(fs))
                {
                    string file;
                    file = sr.ReadToEnd();
                    int startPos, endPos;
                    startPos = file.IndexOf("{");
                    endPos = file.IndexOf("}", startPos);
                    string bytes = file.Substring(startPos + 1, endPos - startPos - 1);
                    int lf = file.LastIndexOf(Environment.NewLine, startPos) + 1;
                    packedFont = file.Substring(lf, startPos - lf + 1);
                    file = file.Substring(endPos + 1);
                    bmp = readHexs(bytes);
                    startPos = file.IndexOf("{");
                    lf = file.LastIndexOf(Environment.NewLine, startPos);
                    glyphHead = file.Substring(lf, startPos - lf + 1);
                    bytes = file.Substring(startPos + 1);
                    endPos = file.LastIndexOf("};") + 2;
                    startPos = file.LastIndexOf("};", endPos - 2);
                    startPos = file.IndexOf(Environment.NewLine, startPos);
                    fontDeclaration = file.Substring(startPos, endPos - startPos);
                    glyphs = readInts(bytes);
                    chars = glyphs.Count;
                    sr.Close();
                }
                fs.Close();
            }
        }

        public static bool TrimEndZeros(ref string asciiMap)
        {
            int lastZero = asciiMap.Length;
            if (lastZero > 0)
            {
                int lastZeroStart = lastZero;
                while (lastZero > 0 && asciiMap[--lastZero] == '0') ;
                if (lastZeroStart != ++lastZero)
                    asciiMap = asciiMap.Substring(0, lastZero);
                return lastZero != lastZeroStart;
            }

            throw new Exception("Bug of logic !");
        }

        private static Byte[] readHexs(string hexs)
        {
            Byte[] result = new Byte[hexs.Length / 5];
            int startPos = 0, endPos = 0, idx = 0;
            while (endPos >= 0)
            {
                startPos = hexs.IndexOf("x", startPos) + 1;
                endPos = hexs.IndexOf(",", startPos);
                if (endPos < 0) break;
                result[idx++] = Byte.Parse(hexs.Substring(startPos, endPos - startPos), NumberStyles.HexNumber);
            }
            Byte[] resultSmall = new Byte[idx];
            Array.Copy(result, resultSmall, idx);
            return resultSmall;
        }

        private static List<Int32[]> readInts(string data)
        {
            List<Int32[]> result = new List<Int32[]>();
            int startPos = 0, endPos = 0, bracketPos = 0, semiPos;
            startPos = data.IndexOf("{", startPos) + 1;
            do
            {
                Int32[] glyph = new Int32[6];
                bracketPos = data.IndexOf("}", startPos);
                int idx = 0;
                while (endPos >= 0 && endPos != bracketPos)
                {
                    endPos = data.IndexOf(",", startPos);
                    if (endPos > 0 && bracketPos < endPos)
                    {
                        endPos = bracketPos;
                    }
                    glyph[idx++] = Int32.Parse(data.Substring(startPos, endPos - startPos), NumberStyles.Integer);
                    startPos = endPos + 1;
                }
                result.Add(glyph);
                startPos = endPos;
                int commentPos = data.IndexOf("//", startPos),
                    nlPos = data.IndexOf("\n", startPos);
                semiPos = data.IndexOf(";", startPos);
                if (commentPos > 0 && commentPos < nlPos)
                {
                    startPos = data.IndexOf("{", nlPos) + 1;
                }
                else
                {
                    startPos = data.IndexOf("{", startPos) + 1;
                }
                if (semiPos > commentPos && semiPos < nlPos)
                {
                    semiPos = startPos + 1;
                }
            } while (startPos > 0 && startPos < semiPos);
            return result;
        }
    }
}
