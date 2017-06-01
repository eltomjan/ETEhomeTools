#define SPEED 2688
#define PITCH 5
#define NOISE 2
#define STEP 1
#define VOLUMELIMIT 32
#define BITRATE 44000
#include <Windows.h>
/*
A simple sound library for CSE 20211 by Douglas Thain
For course assignments, you should not change this file.
For complete documentation, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2013/wavfile
*/
//wavfile.h
#ifndef WAVFILE_H
#define WAVFILE_H

#include <stdio.h>
#include <string.h>

FILE * wavfile_open( const char *filename, int samples_per_second, int bits_per_sample );
void wavfile_write( FILE *file, short data[], int length );
void wavfile_close( FILE * file );

#define WAVFILE_SAMPLES_PER_SECOND 44100

#endif

#include <stdio.h>

using namespace System;
unsigned char NumTXT[] = {//(unsigned char*)"\0NULA\0JEDNA\0DVA\0TR'I\0C'TYR'I\0PJET\0S'EST\0SEDUM\0OSUM\0DEVJET"
	0,'N','U','L','A',0,'J','E','D','N','A',0,'D','V','A',0,'T','R','\'','I',0,'C','\'','T','Y','R','\'','I',0,'P','J','E','T',0,'S','\'','E','S','T',
	0,'S','E','D','U','M',0,'O','S','U','M',0,'D','E','V','J','E','T',0,
//TABLE1:	; index do tab2
// next 00052 bytes is from file:hlasdat1.bin = index písmena
// A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z   
   0, 2, 6,10,14,16,18,22,26,28,34,38,42,46,50,52,56,66,72,74,78,80,80,86,26,92,
//00 02 06 0a 0e 10 12 16 1a 1c 22 26 2a 2e 32 34 38 42 48 4a 4e 50 50 56 1a 5c
// A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
   100,102,110,114,120,122,192,130,132,192,192,192,134,138,144,146,192,156,164,166,172,174,192,192,132,186,
//0x64,x66,x6e,x72,x78,x7a,xc0,x82,x84,xc0,xc0,xc0,x86,x8a,x90,x92,xc0,x9c,xa4,xa6,xac,xae,xc0,xc0,x84,xba
//.db 100,102,112,116,122,124,194,132,134,194,194,194,136,140,146,148,194,158,166,168,174,176,194,194,134,188

// next 00194 bytes is from file:hlasdat2.bin
//  AAAAAA B                  C0
//unsigned char TABLE2[] = {
	54,129,52,25,49,171,24,25,145,195,52,25,49,224,54,132,146,227,53,25,81,156,49,49,52,150,54,135,51,58,50,61,50,192,
//0x36  81 34 19 31  ab 18 19  91  c3 34 19 31  e0 36  84  92  e3 35 19 51  9c 31 31 34  96 36  87 33 3a 32 3d 32  c0
	24,25,81,156,51,34,49,177,49,49,54,165,49,49,54,168,54,138,24,25,49,171,24,25,81,28,52,49,50,52,50,183,34,16,
//0x18 19 51  9c 33 22 31  b1 31 31 36  a5 31 31 36  a8 36  8a 18 19 31  ab 18 19 51 1c 34 31 32 34 32  b7 22 10
	19,25,33,174,146,195,24,25,49,224,54,141,52,49,50,52,50,183,24,25,113,28,146,195,50,49,50,67,50,68,50,197,63,129,
//  13 19 21  ae  92  c3 18 19 31  e0 36  8d 34 31 32 34 32  b7 18 19  71 1c  92  c3 32 31 32 43 32 44 32  c5 3f  81
//                            C'
	  52,25,49,43,51,58,50,61,24,25,145,211,       51,25,113,109,50,147,62,132,146,99,51,58,50,61,50,192,146,243,62,135,
//    34 19 31 2b 33 3a 32 3d 18 19  91  d3        33 19  71  6d 32  93 3e  84  92 63 33 3a 32 3d 32  c0  92  f3 3e  87
//.db 52,25,49,43,51,58,50,61,50,192,24,25,145,211,51,25,113,109,50,147,62,132,146,99,51,58,50,61,50,192,146,243,62,135
	49,49,54,37,49,49,53,37,50,147,62,138,24,25,49,43,51,58,50,61,/*A*/50,192,19,25,50,96/*A*/,19,25,113,221,146,211,24,25,
//  31 31 36 25 31 31 35 25 32  93 3e  8a 18 19 31 2b 33 3a 32 3d      32  c0 13 19 32 60      13 19  71  dd  92  d3 18 19
	113,109,50,147,62,141,52,49,50,52,50,55,51,58,50,61,50,192,50,83,50,84,50,213
//   71  6d 32  93 3e  8d 34 31 32 34 32 37 33 3a 32 3d 32  c0 32 53 32 54 32  d5
,/*};unsigned char TABLE3[] = {*/
//next 00263 bytes is from file:hlasdat3.bin
//             AAAAAAAAAAAAAAAAAAAAAA,EEEEEEEEEEEEEEEEEEE,YYYYYYYYYYYYYYYYY,OOOOOOOOOOOOOOOOOOOOO,UUUUUUUUUUUUUUUUUU,RRRRRR
	26,153,225,195,225,199,143,15,248,62,15,7,193,227,255,64,23,255,0,3,248,124,193,241,248,3,254,0,127,252,0,63,248,15,
//  1a  99  e1  c3  e1  c7  8f 0f  f8 3e 0f 07 c1  e3  ff 40 17 ff 00 03 f8  7c  c1  f1 f8 03 fe 00  7f fc 00 3f  f8 0f
//  26,153,225,195,225,199,143,15,248,*3,15,7,193,227,255,64,23,255,0,3,248,124,193,241,248,3,254,0,127,252,0,*3,248,15
//  RRRRRRRRRRR,                 ,HHHHHHHHHHHHHHHHHHHH,      GGGGGGGGGGG      ,XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	9,241,254,3,239,64,23,255,0,3,225,92,53,197,170,53,/*BD*/0, 0, 0, 0, 0, 0/*BD*/,227,142,56,115,207,248,120,195,223,28,241,199,
// 09  f1 fe 03  ef 40 17 ff 00 03 e1 5c 35  c5  aa 35      00 00 00 00 00 00        e3  8e 38  73  cf  f8  78  c3  df 1c  f1  c7
//  9,241,254,3,239,64,23,255,0,3,225,92,53,197,170,53,0,0,0,0,0,0,*62,142,56,115,207,248,120,195,223,28,241,199
//  LLLLLLLLLLLLLLLLL,MMMMMMMMMMMMMMMMMMM,NNNNNNNNNNNNNNNNNNN,PPPPPPPPPPPPPPPPPPPP,RRRRRRRRRRRRRRRRR,ZZZZZZZZZZZZ
	254,3,192,255,0,0,255,248,0,127,248,3,255,240,1,255,224,3,170,202,90,213,33,61,254,31,248,0, 0,31,255,252,32,0,
//  fe 03  c0 ff 00 00 ff f8 00  7f f8 03  ff f0 01  ff e0 03  aa  ca 5a  d5 21 3d  fe 1f f8 00 00 1f  ff  fc 20 00
//	ZZZ,WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW,JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ      ZZZZZZZ
	0,3,255,255,8,121,0,2,255,225,199,31,224,3,255,208,1,255,240,3,127,1,250,95,192,7,248,15,192,255,0,66,/*C*/170,165,
//00 03  ff ff 08  79 0 02 ff  e1  c7 1f e0 03  ff d0 01  ff f0 03 7f 01  fa 5f c0 07  f8 0f  c0 ff 00 42       aa  a5
//  ZZZZZZZZZZZZZ        ZZZZZZ,XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	85,90,170,170,       90,170,85,85,170,170,165,85,170,90,170,165,85,170,170,165,85,170,170,85,165,165,170,165,183,/*C*/102,108,219,
//  55 5a  aa  aa        5a  aa 55 55  aa  aa  a5 55  aa 5a  aa  a5 55  aa  aa  a5 55  aa  aa 55  a5  a5  aa  a5  b7       66  6c  db
//  85,90,170,170,90,165,90,170,85,85,170,170,165,85,170,90,170,165,85,170,170,165,85,170,170,85,165,165,170,165,183,102,108,216
//                           x252x155 (too high volume)                                          TTTTTTTTTTTTTTTTTTTTTT,FFFFFFFFFF
	249,179,108,173,55,55,102,  3, 27,183,246,200,211,182,96,247,110,   77,251,254,93,183,222,70,246,150,180,79,170,169,85,170,165,
//   f9  b3  6c  ad 37 37  66 -fc -9b  b7  f6  c8  d3  b6 60  f7  6e    4d  fb  fe 5d  b7  de 46  f6  96  b4 4f  aa  a9 55  aa  a5
//  249,179,108,173,55,55,102,252,155,135,246,192,211,182,96,247,247,62,77,251,254,93,183,222,70,246,150,180,79,170,169,85,170,170
//      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
	    105,89,154,106,149,85,149,85,106,165,85,169,77,102,106,146,236,165,85,210,150,85,162,186,205,13,102,153,204,103,49,142,102,
//       69 59  9a  6a  95 55  95 55  6a  a5 55  a9 4d  66  6a  92  ec  a5 55  d2  96 55  a2  ba  cd 0d  66  99  cc  67 31  8e  66
//  165,105,89,154,106,149,85,149,85,106,165,85,169,77,102,106,146,236,165,85,210,150,85,162,186,205,*0,102,153,204,103,49,142,102
	57,166,107,25,102,89,198,113,154,103,25,203,26,113,204,115,25,153,204,198,103,25,154,198
//  39  a6  6b 19  66 59  c6  71  9a  67 19  cb 1a  71  cc  73 19  99  cc  c6  67 19  9a  c6
//  57,166,107,25,102,89,198,113,**9,103,25,203,*1,113,204,115,25,153,204,198,103,25,154,198,89
,/*};
unsigned char MISSINGTAB_50831_C68F[] = {*/
//length of samples
// AA
  89,46,90,94,254,26,119,19,35,5,200,195,148,198,0,0
//59 2e 5a 5e  fe 1a  77 13 23 05 c8  c3  94  c6 0 0
// 46 délka áèka
};
unsigned char* TABLE1 = (NumTXT + 58);
unsigned char* TABLE2 = (TABLE1 + 52);
unsigned char* TABLE3 = (TABLE2 + 192);
unsigned char* MISSINGTAB_50831_C68F = (TABLE3 + 258); // 16B
void WAIT_Y(unsigned char*& DE) {
	while(DE--);
}
bool NUMBER(unsigned char& inp) {
	char& A = *(char*)&inp;
	A -= '0';
	if(A < 0) return true;
	if(A < 10) return false;
	A -= 0x11;
	A += 10;
	if(A < 10) return true;
	return !(A<10);
}
static int avg=128;
const unsigned char avgTab[] =
//{1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 17, 18, 19, 20, 21, 22, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 42, 43, 44, 45, 46, 47, 49, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 102, 103, 105, 106, 108, 109, 111, 112, 114, 116, 117, 119, 120, 122, 123, 125, 126, 128, 130, 131, 133, 134, 136, 137, 139, 140, 142, 144, 145, 147, 148, 150, 151, 153, 154, 156, 157, 159, 160, 162, 163, 165, 166, 168, 169, 171, 172, 174, 175, 177, 178, 179, 181, 182, 184, 185, 186, 188, 189, 191, 192, 193, 195, 196, 197, 199, 200, 201, 202, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 234, 235, 236, 237, 238, 239, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 246, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255};
//=SIN((-128+ROW())*3,1415926/256)*127+128
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,18,18,19,19,20,20,21,22,22,23,23,24,25,25,26,27,27,28,29,30,30,31,32,33,34,34,35,36,37,38,39,40,41,42,43,44,44,45,47,48,49,50,51,52,53,54,55,56,58,59,60,61,62,64,65,66,67,69,70,71,73,74,76,77,78,80,81,83,84,86,87,89,91,92,94,95,97,99,100,102,104,105,107,109,110,112,114,116,117,119,121,123,125,126,128,130,132,134,136,138,140,141,143,145,147,149,151,153,155,157,159,161,163,165,166,168,170,172,174,176,178,180,182,184,186,187,189,191,193,195,197,198,200,202,204,206,207,209,211,212,214,216,217,219,220,222,223,225,226,228,229,230,232,233,234,235,237,238,239,240,241,242,243,244,245,246,247,247,248,249,250,250,251,251,252,252,253,253,253,254,254,254,254,254,254,255};
//=CELÁ.ÈÁST((EXP(SIN((-128+ØÁDEK())*3,1415926/256))-0,367907142979737)*254/2,35037468547931+1)
void SAY(unsigned char& A , FILE *f) {
	unsigned char *DE, *HL, *BC, B, C;

	DE=0;

	HL = TABLE1-65;
	HL += A; // letter index normal, accent (+0x1A) to table 2
	BC = TABLE2;
	HL = *HL + BC;
STRANGELOOP:
	A = *HL++;
	C = A;
	C |= *HL & 0x80;
	HL--;
RLCA:
	A = *HL;
	A >>= 5;
	if(!A)
		goto OUTLAST;
	DE = MISSINGTAB_50831_C68F;
	DE = A + DE;
	B = *DE;
	A = *(HL+1);
	if(A > 127) A = (A << 1) + 1;
	else A <<= 1;
	DE = TABLE3 + A;
	printf("TAB3:%i, %c",A, *DE);
	A = 0x80;
OUTER:
	char Avyska = PITCH;
	char step = STEP;
	while(Avyska--) {
		if(PITCH-Avyska < 2) {
			if((A &(*DE))) avg+=step; else avg-=step;
		}
		step = 1;
		if(avg>255) avg=255-VOLUMELIMIT;
		if(avg<0) avg=VOLUMELIMIT;
		fputc(avgTab[avg] + ((A &(*DE))?NOISE:-NOISE),f);
	}
	if(!B--)
		goto OUTLAST;
	if(A & 1) {
		A = (A >> 1) + 0x80;
		DE++;
	} else A >>= 1;
	goto OUTER;
OUTLAST:
	if(DE)
	printf("\nTAB3 end:%i, %c\n",DE-TABLE3, *DE);
	unsigned char *DEwait = (unsigned char *)SPEED;
	for(unsigned i=0;i<(unsigned)DEwait;i+=9) {
		if(avgTab[avg]>128) avg-=STEP; else if(avgTab[avg]<128) avg+=STEP;
		fputc(avgTab[avg],f);
	};
	C--;
	if(A = C & 0xF)
		goto RLCA;
	A = C;
	A <<= 1;
	if(C & 0x80) return;
	HL += 2;
	goto STRANGELOOP;
}
void SAY_IT(const unsigned char *HL, FILE *f) {
	unsigned char A,B = '@',C,D,E;
	unsigned char *DE;
	while(A=B,C=B,B=*HL,A) { // NEXTLETTER / KEEP
		A=C;
		if(A < 'A')
			goto NOLETTER;
		if(A > 'Z')
			goto NOLETTER;
		if(A != 'C') {
			A=B;
			goto NOCH;
		}
		if(B == 'H')
			goto CH;
NOCH:
		A=C;
		if(B != '\'') 
			goto NOACCENT;
CH:
		A+=0x1A;
		B='@';
NOACCENT:
		printf("SAY %c\n",A);
		SAY(A, f);
CONT:
		HL++;
		continue;
NOLETTER:
		if(A==64)
			goto CONT;
		DE=0; D=E=0;
		if(A=='.')
			goto WAITCONT;
		DE=(unsigned char*)0x8000;
		if(A==',')
			goto WAITCONT;
		DE=(unsigned char*)0x4000;
		if(A!=' ')
			goto OTHER;
WAITCONT:
		WAIT_Y(DE);
		goto CONT;
OTHER:
		if(NUMBER(A))
			goto CONT;
		C=A;
		C++;
		unsigned char *HL2 = (unsigned char*)NumTXT;
		do { // LP
			do {
				A=*HL2++;
			} while(A);
		} while (--C);
		DE=const_cast<unsigned char*>(HL);
		SAY_IT(HL2, f);
		goto CONT;
	}
}

int main(array<System::String ^> ^args)
{
	const char *fileName = "Kecal.wav";
	FILE * f = wavfile_open(fileName, BITRATE, 8);
	if(!f) {
		printf("couldn't open %s for writing", fileName);
		return 1;
	}

	unsigned char *HL = (unsigned char*)"JSEM SINKLE'R ZET IKS SPEKTRUM POC'I'TAC' VY SOKY'CH KVALIT ZADEJ SVU'J TEKST";
//	HL= (unsigned char*)"STANISLAV LEM KYBERIA'DA BAJKY KYBERNETICKE'HO VJEKU JAK BYL ZACHRA'NJEN SVJET KONSTRUKTOR TRURL JEDNOU VYVINUL STROJ, KTERY' DOVEDL VYROBIT VS'ECHNO, CO ZAC'I'NALO PI'SMENEM EN. JAKMILE BYL STROJ HOTOV, NAR'I'DIL MU NA ZKOUS'KU VYROBIT NITJE, NACPAT JE DO NA'PRSTKU', KTERE' STROJ ROVNJEZ' ZHOTOVIL, A SPOLU S NIKLA'KY, NABJERA'KY, NOKY A NY'TY NAHA'ZET DO NU'S'E.";
//	HL = (unsigned char*)"ABCDE'FGHIJKLMNOPQRSTUVWXYZ";
	SAY_IT(HL, f);

	wavfile_close(f);
	system(fileName);
    return 0;
}

//wavfile.c
struct wavfile_header {
	char	riff_tag[4];
	int	riff_length;
	char	wave_tag[4];
	char	fmt_tag[4];
	int	fmt_length;
	short	audio_format;
	short	num_channels;
	int	sample_rate;
	int	byte_rate;
	short	block_align;
	short	bits_per_sample;
	char	data_tag[4];
	int	data_length;
};

FILE * wavfile_open( const char *filename, int samples_per_second, int bits_per_sample )
{
	struct wavfile_header header;

	strncpy(header.riff_tag,"RIFF",4);
	strncpy(header.wave_tag,"WAVE",4);
	strncpy(header.fmt_tag,"fmt ",4);
	strncpy(header.data_tag,"data",4);

	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1;
	header.num_channels = 1;
	header.sample_rate = samples_per_second;
	header.byte_rate = samples_per_second*(bits_per_sample/8);
	header.block_align = bits_per_sample/8;
	header.bits_per_sample = bits_per_sample;
	header.data_length = 0;

	FILE * file = fopen(filename,"w+");
	if(!file) return 0;

	fwrite(&header,sizeof(header),1,file);

	fflush(file);

	return file;

}

void wavfile_write( FILE *file, short data[], int length )
{
	fwrite(data,sizeof(short),length,file);
}

void wavfile_close( FILE *file )
{
	int file_length = ftell(file);

	int data_length = file_length - sizeof(struct wavfile_header);
	fseek(file,sizeof(struct wavfile_header) - sizeof(int),SEEK_SET);
	fwrite(&data_length,sizeof(data_length),1,file);

	int riff_length = file_length - 8;
	fseek(file,4,SEEK_SET);
	fwrite(&riff_length,sizeof(riff_length),1,file);

	fclose(file);
}
/* TAB3:
A
3-8
B
50-55
87-92
C
?
135-166
'167-198
D
50-55
193-8
'218-29
'39-44
E
9-14
F
199-230
G
50-55
57-68
H
98-103
45-50
I
15-20
J
116-121
122-127
129-134
K
57-68
L
68-73
99-104
M
98-103
75-80
N
98-103
81-86
O
21-26
P
87-92
Q
56-67
98-103
104-109
111-116
R
32-37
93-98
S
135-166
T
193-8
U
27-32
V
98-103
104-109
111-6
W
98-103
104-109
111-6
X
56-67
135-166
Y
15-20
Z
98-103
134-9
136-41
139-44
*/