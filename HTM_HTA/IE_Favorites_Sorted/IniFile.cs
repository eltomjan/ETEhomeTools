using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace CodeProject.IEFavDecon
{
    /// <summary>
    /// Interop wrapper for the legacy "PrivateProfileString" API.
    /// </summary>
    public class IniFile
    {
        [DllImport("KERNEL32.DLL", EntryPoint = "WritePrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError = true)]
        private static extern int WritePrivateProfileString(string section, string key, string value, string filename);

        [DllImport("KERNEL32.DLL", EntryPoint = "GetPrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError=true)]
        private static extern int GetPrivateProfileString(string section, string key, string defaultValue, StringBuilder returnString, int returnSize, string filename);

        private string filename;

        public IniFile(string filename)
        {
            this.filename = filename;
        }

        public string GetValue(string section,string key,string defaultValue)
        {
			StringBuilder sb = new StringBuilder(32768);
            GetPrivateProfileString(section, key, defaultValue, sb, sb.Capacity, filename);
			return sb.ToString();
        }

        public void WriteValue(string section, string key, string value)
        {
            if (WritePrivateProfileString(section, key, value, filename) == 0)
            {
                throw new Exception("WritePrivateProfileString failed: code " + Marshal.GetLastWin32Error().ToString());
            }
        }
    }
}
