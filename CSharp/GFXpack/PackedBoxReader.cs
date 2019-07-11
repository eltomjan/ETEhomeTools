using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XorPack
{
    class PackedBoxReader
    {
        private byte[] font;
        private byte chars;
        private int rowBMsize;
        private int collBMstart, collBMsize;
        private int dataStart;
        private byte withB, height;

        public int CharNo { get { return chars; } }
        public int Height { get { return height; } }

        public PackedBoxReader(byte[] font)
        {
            this.font = font;
            collBMsize = (font[0] << 8) + font[1];
            chars = font[2]; // # of chars in this font
            withB = font[3]; // char with in Bytes
            height = font[4]; // char height
            rowBMsize = height * chars; // all chars rows bit array
            if ((rowBMsize % 8) > 0) rowBMsize = 1 + (rowBMsize >> 3);
            else rowBMsize = (rowBMsize >> 3);
            collBMstart = 5 + rowBMsize; // array of saved (non-zero) bytes in a non-zero column
            dataStart = collBMstart + collBMsize; // only non-zero bytes (reduced by XOR) operation
        }
        public CharBox readChar(int pos) {
            CharBox box = new CharBox(withB, height); // empty char object
            BitArrayReader rowBM = new BitArrayReader(font, 5);
            BitArrayReader collBM = new BitArrayReader(font, collBMstart);
            int collBMpos = 0;
            int dataPos = dataStart;

            byte? bitRow = rowBM.getBit(0);
            byte? bitColl = collBM.getBit(collBMpos);

            SkipPreviousCharacters(pos, rowBM, collBM, ref dataPos, ref bitRow, ref bitColl);
            
            FillCharData(box, rowBM, collBM, ref dataPos, ref bitRow, ref bitColl);
            
            box.UnXor(); // unpack char data

            return box;
        }

        public CharBox[] readAllChars()
        {
            CharBox[] all = new CharBox[chars];
            BitArrayReader rowBM = new BitArrayReader(font, 5);
            BitArrayReader collBM = new BitArrayReader(font, collBMstart);
            int collBMpos = 0;
            int dataPos = dataStart;

            byte? bitRow = rowBM.getBit(0);
            byte? bitColl = collBM.getBit(collBMpos);

            for (int i = 0; i < chars; i++)
            {
                all[i] = new CharBox(withB, height); // empty char object
                FillCharData(all[i], rowBM, collBM, ref dataPos, ref bitRow, ref bitColl);
                all[i].UnXor(); // unpack char data
            }
            return all;
        }

        private void SkipPreviousCharacters(int pos, BitArrayReader rowBM, BitArrayReader collBM, ref int dataPos, ref byte? bitRow, ref byte? bitColl)
        {
            for (int i = 0; i < pos; i++) // must iterate all previous character and skip their non-zero column bytes
            {
                for (int r = 0; r < height; r++)
                {
                    if (bitRow > 0) // row 1 means there are non-zero data in some Coll(s)
                    {
                        for (int c = 0; c < withB; c++)
                        {
                            if (bitColl > 0)
                            { // non-zero byte was saved
                                //Console.WriteLine("Skipping @{0} {1}", dataPos, font[dataPos]);
                                dataPos++;
                            }
                            bitColl = collBM.getNextBit();
                        }
                    }
                    bitRow = rowBM.getNextBit();
                }
            }
        }

        private void FillCharData(CharBox box, BitArrayReader rowBM, BitArrayReader collBM, ref int dataPos, ref byte? bitRow, ref byte? bitColl)
        {
            foreach (var row in box.box)
            {
                if (bitRow > 0)
                {
                    for (int c = 0; c < withB; c++)
                    {
                        if (bitColl > 0)
                        { // non-zero byte was saved
                            row.Row[c] = font[dataPos];
                            //Console.WriteLine("Reading @{0} {1}", dataPos, font[dataPos]);
                            dataPos++;
                        }
                        bitColl = collBM.getNextBit();
                    }
                }
                bitRow = rowBM.getNextBit();
            }
        }
    }
}
