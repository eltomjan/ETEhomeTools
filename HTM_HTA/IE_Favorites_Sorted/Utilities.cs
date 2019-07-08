using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.IO;
using System.Resources;
using System.Runtime.InteropServices;

namespace CodeProject.IEFavDecon
{
    /// <summary>
    /// Some utility methods needed to load internet explorer favorites.
    /// </summary>
    public class Utilities
    {
        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport("user32.dll")]
        private static extern int LoadString(IntPtr hInstance, uint uID, StringBuilder lpBuffer, int nBufferMax);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool FreeLibrary(IntPtr hModule);

        private static String GetStringResource(IntPtr hModuleInstance, uint uiStringID)
        {
            StringBuilder sb = new StringBuilder(2048);
            LoadString(hModuleInstance, uiStringID, sb, sb.Capacity);
            return sb.ToString();
        }

        public static String GetStringResource(string resourceIdentifier)
        {
            string result = "";
            string[] splitted = resourceIdentifier.Split(new char[] { ',' });
            string dllName = splitted[0].Trim('@');

            IntPtr lib = LoadLibrary(dllName);

            if (lib != IntPtr.Zero)
            {
                uint stringId = (uint)(-Int32.Parse(splitted[1]));
                result = GetStringResource(lib, stringId);
                FreeLibrary(lib);
            }
            return result;
        }

        public static string ReadZeroTerminatedString(Stream stream, Encoding encoding)
        {
            long position = stream.Position;
            StreamReader sr = new StreamReader(stream, encoding);
            StringBuilder stringBuilder = new StringBuilder();

            while (true)
            {
                char c = (char)sr.Read();
                if (c == 0) break;
                stringBuilder.Append(c);
            }

            string result = stringBuilder.ToString();

            if (encoding == Encoding.ASCII)
            {
                stream.Position = position + result.Length + 1;
            }
            else
            {
                stream.Position = position + (result.Length * 2) + 2;
            }
            return result;
        }

        private static byte CheckSum(long fileTime)
        {
            return (byte)(((fileTime >> (0 * 8)) +
                    (fileTime >> (1 * 8)) +
                    (fileTime >> (2 * 8)) +
                    (fileTime >> (3 * 8)) +
                    (fileTime >> (4 * 8)) +
                    (fileTime >> (5 * 8)) +
                    (fileTime >> (6 * 8)) +
                    (fileTime >> (7 * 8))) & 0xff);
        }

        public static DateTime HexEncodingToDateTime(string hex)
        {
            DateTime result;
            if (hex.Length >= 18)
            {
                MemoryStream ms = new MemoryStream();

                for (int t = 0; t < 9; t++)
                {
                    ms.WriteByte(Convert.ToByte(hex.Substring(t * 2, 2), 16));
                }

                ms.Seek(0, SeekOrigin.Begin);
                BinaryReader br = new BinaryReader(ms);
                long fileTime = br.ReadInt64();
                if (CheckSum(fileTime) == br.ReadByte())
                {
                    result = DateTime.FromFileTime(fileTime);
                }
                else
                {
                    result = DateTime.Now;
                }
            }
            else
            {
                result = DateTime.Now;
            }
            return result;
        }

        public static string DateTimeToHexEncoding(DateTime dateTime)
        {
            long fileTime = dateTime.ToFileTime();
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write(fileTime);
            bw.Write(CheckSum(fileTime));
            bw.Flush();
            StringBuilder sb = new StringBuilder();
            foreach (byte b in ms.ToArray())
            {
                sb.AppendFormat("{0:x2}", b);
            }
            return sb.ToString();
        }
    }
}
