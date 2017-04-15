#include "cArrayPtr.h"
#include "fntXorPacker.h"

//#define Testing
//===========================================================================
// Params: GLCD.glc G 10x19 chars ATMout.h
//         XORed.h  X 10x19 chars ATMout.h
//        ASCII.txt A 10x19 chars ATMout.h
//===========================================================================
int fntXorPacker(int argc, char *argv[])
{
	int argPos = 2;
    if(argc >= 5) {
		bool xored = false, glcd = false, ascii = false;
		if(!(xored = !strcmp(argv[argPos],"X")) && !(glcd = !strcmp(argv[argPos],"G")) && !(ascii = !strcmp(argv[argPos],"A")))
			return -1;
		argPos++;
        ifstream cFile;
        cFile.open(argv[1], ios::in); // first parameter - input font file
		bool rotated = false;
		if(cFile.is_open()) { // file processing block
			unsigned int width, height;
			if(argv[argPos][0] == 'R') { // rotate - not implemented yet
				argPos++;
				rotated = true;
			}
			width = atoi(argv[argPos]);
			height = atoi(strchr(argv[argPos++],'x') + 1);
			int chars = atoi(argv[argPos++]);
			unsigned int dataWidth, dataHeight;
			unsigned int charBufSize, bufSize;
			dataWidth = (width/8) + !!(width&7);
			dataHeight = (height/8) + !!(height&7);
			charBufSize = dataWidth * height;
			if(charBufSize < dataWidth * dataHeight) charBufSize = width * dataHeight;
			bufSize = (chars + 1) * charBufSize;
            string line;
			size_t pos = 0;
			bool data = false;
			while(!ascii && getline(cFile, line)) if(line.length()) { // skip inital preprocessor cmds and line comments
				pos = 0;
				while(isspace(line[pos])) pos++;
				if(line[pos] == '#') continue;
				if(line.substr(pos,2) == "//") continue;
				if(pos == line.length())continue;
				if((pos = (int)line.find('{')) >= 0) {
					data = true;
					line = line.substr(pos+1);
					break;
				}
            } // skip inital preprocessor cmds and line comments
			if(data || ascii) { // data (... { data } ...)
				unsigned int zerosNo0 = 0, zerosNo1 = 0; // # of zeros before and after XOR - 4statistics only
				string src, xor;
				cArrHolder fontBuf(bufSize);
				cArrHolder &nzData = *new cArrHolder(bufSize);
				(*fontBuf).set2(0);
				unsigned int rowBitmapSize = height * chars;
				unsigned char *rowBitmap = NULL;
				if(glcd || ascii) {
					rowBitmapSize = (rowBitmapSize / 8) + !!(rowBitmapSize & 7);
					rowBitmap = new unsigned char[rowBitmapSize+1];
					memset(rowBitmap, 0, rowBitmapSize+1);
				}
				unsigned int colBitmapSize = height * dataWidth * chars;
				unsigned char *colBitmap = NULL;
				if(glcd || ascii) {
					colBitmapSize = (colBitmapSize / 8) + !!(colBitmapSize & 7);
					colBitmap = new unsigned char[colBitmapSize];
					memset(colBitmap, 0, colBitmapSize);
				}
				unsigned char *transformBuf = new unsigned char[charBufSize*2];
				cArrHolder bufPos(*fontBuf, bufSize);
				size_t charPos = 0;
				string allowed = "0123456789ABCDEFX,";
				int bytesRead = 0;
				do { // process file
					if(line.length()) {
						pos = 0;
						string oneRead;
						if(ascii) { // ascii char read
							for(unsigned int r=0;r<height;r++) {
								if(r) oneRead += "|\n";
								pos = line.find_first_not_of("* ");
								if(pos > line.length()) pos = line.length();
								oneRead += line.substr(0, pos);
								getline(cFile, line);
							}
						} else { // binary char read
							while(isspace(line[pos])) pos++;
							bool hex = false, read = false;
							unsigned char readByte = 0;
							int hexPos;
							while((line.length() >= pos) && (hexPos = (int)allowed.find((char)toupper(line[pos]))) >= 0) {
								if(readByte && hexPos < 16) {
									if(hex) readByte <<= 4; else readByte *= 10;
								}
								if(hexPos < 10 || (hex && hexPos < 16)) {
									read = true;
									readByte += hexPos;
								}
								if(hexPos == 16) hex = true;
								if(line.length() > pos) pos++; else break;
								if(hexPos == 17) {
									read = false;
									hex = false;
									bufPos.addChar(readByte);
									bytesRead++;
									readByte = 0;
									charPos++;
									pos++;
								}
							}
							if(read) {
								bufPos.addChar(readByte);
								bytesRead++;
								charPos++;
							}
						} // binary char read
						if((glcd && charPos >= dataHeight) || ascii) { // 1 char read
							if(!ascii) {
								cArrHolder srcBuf(*bufPos, charBufSize);
								(*srcBuf).moveWindow(-(long)charPos);
								oneRead = readGlcChar(srcBuf, width, height);
							} else {
								oneRead += "|\n";
								oneRead += line; // char delimiter and new line will be removed during transformation
								oneRead += "\n";
							}
							cArrHolder srcBuf2(*bufPos, charBufSize*2);
							if(ascii) {
								(*bufPos).shiftPos(charBufSize);
							} else {
								bufPos += charBufSize - charPos;
							}
							(*srcBuf2).shiftPos(charBufSize);
							transformChar(oneRead, srcBuf2, width, dataWidth, charBufSize, height, zerosNo0);
							cArrHolder srcBuf3(*bufPos, charBufSize);
							(*srcBuf3).moveWindow(-(long)charBufSize);
							xorChar(srcBuf3, dataWidth, charBufSize, height, zerosNo1);
							oneRead = oneRead.substr(0, oneRead.length()-1); // cut last \n
							charPos = oneRead.find(oneRead[oneRead.length()-1]);
							oneRead = oneRead.substr(0, charPos + width); // cut char delimiter
							src += oneRead;
							src += "\n";
							charPos = 0;
							xor += readLinearChar(srcBuf3, width, height);
						}
					}
				} while(getline(cFile, line)); // process file
				unsigned char rowBMmask = 128, colBMbit = 7;
				unsigned int colByte;
				unsigned char *colBitmapPtr = colBitmap;
				unsigned char *rowBitmapPtr = rowBitmap;
				cArrHolder &nzDataPtr = *new cArrHolder(*nzData, bufSize);
				if(xored) {
					long dataSize = (*bufPos).getRelPos();
					(*bufPos).setPos(0l);
					colBitmapSize = ((**bufPos) << 8);
					++bufPos;
					colBitmapSize += (**bufPos); ++bufPos;
					chars = **bufPos; ++bufPos;
					dataWidth = **bufPos; ++bufPos;
					height = **bufPos; ++bufPos;
					rowBitmapSize = height * chars; // 89
					rowBitmapSize = (rowBitmapSize / 8) + !!(rowBitmapSize & 7);
					rowBitmap = const_cast<unsigned char *>((*bufPos).getPos()); // bad & dirty ;-)
					rowBitmapPtr = colBitmap = rowBitmap+rowBitmapSize;
					colBitmapPtr = colBitmap + colBitmapSize;
					(*nzData).RelRef();
					(*nzDataPtr).RelRef();
					nzData = *new cArrHolder(colBitmapPtr, (long)(dataSize - rowBitmapSize - colBitmapSize), false); // external buffer
					nzDataPtr = *new cArrHolder(*nzData, dataSize - rowBitmapSize - colBitmapSize);
					(*nzDataPtr).setPos(dataSize);
				} else if(glcd || ascii) { // glcd collecting info
					cArrHolder bufPos(*fontBuf, bufSize);
					for(int i=0;i<chars;i++) {
						for(unsigned int r=0;r<height;r++) { // collect row bitmap (zero or not zero)
							colByte = 0;
							for(unsigned int c=0;c<dataWidth;c++) {
								colByte <<= 1;
								if(**bufPos) {
									(*nzDataPtr).addChar(**bufPos);
									colByte++;
								}
								++bufPos;
							}
							if(colByte) {
								unsigned char shift = colBMbit+9-dataWidth; // shift to 15-th bit
								colByte <<= shift;
								*colBitmapPtr |= (colByte >> 8);
								if(shift < 8)
									*++colBitmapPtr |= (colByte & 255);
								else if(colBMbit < dataWidth)
									colBitmapPtr++;
								colBMbit = (colBMbit - dataWidth) & 7;
								*rowBitmapPtr |= rowBMmask;
							}
							rowBMmask >>= 1;
							if(!rowBMmask) {
								rowBMmask = 128;
								rowBitmapPtr++;
							}
						}
					}
					if(*colBitmapPtr) colBitmapPtr++;
					colBitmapSize = (unsigned int)(colBitmapPtr - colBitmap);
				} // glcd collecting info
//				delete[] fontBuf;
				string output = "= {"; // start data block
				output += "0x";
				output += allowed[colBitmapSize >> 12];
				output += allowed[(colBitmapSize >> 8) & 15];
				output += ",0x";
				output +=  allowed[(colBitmapSize >> 4) & 15];
				output +=  allowed[colBitmapSize & 15];

				output += ",0x";
				output +=  allowed[chars >> 4];
				output +=  allowed[chars & 15];

				output += ",0x";
				output +=  allowed[dataWidth];

				output += ",0x";
				output +=  allowed[height >> 4];
				output +=  allowed[height & 15];
				output += ",// CollBM size H,L, charsNo, byteWidth, bitHeight\n//RowBMP\n";
				if(*rowBitmapPtr && !xored) rowBitmapPtr++; // uncomplete byte - move end pos
				unsigned char *ptr = rowBitmap;
				while(ptr < rowBitmapPtr) {
					output += "0x";
					output += allowed[(*ptr) >> 4];
					output += allowed[(*ptr++) & 15];
					output += ",";
				}
				output += "\n//ColBMP\n";
				if(*colBitmapPtr && !xored) colBitmapPtr++; // uncomplete byte - move end pos
				colBitmapSize = (unsigned int)(colBitmapPtr - colBitmap);
				ptr = colBitmap;
				while(ptr < colBitmapPtr) {
					output += "0x";
					output += allowed[(*ptr) >> 4];
					output += allowed[(*ptr++) & 15];
					output += ",";
				}
				// Check unpacking from binary arrays
				string check;
				ptr = rowBitmapPtr;
				rowBMmask = 128;
				cArrHolder ptrNz(*nzData, bufSize);
				unsigned char *ptrCB = colBitmap;
				colBMbit = 128;
				ptr = rowBitmap;
				unsigned char charRows = height;
				size_t xoredPos = 0;
				while(ptr < rowBitmapPtr) {
					if(*ptr & rowBMmask) {
						int maxW = width;
						for(unsigned int c=0;c<dataWidth;c++) {
							if(*ptrCB & colBMbit) {
								for(unsigned char m=128;m && maxW--;m>>=1)
									check += ((**ptrNz)&m)?'*':' ';
								++ptrNz;
							} else {
								for(unsigned int c=0;c<8 && maxW--;c++)
									check += ' ';
							}
							colBMbit >>= 1;
							if(!colBMbit) {
								colBMbit = 128;
								ptrCB++;
							}
						}
					} else {
						for(unsigned int c=0;c<width;c++)
							check += ' ';
					}
					check += "|\n";
					if(!--charRows) {
						for(unsigned int c=0;c<width;c++)
							check += '-';
						check += "\n";
						if(xored) {
							cArrHolder nextChar(transformBuf, charBufSize*2, false); // external buffer
							(*nextChar).AddRef(); // keep parent alive
							(*nextChar).shiftPos(charBufSize);
							transformChar(check.substr(xoredPos), nextChar, width, dataWidth, charBufSize, height, zerosNo0);
							xorCharRev(transformBuf, dataWidth, charBufSize, height, zerosNo1);
							xoredPos = check.length();
							(*nextChar).shiftPos(-(long)charBufSize);
							src += readLinearChar(nextChar, width, height);
						}
						charRows = height;
					}
					rowBMmask >>= 1;
					if(!rowBMmask) {
						rowBMmask = 128;
						ptr++;
					}
				}
				if(xored) {
					xor = check;
				}
				// End of check
				output += "\n//Non-zero font data\n";
				cArrHolder nzDataRead(*nzData, (*ptrNz).getRelPos());
				while(!(*nzDataRead).isInvalid()) {
					output += "0x";
					output += allowed[(**nzDataRead) >> 4];
					output += allowed[(**nzDataRead) & 15];
					++nzDataRead;
					if((*nzDataRead).isInvalid()) break;
					else output += ",";
				}
				output += "\n};\n";
/*				// Save GLCD as ASCII
				if(argc > argPos) {
					ofstream o(argv[argPos++]);
					o << src;
					o.close();
				} else
					cout << src;
				// Save GLCD as XORed ASCII
				if(argc > argPos) {
					ofstream o(argv[argPos++]);
					o << xor;
					o.close();
				} else
					cout << xor;*/
				// Save GLCD as tiny font data
				if(argc > argPos) {
					ofstream o(argv[argPos++]);
					int orig = height*width*chars;
					orig = (orig/8) + !!(orig&7);
					o << "// Zero's no:" << zerosNo1 << "-" << zerosNo0 << "=" << zerosNo1-zerosNo0 << 
						" tabs size 5+" << (rowBitmapPtr-rowBitmap) << "+" << colBitmapSize << "=" << 5+(rowBitmapPtr-rowBitmap)+colBitmapSize <<
						" data:" << (*nzDataPtr).getRelPos() << " of b->B " << orig << " => " << 
						(int)(5+(rowBitmapPtr-rowBitmap)+colBitmapSize+(*nzDataPtr).getRelPos())*100 / orig - 100 << "%" << endl;
					o << output;
					o << "/*" << endl;
					size_t startLine = 0, endLine;
					rowBMmask = 128;
					unsigned char *ptrCB = colBitmap;
					colBMbit = 128;
					ptr = rowBitmap;
					cArrHolder bufPos(*nzData, bufSize);
					int rows = height;
					while((endLine = src.find('\n', startLine+1)) != string::npos) {
						o << src.substr(startLine, endLine - startLine);
						o << xor.substr(startLine, endLine - startLine);
						if(rows > 0) {
							o << (int)!!(*ptr & rowBMmask);
							if(*ptr & rowBMmask) {
								unsigned char cols = 0;
								for(unsigned int c=0;c<dataWidth;c++) {
									cols <<= 1;
									cols |= !!(*ptrCB & colBMbit);
									o << (cols & 1);
									colBMbit >>= 1;
									if(!colBMbit) {
										colBMbit = 128;
										ptrCB++;
									}
								}
								string bin, hexVals;
								unsigned char colsMask = 1 << (dataWidth-1);
								while(cols) {
									unsigned int val = 0;
									if(cols & colsMask) {
										val = **bufPos;
										++bufPos;
									}
									if(val) {
										hexVals += " 0x";
										hexVals += allowed[val >> 4];
										hexVals += allowed[val & 15];
									}
									val = val*2 + 1;
									while(val&255) {
										if((val & 511) == 256) break;
										if(val&256) bin += '*'; else bin += '.';
										val <<= 1;
									}
									bin += ' ';
									cols <<= 1;
								}
								o << bin.substr(0,width) << hexVals;
							} else {
								for(unsigned int c=0;c<dataWidth;c++)
									o << '-';
							}
						}
						o << endl;
						if(rows > 0)
							rowBMmask >>= 1;
						startLine = endLine+1;
						if(!rowBMmask) {
							rowBMmask = 128;
							ptr++;
						}
						rows--;
						if(rows < 0)
							rows = height;
					}
					o << "*/" << endl;
					o.close();
				} else
					cout << output;
				if(!xored) {
					delete[] rowBitmap;
					delete[] colBitmap;
				}
				delete &nzDataPtr;
				delete &nzData;
				delete[] transformBuf;
			} // data (... { data } ...)
		} // file processing block
    }
    return 0;
}
//===========================================================================
// Convert GLCD bitmap data to ASCII
//===========================================================================
string readGlcChar(cArrHolder &srcBuf, unsigned int dataWidth, unsigned int height) {
	string picture;
	unsigned char mask = 1;
	unsigned int bytesRead = 0;
	for(int y=0;height;height--) {
		if(!mask) {
			mask = 1;
			y++;
		}
		for(unsigned int x=0;x<dataWidth;x++) {
			if(**srcBuf & mask) picture += '*'; else picture += ' ';
			++srcBuf;
		}
		picture += "|\n";
		mask <<= 1;
		if(mask) { (*srcBuf).shiftPos(-(long)dataWidth);
		} else bytesRead += dataWidth;
	}
	for(unsigned int x=0;x<dataWidth;x++)
		picture += "-";
	picture += "\n";
	return picture;
}

//===========================================================================
// Convert ASCII string to common BMP font data
//===========================================================================
void transformChar(string src, cArrHolder& destBuf, unsigned int width, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo0) {
	unsigned char mask = 128;
	unsigned char byte = 0;
	src = src.substr(0, src.length()-2); // cut last \n
	src = src.substr(0, src.find(src[src.length()-1])); // cut char delimiter
	const char *pos = src.c_str();
	cArrHolder bufPos(*destBuf, size);
	unsigned int bufWritten = 0;
	bool cycle;
#ifdef Testing
	string check;
#endif
	while(*pos) {
		byte = 0;
		while(mask) {
			cycle = false;
			if(*pos == '|') break;
			else if(*pos == '\n') break;
			else if(*pos++ != ' ') byte |= mask;
			cycle = true;
			mask >>= 1;
		}
		if(cycle || ((*pos == '|') && (dataWidth*8 > width))) {
			(*bufPos).addChar(byte);
#ifdef Testing
			mask = 128;
			while(mask) {
				check += mask&byte?'*':' ';
				mask >>= 1;
			}
#endif
			bufWritten++;
		}
		if(!cycle) {
			mask = 128;
			while(*pos++ != '\n');
#ifdef Testing
			check += "skip2LF\n";
#endif
		}
		if(!mask) {
			mask = 128;
		}
	}
#ifdef Testing
	cout << check;
#endif
	(*destBuf).copyData(-(long)size, 0, size);
	cArrHolder srcBuf(*destBuf, size);
	for(unsigned int r=0;r<height;r++)
		for(unsigned int c=0;c<dataWidth;c++) {
			(*srcBuf).setPos(c+(r*dataWidth));
			if(!**srcBuf) zerosNo0++;
		}
}

//===========================================================================
// XOR data
//===========================================================================
void xorChar(cArrHolder& destBuf, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo1) {
	// XOR from bottom to 2-nd line
	cArrHolder bufPos(*destBuf, size - dataWidth); // -1 row
	(*bufPos).moveWindow(dataWidth); // 1 row down
	(*bufPos).setPos((*bufPos).end()); // end
	cArrHolder srcBuf(*destBuf, size - dataWidth); // -1 row
	(*srcBuf).setPos((*srcBuf).end()); // end -1
	
#ifdef Testing
	string check;
	unsigned char mask = 128;
	unsigned char *destBuf2 = (*destBuf).getPos();
	for(int r=0;r<height;r++) {
		for(int c=0;c<dataWidth;c++) {
			mask = 128;
			while(mask) {
				check += mask&*(destBuf2+r*dataWidth+c)?'*':' ';
				mask >>= 1;
			}
		}
		check += '\n';
	}
	cout << check;
#endif
	do {
		(*bufPos).setData(**bufPos ^ **srcBuf);
		--bufPos; --srcBuf;
	} while(!(*srcBuf).isInvalid());
	cArrHolder rowBufPos(*destBuf, dataWidth); // -1 row
	unsigned int b;
	for(unsigned int i=0;i<height;i++) {
		// row XOR
		bool carry = false;
		do {
			b = carry?256:0;
			b ^= **rowBufPos;
			carry = !!(b & 1);
			b ^= (b >> 1);
			b &= 0xFF;
			(*rowBufPos).setData(b);
			++rowBufPos;
		} while(!(*rowBufPos).isInvalid());
		// row XOR
		// row unXOR
/*		(*rowBufPos).shiftPos(-(int)dataWidth);
		b = 0;
		do {
			if(b&1) b = 128; else b = 0;
			b ^= **rowBufPos;
			b = b ^ (b>>1);
			b ^= b >> 2;
			b ^= b >> 4;
			(*rowBufPos).setData(b);
			++rowBufPos;
		} while(!(*rowBufPos).isInvalid());
*/		// row unXOR
		if(i < height-1) // exception
			(*rowBufPos).moveWindow(dataWidth);
	}
#ifdef Testing
	for(int r=0;r<height;r++) {
		for(int c=0;c<dataWidth;c++) {
			mask = 128;
			while(mask) {
				check += mask&*(destBuf2+r*dataWidth+c)?'*':' ';
				mask >>= 1;
			}
		}
		check += '\n';
	}
	cout << check;
#endif
	cArrHolder zerosBuf(*destBuf, size);
	for(unsigned int r=0;r<height;r++)
		for(unsigned int c=0;c<dataWidth;c++) {
			(*zerosBuf).setPos(c+(r*dataWidth));
			if(!**zerosBuf) zerosNo1++;
		}
}

//===========================================================================
// unXOR data
//===========================================================================
void xorCharRev(unsigned char *destBuf, unsigned int dataWidth, unsigned int size, unsigned int height, unsigned int& zerosNo1) {
	// XOR from top to bottom-1
	unsigned char *bufPos = destBuf;
	unsigned char *srcBuf = bufPos + size - dataWidth;
	while(bufPos < srcBuf) {
		*(bufPos+dataWidth) ^= *bufPos++;
	}
	// row unXOR
	bufPos = destBuf;
	int b = 0;
	for(int i=0;i<size;i++) {
		if(b&1) b = 128; else b = 0;
		b ^= *bufPos;
		b = b ^ (b>>1);
		b ^= b >> 2;
		b ^= b >> 4;
		*bufPos++ = b;
	}
	// row unXOR
	for(unsigned int r=0;r<height;r++)
		for(unsigned int c=0;c<dataWidth;c++)
			if(!*(srcBuf+c+(r*dataWidth))) zerosNo1++;
}

//===========================================================================
// Read common font data 2 string
//===========================================================================
string readLinearChar(cArrHolder& srcBuf, unsigned int width, unsigned int height) {
	string picture;
	unsigned char mask = 128;
	unsigned int bytesRead = 0;
	for(unsigned int y=0;y<height;y++) {
		for(unsigned int x=0;x<width;x++) {
			if(**srcBuf & mask) picture += '*'; else picture += ' ';
			mask >>= 1;
			if(!mask) {
				mask = 128;
				++srcBuf;
				bytesRead++;
			}
		}
		if(mask && ((width < 8) || (width & 7)))
			++srcBuf;
		mask = 128;
		bytesRead++;
		picture += "|\n";
	}
	for(unsigned int x=0;x<width;x++)
		picture += "-";
	picture += "\n";
	return picture;
}
