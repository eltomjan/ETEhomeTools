using System;
using System.IO;

namespace XorPack
{
    public static partial class Program
    {
        public static void Main(string[] args)
        {
            if (args.Length == 0)
                args = new string[] { "Arial10pt.h" }; // Picopixel.h Arial24pt.h

            ReadFontH(args[0]);

            for (int i = 0; i < chars; i++)
            {
                CharBox box = new CharBox(bmp, glyphs[i], i);
                box.Xor();
                string asciiMap = box.getGFXstream(); // full BMP
                allChars.Add(asciiMap); // XORed original
                if (box.box != null) {
                    TrimEndZeros(ref asciiMap);
                }
                int round = asciiMap.Length & 7;
                if (round != 0)
                    rounds += (8 - round);
                CountBitsAndStats(asciiMap);
            }
            // Arial10pt 3: 7640 => 955B <- 1432Z => 66,689%
            // Arial24pt 5: 27956 => 3495B (+4b) <- 7598 => 45,999%
            // Pico 3: 1321  => 166B (+1b) <- 179 => 92,737%
            packedFont += EncodeFont() + Environment.NewLine + "};" + Environment.NewLine +
                glyphHead + Environment.NewLine;

            int pos = 0;
            for (int i = 0; i < chars; i++)
            {
                packedFont += "{" + String.Join(",\t", glyphs[i]) + "}," + Environment.NewLine;
                if (glyphs[i][0] != Int16.MinValue)
                    pos += glyphs[i][0];
            }
            packedFont += "};" + fontDeclaration + Environment.NewLine;
            using (FileStream fs = new FileStream(args[0].Substring(0, args[0].Length - 4) + ".txt", FileMode.Create, FileAccess.Write))
            {
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.Write(packedFont);
                }
                fs.Close();
            }
        }

    }
}
