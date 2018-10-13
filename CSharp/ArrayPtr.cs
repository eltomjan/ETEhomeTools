class ArrayPtr
{
    private ArrayPtr m_parent;
    private int m_pos;
    private byte m_bitPos;
    private int m_begin;
    private int m_end;
    private byte[] m_data;

    public ArrayPtr()
    {
        m_bitPos = 7;
    }
    public ArrayPtr(int c_size)
    {
        //ArrayPtr();
        if (c_size >= 0)
        {
            m_data = new byte[c_size];
            m_begin = 0;
            m_end = m_begin + c_size - 1;
            m_pos = 0;
            m_bitPos = 7;
        }
    }
    public ArrayPtr(ArrayPtr parent, int c_size)
    {
        //ArrayPtr();
        m_parent = parent;
        m_pos = parent.m_pos;
        m_bitPos = parent.m_bitPos;
        m_begin = parent.m_pos;
        if (parent.m_end > parent.m_pos + c_size - 1)
        {
            m_end = parent.m_pos + c_size - 1;
        }
        else
        {
            m_end = parent.m_end;
        }
        m_data = parent.m_data;
    }

    public int getBytesUsed()
    {
        if (m_bitPos == 7)
        {
            return m_pos - m_begin;
        }
        else
        {
            return m_pos - m_begin + 1;
        }
    }

    bool setPos(int pos)
    {
        int bug;
        if (0 == (bug = checkPos(pos)))
        {
            m_pos = pos;
            m_bitPos = 7;
            return true;
        }
        return false;
    }

    bool setBitPos(byte pos)
    {
        int bug;
        if (pos <= 7 && (0 == (bug = checkPos(m_pos))))
        {
            m_bitPos = pos;
            return true;
        }
        return false;
    }

    int checkPos(int pos)
    {
        if (pos < m_begin)
        {
            m_pos = -1; // ev. fix, should report error when it was less
            return -1;
        }
        else if (pos > m_end)
        {
            m_pos = m_end + 1; // ev. fix, should report error when it was more
            return 1;
        }
        return 0;
    }

    public int getPos()
    {
        return m_pos;
    }

    public int Capacity()
    {
        return m_end - m_begin + 1;
    }
    public byte getBitPos()
    {
        return m_bitPos;
    }

    public int isInvalid()
    {
        return checkPos(m_pos);
    }

    public bool addBit(byte b)
    { // m_pos not adjusted => should not be used outside
        if (m_pos <= m_end)
        {
            if (m_bitPos < 8)
            {
                byte bitMask = (byte)(1 << m_bitPos);
                if (b > 0)
                {
                    m_data[m_pos] |= bitMask;
                }
                else
                {
                    m_data[m_pos] &= (byte)(255 - bitMask);
                }
                if (0 == m_bitPos)
                {
                    m_bitPos = 8;
                    m_pos++;
                }
                m_bitPos--;
            }
            return true;
        }
        return false;
    }

    public void shrink2pos()
    {
        System.Array.Resize(ref m_data, m_pos + 1);
        m_end = m_pos;
    }

    public string getHexBytes()
    {
        string result = "";
        foreach (var rb in m_data)
        {
            result += System.String.Format("0x{0:X2},", rb);
        }
        return result;
    }
};