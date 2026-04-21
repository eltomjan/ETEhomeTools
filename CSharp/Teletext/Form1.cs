using System;
using System.Drawing;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Windows.Forms;
using ImageMagick;


namespace Teletext
{
    public partial class Form1 : Form
    {
        Font f = new Font();

        public Form1()
        {
            InitializeComponent();
            textBox1_TextChanged(textBox1, new EventArgs());
        }

        public async Task<Bitmap> LoadWebpAsBitmap(string pg)
        {
            try
            {
                using (var client = new HttpClient())
                {
                    client.Timeout = TimeSpan.FromSeconds(10);

                    var response = await client.GetAsync("https://api-teletext.ceskatelevize.cz/pages/" + pg + "/image.webp");

                    response.EnsureSuccessStatusCode();

                    var bytes = await response.Content.ReadAsByteArrayAsync();

                    using (MagickImage mi = new MagickImage(bytes))
                    {
                        mi.Format = MagickFormat.Bmp;
                        using (MemoryStream ms = new MemoryStream())
                        {
                            mi.Write(ms);
                            ms.Position = 0;
                            return new Bitmap(ms);
                        }
                    }
                }
            }
            catch (HttpRequestException)
            {
                return null;
            }
            catch (Exception)
            {
                return null;
            }
        }
        private async void textBox1_TextChanged(object sender, EventArgs e)
        {
            string pg = ((System.Windows.Forms.TextBox)sender).Text.ToUpper();
            string ocr = string.Empty;
            if (pg.Length >= 3 && pg.Length < 5)
            {
                using (WebClient client = new WebClient())
                {
                    ServicePointManager.SecurityProtocol = SecurityProtocolType.Ssl3 | SecurityProtocolType.Tls11 | SecurityProtocolType.Tls12;

                    Bitmap bitmap = await LoadWebpAsBitmap(pg);
                    if (bitmap == null)
                    {
                        if (pg.Length == 3)
                        {
                            bitmap = await LoadWebpAsBitmap(pg + 'A');
                        }
                    }

                    if (bitmap == null) {
                        richTextBox1.Text = pg + " nenalezena ?!";
                        return;
                    }
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
                                        box += (c != first) ? "█" : " ";
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
            }
        }
    }
}
