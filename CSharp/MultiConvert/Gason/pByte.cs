using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace Gason
{
    [StructLayout(LayoutKind.Explicit)]
    public struct P_ByteLnk
    {
        [FieldOffset(0)]
        public Int64 data;
        [FieldOffset(0)]
        public double number;
        [FieldOffset(0)]
        public Int32 length;
        [FieldOffset(4)]
        public Int32 pos;
    }
    public struct LinkedByteString
    {
        public static Byte[] storage;
        public P_ByteLnk idxes;
        public Int32 length { get { return idxes.length; } set { idxes.length = value; } }
        public LinkedByteString(P_ByteLnk idxes)
        {
            this.idxes = idxes;
        }
        public override string ToString()
        {
            if (idxes.length == -1) return "#N/A - resetted";
            if (idxes.data == 0) return "#Not initialized";
            return Encoding.UTF8.GetString(storage, idxes.pos, idxes.length);
        }
    }
    public class ByteString
    {
        readonly Byte[] storage;
        public int length;
        public ByteString(Byte[] storage, P_ByteLnk idxes)
        {
            this.storage = new Byte[idxes.length + 1];
            Array.Copy(storage, idxes.pos, this.storage, 0, idxes.length);
            length = idxes.length;
        }
        public ByteString(String s)
        {
            this.storage = Encoding.UTF8.GetBytes(s);
            this.length = s.Length;
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        private bool CompareRaw(Byte[] src, int pos)
        {
            for(int i=0;i<length;i++)
            {
                if (src[pos++] != storage[i]) return false;
            }
            return true;
        }
        public override string ToString()
        {
            return Encoding.UTF8.GetString(storage, 0, length);
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Boolean Equals(Byte[] src, P_ByteLnk width)
        {
            if (length != width.length) return false;
            return CompareRaw(src, width.pos);

        }
    }
}
