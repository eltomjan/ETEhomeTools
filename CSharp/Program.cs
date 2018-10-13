using System;
using System.Linq;
using System.Windows.Forms;

namespace XorPack
{
    static class Program
    {
        /// <summary>
        /// Font packer C# translation demo / tester v0
        /// </summary>
        [STAThread]
        static void Main()
        {
            CharBox b = new CharBox(StaticData.b24x36); // packed ! as byte array
            int size;
            size = b.bitSize(); // 113
            b.UnXor();
            size = b.bitSize(); // 278
            b = new CharBox(StaticData.raw36x40, '*'); // Star of David as string array
            size = b.bitSize(); // 750
            b.Xor();
            size = b.bitSize(); // 705
            var tb = new CharBox(StaticData.raw36x40txt, '*'); // Star of David as single string
            tb.Xor();
            tb.UnXor();
            PackedBoxReader font = new PackedBoxReader(StaticData.fontMedium2); // packed font
            var chars = font.readAllChars();
            string printOut = "";
            int normalSize = 0, xoredSize = 0; // 42143 / 24153
            for (int i = 0; i < font.CharNo; i++)
            {
                string[] charOut = new string[font.Height];
                normalSize += chars[i].bitSize();
                for (int j = 0; j < font.Height; j++)
                {
                    var c = chars[i].box[j];
                    charOut[j] = c.ToString() + '│';
                }
                chars[i].Xor();
                xoredSize += chars[i].bitSize();
                for (int j = 0; j < font.Height; j++)
                {
                    var c = chars[i].box[j];
                    printOut += charOut[j] + c.ToString() + Environment.NewLine;
                }
                printOut += new string('─', chars[i].box[0].Row.Count() * 8) +
                            "┼" +
                            new string('─', chars[i].box[0].Row.Count()) +
                            "┼" +
                            new string('─', chars[i].box[0].Row.Count() * 8) +
                            "┼" +
                            new string('─', chars[i].box[0].Row.Count()) +
                            "\r\n";
            }

            PackedFontWriter pf = new PackedFontWriter(chars);
            string fontPrint = pf.getFormatedData(); /*
                header	5
                rowBM	423
                collBM	479
                data	2118 => 3025B => 24200b
            */
            fontPrint += "/*\r\n" + printOut + "*/\r\n";

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
