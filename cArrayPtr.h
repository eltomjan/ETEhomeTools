#include <stdio.h>
#include <new>
#include <string.h>

//#define TESTING
#define PREVIEW_SIZE 200
class cArray {
public:
	cArray() {
		m_refCnt = 0;
		m_parent = NULL;
		m_begin = m_pos = m_end = NULL;
		m_manageData = true;
	}
	cArray(long c_size) {
		new (this) cArray();
		if(c_size >= 0) {
			m_data = (unsigned char(*)[PREVIEW_SIZE])new unsigned char[c_size];
			AddRef();
			m_begin = (unsigned char*)&m_data[0];
			m_end = m_begin + c_size - 1;
			m_pos = m_begin;
//			for(int i=0;i<c_size;i++) (*m_data)[i] = i;
		}
	}
	cArray(const cArray& parent, long c_size) {
		new (this) cArray();
		m_parent = const_cast<cArray*>(&parent);
		m_pos = parent.m_pos;
		m_begin = parent.m_pos;
		if(parent.m_end > parent.m_pos + c_size - 1) {
			m_end = parent.m_pos + c_size - 1;
		} else {
			m_end = parent.m_end;
		}
		m_data = parent.m_data;
		AddRef();
	}
	cArray(unsigned char *parent, long c_size, bool manageData = false) {
		new (this) cArray();
		m_manageData = manageData;
		m_parent = NULL;
		move2cArray(parent, c_size);
		AddRef();
	}
	int AddRef() {
		return m_refCnt++;
	}
	int RelRef ()
	{ 
		if (--m_refCnt == 0) {
			if(!m_parent) {
#ifdef TESTING
				printf("Garbage-collecting cArray\n");
#endif
				delete this;
			} else {
#ifdef TESTING
				printf("cArray has parrent !\n");
#endif
				m_refCnt++;
//				throw "cArray has parrent !\n";
			}
			return 0;
		}
		return m_refCnt;
	}
	void set2(unsigned char val, bool rest = false) {
		if(rest) {
			if(m_end >= m_pos)
				memset(m_pos, val, m_end - m_pos + 1);
		} else {
			if(m_end >= m_begin)
				memset(m_begin, val, m_end - m_begin + 1);
		}
	}
	bool addChar(unsigned char c) {
		if(m_pos <= m_end) {
			*m_pos++ = c;
			return true;
		}
		return false;
	}
	bool shiftPos(long number) {
		if(m_pos + number > m_end) {
#ifdef TESTING
			printf("cArray move past end !\n");
#endif
			m_pos = m_end + 1;
			return false;
		} else if(m_pos + number < m_begin) {
#ifdef TESTING
			printf("cArray move past end !\n");
#endif
			m_pos = m_begin - 1;
			return false;
		} else {
			m_pos += number;
			return true;
		}
	}
	bool setPos(unsigned char *pos) {
		int bug;
		if(!(bug = checkPos(pos))) {
			m_pos = pos;
			return true;
		}
#ifdef TESTING
			printf("cArray setPos %i !\n", bug);
#endif
		return false;
	}
	bool setPos(long pos) {
		int bug;
		if(!(bug = checkPos(m_begin + pos))) {
			m_pos = m_begin + pos;
			return true;
		}
#ifdef TESTING
			printf("cArray setPos %i !\n", bug);
#endif
		return false;
	}
	const int checkPos(unsigned char *pos) {
		if(m_pos < m_begin) {
			m_pos = m_begin - 1;
			return -1;
		} else if(m_pos > m_end) {
			m_pos = m_end + 1;
			return -1;
		}
		return 0;
	}
	const unsigned char operator * () {
		int bug;
		if(!(bug = checkPos(m_pos))) {
			return *m_pos;
		} else {
#ifdef TESTING
			printf("cArray read out of array %i !\n", bug);
#endif
			throw "cArray read out of array !\n";
		}
	}
	bool copyData(long shiftDest, long shiftSrc, unsigned int size) {
		int bug;
		unsigned char *src = m_pos + shiftSrc, *dest = m_pos + shiftDest;
		if(!(bug = checkPos(src)))
			if(!(bug = checkPos(dest)))
				if(!(bug = checkPos(src + size - 1)))
					if(!(bug = checkPos(dest + size - 1))) {
						memcpy(dest, src, size);
						return true;
					}
#ifdef TESTING
		printf("cArray copyData out of array !\n");
#endif
		throw "cArray copyData out of array !\n";
		return false;
	}
	unsigned char *getPos() {
		return m_pos;
	}
	bool moveWindow(long shift) {
		int bug;
		unsigned char *begin = m_begin + shift, *end = m_end + shift;
		if(!(bug = m_parent->checkPos(begin)))
			if(!(bug = m_parent->checkPos(end))) {
				m_begin = begin;
				m_end = end;
				m_pos = begin;
				return true;
			}
#ifdef TESTING
		printf("cArray moveWindow out of parent %i !\n", bug);
#endif
		throw "cArray moveWindow out of parent !\n";
		return false;
	}
	bool setData(unsigned char char2Bset) {
		int bug;
		if(!(bug = checkPos(m_pos))) {
			*m_pos = char2Bset;
			return true;
		}
#ifdef TESTING
		printf("cArray setData out of array %i !\n", bug);
#endif
		throw "cArray setData out of array !\n";
	}
	int isInvalid() {
		return checkPos(m_pos);
	}
	unsigned char *end() {
		return m_end;
	}
	const long getRelPos() const {
		return (long)(m_pos - m_begin);
	}

protected:
	void move2cArray(unsigned char *parent, long c_size) {
		m_pos = parent;
		m_begin = parent;
		m_end = m_pos + c_size - 1;
		m_data = (unsigned char(*)[PREVIEW_SIZE])parent;
	}

	cArray *m_parent;
	unsigned char *m_pos;
	unsigned char *m_begin;
	unsigned char *m_end;
	unsigned char (*m_data)[PREVIEW_SIZE];
	~cArray() {
		if((m_data && !m_parent) && m_manageData) {
			delete[] m_data;
			m_data = NULL;
		}
	};
private:
	int m_refCnt;
	bool m_manageData;
};

class cArrHolder {
public:
	cArrHolder() {
		m_ptr = NULL;
	}
	cArrHolder(long size) {
		new (this) cArrHolder();
		m_ptr = new cArray(size);
	}
	cArrHolder(const cArray& parent, long c_size) {
		m_ptr = new cArray(parent, c_size);
	}
	cArrHolder(unsigned char* binArray, long size, bool manageData = false) {
		m_ptr = new cArray(binArray, size, manageData);
	}
	~cArrHolder() {
		m_ptr->RelRef();
	}
	cArray& operator * () const
	{
#ifdef TESTING
		if (!m_ptr)
			printf("cArray attempt to use NULL pointer. !\n");
#endif
		return *m_ptr;
	}
	bool addChar(unsigned char c) { return m_ptr->addChar(c); }
	cArrHolder& operator += (long number) {
		m_ptr->shiftPos(number);
		return *this;
	}
	cArray& operator ++() const {
		m_ptr->setPos(m_ptr->getPos() + 1);
		return *m_ptr;
	}
	cArray& operator --() const {
		m_ptr->setPos(m_ptr->getPos() - 1);
		return *m_ptr;
	}

protected:
	cArray *m_ptr;
};
#undef TESTING
