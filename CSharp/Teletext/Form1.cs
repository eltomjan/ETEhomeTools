using System;
using System.Drawing;
using System.IO;
using System.Net;
using System.Windows.Forms;

namespace Teletext
{
    public partial class Form1 : Form
    {
        Font f = new Font();

        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string pg = ((System.Windows.Forms.TextBox)sender).Text;
            string ocr = string.Empty;
            if (pg.Length >= 3)
            {
                using (WebClient client = new WebClient())
                {
                    ServicePointManager.SecurityProtocol = SecurityProtocolType.Ssl3 | SecurityProtocolType.Tls | SecurityProtocolType.Tls11 | SecurityProtocolType.Tls12;
                    Stream stream = client.OpenRead("https://www.ceskatelevize.cz/services-old/teletext/picture.php?channel=CT2&page=" + pg);
                    Bitmap bitmap; bitmap = new Bitmap(stream);

                    if (bitmap != null)
                    {
                        for (int y = 0; y < bitmap.Height; y += 12)
                        {
                            for (int x = 0; x < bitmap.Width; x += 8)
                            {
                                string box = string.Empty;
                                //Color[,] pict = new Color[12,8];
                                Color? first = null;
                                for (int yc = 0; yc < 12; yc++)
                                {
                                    for (int xc = 0; xc < 8; xc++)
                                    {
                                        //pict[yc,xc] = bitmap.GetPixel(x + xc, y + yc);
                                        Color c = bitmap.GetPixel(x + xc, y + yc);
                                        if (first == null)
                                        {
                                            first = c;
                                            box = " ";
                                        }
                                        else
                                        {
                                            box += (c != first) ? "1" : " ";
                                        }

                                    }
                                    box += "\n";
                                }
                                if (f.toChar.ContainsKey(box)) ocr += f.toChar[box];
                                else ocr += ' ';
                            }
                            ocr += '\n';
                        }
                        richTextBox1.Text = ocr;
                    }
                    stream.Flush();
                    stream.Close();
                }
            }
        }
    }
}
