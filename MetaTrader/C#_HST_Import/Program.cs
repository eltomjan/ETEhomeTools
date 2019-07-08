using System;
using System.Text;
using System.IO;
using System.Collections.Generic;

namespace HstImport
{ // Compile in cmd (v2 or higher): \Windows\Microsoft.NET\Framework\v2.0.50727\csc.exe *.cs
    class Program
    { // Update 4 files v401 from MT 4 Build 1136 (26 Oct 2018)
        // https://www.mql5.com/en/forum/149178 (format specs)
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                args = new String[] { @"." };
            }
            DirectoryInfo di = new DirectoryInfo(args[0]);
            FileInfo[] fi = di.GetFiles("*.hst");
            int no = fi.Length;
            if (no == 0) return; // No history files found...
            List<Symbol> symbol = new List<Symbol>(); // Symbol histories
            StreamWriter sw = new StreamWriter(args[0] + @"\output.txt"); // CSV intersection output
            StringBuilder sb = new StringBuilder();
            sb.Append("Date\t"); // First row
            foreach (FileInfo f in fi) // Read symbol files
            {
                using (FileStream fs = new FileStream(f.FullName, FileMode.Open, FileAccess.Read))
                {
                    using (BinaryReader br = new BinaryReader(fs))
                    {
                        symbol.Add(new Symbol(br));
                        br.Close();
                    }
                    fs.Close();
                }
            }
            String formulas = "";
            for (int i = 0; i < symbol.Count; i++) // Create average formulas
            {
                sb.Append(symbol[i].header.symbol); // Collect symbols 4 1st row formulas
                sb.Append('\t');
                formulas += "=(";
                for (int j = 0; j < 4; j++)
                {
                    if (j > 0) formulas += '+';
                    int col = (i*6)+ no + 2 + j;
                    if (col >= (26 * 26))
                    {
                        formulas += (char)((col / (26 * 26)) + 'A' - 1);
                        col %= (26 * 26);
                    }
                    if (col >= 26)
                    {
                        formulas += (char)((col / 26) + 'A' - 1);
                        col %= 26;
                    }
                    if(col < 26)
                        formulas += (char)(col + 'A');
                    formulas += '2';
                }
                formulas += ")/$$\t";
            }
            sb.Append('\t');
            foreach (Symbol r in symbol) // Data header - Symbol name and empty columns
            {
                sb.Append(r.header.symbol);
                sb.Append("\t\t\t\t\t\t");
            }
            sb.Remove(sb.Length - 2, 2);
            sb.Append(Environment.NewLine);
            DateTime d = symbol[0].data[0].ctm; // Start with first date
            bool exit = false;
            do
            {
                bool neq;
                do
                {
                    neq = false;
                    foreach (Symbol r in symbol) // Find max if records of "d" date R missing
                    {
                        if (r.data.Length <= r.dataPos)
                        {
                            exit = true;
                            break;
                        }
                        if (d < r.data[r.dataPos].ctm)
                            d = r.data[r.dataPos].ctm;
                    }
                    foreach (Symbol r in symbol) // Skip older records in all files
                    {
                        while (r.dataPos < r.data.Length && d > r.data[r.dataPos].ctm)
                        {
                            r.dataPos++;
                            if (r.dataPos >= r.data.Length)
                                exit = true; // This symbol went out of data...
                        }
                    }
                    if (exit)
                        break;
                    foreach (Symbol r in symbol)
                    {
                        if (d != r.data[r.dataPos].ctm) // Current time is not same in all files
                        {
                            d = r.data[r.dataPos].ctm;
                            neq = true;
                            break;
                        }
                    }
                } while (neq && !exit);
                if (!exit)
                {
                    String row = "";
                    foreach (Symbol r in symbol)
                    {
                        if (d == r.data[r.dataPos].ctm)
                        {
                            if (r == symbol[0]) // 1st time column, 2nd formulas (once only)
                            {
                                row += r.data[r.dataPos].GetDate();
                                if (formulas.Length > 0)
                                {
                                    String[] replace = formulas.Split('$');
                                    formulas = "";
                                    for (int i = 0; i < symbol.Count; i++)
			                        {
                                        Double divisor = symbol[i].data[symbol[i].dataPos].open
                                            + symbol[i].data[symbol[i].dataPos].high
                                            + symbol[i].data[symbol[i].dataPos].low
                                            + symbol[i].data[symbol[i].dataPos].close;
                                        replace[1 + i * 2] = divisor.ToString();
			                        }
                                    row += String.Join("", replace);
                                }
                                else
                                { // Do not repeat formulas
                                    row += new String('\t', symbol.Count);
                                }
                            }
                            if (r.dataPos >= r.data.Length)
                            {
                                exit = true;
                            }
                            else
                            { // Data block
                                row += '\t' + r.data[r.dataPos++].GetNumbers() + '\t';
                            }
                        }
                    }
                    if(!exit) {
                        row = row.Substring(0, row.Length - 1) + Environment.NewLine;
                        sb.Append(row);
                    }
                }
            } while (!exit);
            sw.Write(sb.ToString());
            sw.Close();
        }
    }
}
