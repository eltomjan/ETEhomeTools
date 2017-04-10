#ifndef ARDUINO_ARCH_SAM
#include <avr/io.h>
#endif
#include <avr/pgmspace.h> 
 
#ifndef FONT5X7_H
#define FONT5X7_H

// standard ascii 5x7 font

static const unsigned char font[] PROGMEM = {
0,0,0,0,0,
0x3E	, //	  XXXXX 
0x5B	, //	 X XX XX
0x4F	, //	 X  XXXX
0x5B	, //	 X XX XX
0x3E	, //	  XXXXX 
0x3E	, //	  XXXXX 
0x6B	, //	 XX X XX
0x4F	, //	 X  XXXX
0x6B	, //	 XX X XX
0x3E	, //	  XXXXX 
0x1C	, //	   XXX  
0x3E	, //	  XXXXX 
0x7C	, //	 XXXXX  
0x3E	, //	  XXXXX 
0x1C	, //	   XXX  
0x18	, //	   XX   
0x3C	, //	  XXXX  
0x7E	, //	 XXXXXX 
0x3C	, //	  XXXX  
0x18	, //	   XX   
0x1C	, //	   XXX  
0x57	, //	 X X XXX
0x7D	, //	 XXXXX X
0x57	, //	 X X XXX
0x1C	, //	   XXX  
0x1C	, //	   XXX  
0x5E	, //	 X XXXX 
0x7F	, //	 XXXXXXX
0x5E	, //	 X XXXX 
0x1C	, //	   XXX  
0x00	, //	        
0x18	, //	   XX   
0x3C	, //	  XXXX  
0x18	, //	   XX   
0x00	, //	        
0xFF	, //	XXXXXXXX
0xE7	, //	XXX  XXX
0xC3	, //	XX    XX
0xE7	, //	XXX  XXX
0xFF	, //	XXXXXXXX
0x00	, //	        
0x18	, //	   XX   
0x24	, //	  X  X  
0x18	, //	   XX   
0x00	, //	        
0xFF	, //	XXXXXXXX
0xE7	, //	XXX  XXX
0xDB	, //	XX XX XX
0xE7	, //	XXX  XXX
0xFF	, //	XXXXXXXX
0x30	, //	  XX    
0x48	, //	 X  X   
0x3A	, //	  XXX X 
0x06	, //	     XX 
0x0E	, //	    XXX 
0x26	, //	  X  XX 
0x29	, //	  X X  X
0x79	, //	 XXXX  X
0x29	, //	  X X  X
0x26	, //	  X  XX 
0x40	, //	 X      
0x7F	, //	 XXXXXXX
0x05	, //	     X X
0x05	, //	     X X
0x07	, //	     XXX
0x40	, //	 X      
0x7F	, //	 XXXXXXX
0x05	, //	     X X
0x25	, //	  X  X X
0x3F	, //	  XXXXXX
0x5A	, //	 X XX X 
0x3C	, //	  XXXX  
0xE7	, //	XXX  XXX
0x3C	, //	  XXXX  
0x5A	, //	 X XX X 
0x7F	, //	 XXXXXXX
0x3E	, //	  XXXXX 
0x1C	, //	   XXX  
0x1C	, //	   XXX  
0x08	, //	    X   
0x08	, //	    X   
0x1C	, //	   XXX  
0x1C	, //	   XXX  
0x3E	, //	  XXXXX 
0x7F	, //	 XXXXXXX
0x14	, //	   X X  
0x22	, //	  X   X 
0x7F	, //	 XXXXXXX
0x22	, //	  X   X 
0x14	, //	   X X  
0x5F	, //	 X XXXXX
0x5F	, //	 X XXXXX
0x00	, //	        
0x5F	, //	 X XXXXX
0x5F	, //	 X XXXXX
0x06	, //	     XX 
0x09	, //	    X  X
0x7F	, //	 XXXXXXX
0x01	, //	       X
0x7F	, //	 XXXXXXX
0x00	, //	        
0x66	, //	 XX  XX 
0x89	, //	X   X  X
0x95	, //	X  X X X
0x6A	, //	 XX X X 
0x60	, //	 XX     
0x60	, //	 XX     
0x60	, //	 XX     
0x60	, //	 XX     
0x60	, //	 XX     
0x94	, //	X  X X  
0xA2	, //	X X   X 
0xFF	, //	XXXXXXXX
0xA2	, //	X X   X 
0x94	, //	X  X X  
0x08	, //	    X   
0x04	, //	     X  
0x7E	, //	 XXXXXX 
0x04	, //	     X  
0x08	, //	    X   
0x10	, //	   X    
0x20	, //	  X     
0x7E	, //	 XXXXXX 
0x20	, //	  X     
0x10	, //	   X    
0x08	, //	    X   
0x08	, //	    X   
0x2A	, //	  X X X 
0x1C	, //	   XXX  
0x08	, //	    X   
0x08	, //	    X   
0x1C	, //	   XXX  
0x2A	, //	  X X X 
0x08	, //	    X   
0x08	, //	    X   
0x1E	, //	   XXXX 
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x0C	, //	    XX  
0x1E	, //	   XXXX 
0x0C	, //	    XX  
0x1E	, //	   XXXX 
0x0C	, //	    XX  
0x30	, //	  XX    
0x38	, //	  XXX   
0x3E	, //	  XXXXX 
0x38	, //	  XXX   
0x30	, //	  XX    
0x06	, //	     XX 
0x0E	, //	    XXX 
0x3E	, //	  XXXXX 
0x0E	, //	    XXX 
0x06	, //	     XX 
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, // !	        
0x00	, // !	        
0x00	, // !	        
0x5F	, // !	 X XXXXX
0x00	, // !	        
0x00	, // "	        
0x00	, // "	        
0x07	, // "	     XXX
0x00	, // "	        
0x07	, // "	     XXX
0x00	, // #	        
0x14	, // #	   X X  
0x7F	, // #	 XXXXXXX
0x14	, // #	   X X  
0x7F	, // #	 XXXXXXX
0x14	, // $	   X X  
0x24	, // $	  X  X  
0x2A	, // $	  X X X 
0x7F	, // $	 XXXXXXX
0x2A	, // $	  X X X 
0x12	, // %	   X  X 
0x23	, // %	  X   XX
0x13	, // %	   X  XX
0x08	, // %	    X   
0x64	, // %	 XX  X  
0x62	, // &	 XX   X 
0x36	, // &	  XX XX 
0x49	, // &	 X  X  X
0x56	, // &	 X X XX 
0x20	, // &	  X     
0x50	, // '	 X X    
0x00	, // '	        
0x08	, // '	    X   
0x07	, // '	     XXX
0x03	, // '	      XX
0x00	, // (	        
0x00	, // (	        
0x1C	, // (	   XXX  
0x22	, // (	  X   X 
0x41	, // (	 X     X
0x00	, // )	        
0x00	, // )	        
0x41	, // )	 X     X
0x22	, // )	  X   X 
0x1C	, // )	   XXX  
0x00	, // *	        
0x2A	, // *	  X X X 
0x1C	, // *	   XXX  
0x7F	, // *	 XXXXXXX
0x1C	, // *	   XXX  
0x2A	, // +	  X X X 
0x08	, // +	    X   
0x08	, // +	    X   
0x3E	, // +	  XXXXX 
0x08	, // +	    X   
0x08	, // ,	    X   
0x00	, // ,	        
0x80	, // ,	X       
0x70	, // ,	 XXX    
0x30	, // ,	  XX    
0x00	, // -	        
0x08	, // -	    X   
0x08	, // -	    X   
0x08	, // -	    X   
0x08	, // -	    X   
0x08	, // .	    X   
0x00	, // .	        
0x00	, // .	        
0x60	, // .	 XX     
0x60	, // .	 XX     
0x00	, // /	        
0x20	, // /	  X     
0x10	, // /	   X    
0x08	, // /	    X   
0x04	, // /	     X  
0x02	, // 0	      X 
0x3E	, // 0	  XXXXX 
0x51	, // 0	 X X   X
0x49	, // 0	 X  X  X
0x45	, // 0	 X   X X
0x3E	, // 1	  XXXXX 
0x00	, // 1	        
0x42	, // 1	 X    X 
0x7F	, // 1	 XXXXXXX
0x40	, // 1	 X      
0x00	, // 2	        
0x72	, // 2	 XXX  X 
0x49	, // 2	 X  X  X
0x49	, // 2	 X  X  X
0x49	, // 2	 X  X  X
0x46	, // 3	 X   XX 
0x21	, // 3	  X    X
0x41	, // 3	 X     X
0x49	, // 3	 X  X  X
0x4D	, // 3	 X  XX X
0x33	, // 4	  XX  XX
0x18	, // 4	   XX   
0x14	, // 4	   X X  
0x12	, // 4	   X  X 
0x7F	, // 4	 XXXXXXX
0x10	, // 5	   X    
0x27	, // 5	  X  XXX
0x45	, // 5	 X   X X
0x45	, // 5	 X   X X
0x45	, // 5	 X   X X
0x39	, // 6	  XXX  X
0x3C	, // 6	  XXXX  
0x4A	, // 6	 X  X X 
0x49	, // 6	 X  X  X
0x49	, // 6	 X  X  X
0x31	, // 7	  XX   X
0x41	, // 7	 X     X
0x21	, // 7	  X    X
0x11	, // 7	   X   X
0x09	, // 7	    X  X
0x07	, // 8	     XXX
0x36	, // 8	  XX XX 
0x49	, // 8	 X  X  X
0x49	, // 8	 X  X  X
0x49	, // 8	 X  X  X
0x36	, // 9	  XX XX 
0x46	, // 9	 X   XX 
0x49	, // 9	 X  X  X
0x49	, // 9	 X  X  X
0x29	, // 9	  X X  X
0x1E	, // :	   XXXX 
0x00	, // :	        
0x00	, // :	        
0x14	, // :	   X X  
0x00	, // :	        
0x00	, // ;	        
0x00	, // ;	        
0x40	, // ;	 X      
0x34	, // ;	  XX X  
0x00	, // ;	        
0x00	, // <	        
0x00	, // <	        
0x08	, // <	    X   
0x14	, // <	   X X  
0x22	, // <	  X   X 
0x41	, // =	 X     X
0x14	, // =	   X X  
0x14	, // =	   X X  
0x14	, // =	   X X  
0x14	, // =	   X X  
0x14	, // >	   X X  
0x00	, // >	        
0x41	, // >	 X     X
0x22	, // >	  X   X 
0x14	, // >	   X X  
0x08	, // ?	    X   
0x02	, // ?	      X 
0x01	, // ?	       X
0x59	, // ?	 X XX  X
0x09	, // ?	    X  X
0x06	, // @	     XX 
0x3E	, // @	  XXXXX 
0x41	, // @	 X     X
0x5D	, // @	 X XXX X
0x59	, // @	 X XX  X
0x4E	, // A	 X  XXX 
0x7C	, // A	 XXXXX  
0x12	, // A	   X  X 
0x11	, // A	   X   X
0x12	, // A	   X  X 
0x7C	, // B	 XXXXX  
0x7F	, // B	 XXXXXXX
0x49	, // B	 X  X  X
0x49	, // B	 X  X  X
0x49	, // B	 X  X  X
0x36	, // C	  XX XX 
0x3E	, // C	  XXXXX 
0x41	, // C	 X     X
0x41	, // C	 X     X
0x41	, // C	 X     X
0x22	, // D	  X   X 
0x7F	, // D	 XXXXXXX
0x41	, // D	 X     X
0x41	, // D	 X     X
0x41	, // D	 X     X
0x3E	, // E	  XXXXX 
0x7F	, // E	 XXXXXXX
0x49	, // E	 X  X  X
0x49	, // E	 X  X  X
0x49	, // E	 X  X  X
0x41	, // F	 X     X
0x7F	, // F	 XXXXXXX
0x09	, // F	    X  X
0x09	, // F	    X  X
0x09	, // F	    X  X
0x01	, // G	       X
0x3E	, // G	  XXXXX 
0x41	, // G	 X     X
0x41	, // G	 X     X
0x51	, // G	 X X   X
0x73	, // H	 XXX  XX
0x7F	, // H	 XXXXXXX
0x08	, // H	    X   
0x08	, // H	    X   
0x08	, // H	    X   
0x7F	, // I	 XXXXXXX
0x00	, // I	        
0x41	, // I	 X     X
0x7F	, // I	 XXXXXXX
0x41	, // I	 X     X
0x00	, // J	        
0x20	, // J	  X     
0x40	, // J	 X      
0x41	, // J	 X     X
0x3F	, // J	  XXXXXX
0x01	, // K	       X
0x7F	, // K	 XXXXXXX
0x08	, // K	    X   
0x14	, // K	   X X  
0x22	, // K	  X   X 
0x41	, // L	 X     X
0x7F	, // L	 XXXXXXX
0x40	, // L	 X      
0x40	, // L	 X      
0x40	, // L	 X      
0x40	, // M	 X      
0x7F	, // M	 XXXXXXX
0x02	, // M	      X 
0x1C	, // M	   XXX  
0x02	, // M	      X 
0x7F	, // N	 XXXXXXX
0x7F	, // N	 XXXXXXX
0x04	, // N	     X  
0x08	, // N	    X   
0x10	, // N	   X    
0x7F	, // O	 XXXXXXX
0x3E	, // O	  XXXXX 
0x41	, // O	 X     X
0x41	, // O	 X     X
0x41	, // O	 X     X
0x3E	, // P	  XXXXX 
0x7F	, // P	 XXXXXXX
0x09	, // P	    X  X
0x09	, // P	    X  X
0x09	, // P	    X  X
0x06	, // Q	     XX 
0x3E	, // Q	  XXXXX 
0x41	, // Q	 X     X
0x51	, // Q	 X X   X
0x21	, // Q	  X    X
0x5E	, // R	 X XXXX 
0x7F	, // R	 XXXXXXX
0x09	, // R	    X  X
0x19	, // R	   XX  X
0x29	, // R	  X X  X
0x46	, // S	 X   XX 
0x26	, // S	  X  XX 
0x49	, // S	 X  X  X
0x49	, // S	 X  X  X
0x49	, // S	 X  X  X
0x32	, // T	  XX  X 
0x03	, // T	      XX
0x01	, // T	       X
0x7F	, // T	 XXXXXXX
0x01	, // T	       X
0x03	, // U	      XX
0x3F	, // U	  XXXXXX
0x40	, // U	 X      
0x40	, // U	 X      
0x40	, // U	 X      
0x3F	, // V	  XXXXXX
0x1F	, // V	   XXXXX
0x20	, // V	  X     
0x40	, // V	 X      
0x20	, // V	  X     
0x1F	, // W	   XXXXX
0x3F	, // W	  XXXXXX
0x40	, // W	 X      
0x38	, // W	  XXX   
0x40	, // W	 X      
0x3F	, // X	  XXXXXX
0x63	, // X	 XX   XX
0x14	, // X	   X X  
0x08	, // X	    X   
0x14	, // X	   X X  
0x63	, // Y	 XX   XX
0x03	, // Y	      XX
0x04	, // Y	     X  
0x78	, // Y	 XXXX   
0x04	, // Y	     X  
0x03	, // Z	      XX
0x61	, // Z	 XX    X
0x59	, // Z	 X XX  X
0x49	, // Z	 X  X  X
0x4D	, // Z	 X  XX X
0x43	, // [	 X    XX
0x00	, // [	        
0x7F	, // [	 XXXXXXX
0x41	, // [	 X     X
0x41	, // [	 X     X
0x41	, // \	 X     X
0x02	, // \	      X 
0x04	, // \	     X  
0x08	, // \	    X   
0x10	, // \	   X    
0x20	, // ]	  X     
0x00	, // ]	        
0x41	, // ]	 X     X
0x41	, // ]	 X     X
0x41	, // ]	 X     X
0x7F	, // ^	 XXXXXXX
0x04	, // ^	     X  
0x02	, // ^	      X 
0x01	, // ^	       X
0x02	, // ^	      X 
0x04	, // _	     X  
0x40	, // _	 X      
0x40	, // _	 X      
0x40	, // _	 X      
0x40	, // _	 X      
0x40	, // `	 X      
0x00	, // `	        
0x03	, // `	      XX
0x07	, // `	     XXX
0x08	, // `	    X   
0x00	, // a	        
0x20	, // a	  X     
0x54	, // a	 X X X  
0x54	, // a	 X X X  
0x78	, // a	 XXXX   
0x40	, // b	 X      
0x7F	, // b	 XXXXXXX
0x28	, // b	  X X   
0x44	, // b	 X   X  
0x44	, // b	 X   X  
0x38	, // c	  XXX   
0x38	, // c	  XXX   
0x44	, // c	 X   X  
0x44	, // c	 X   X  
0x44	, // c	 X   X  
0x28	, // d	  X X   
0x38	, // d	  XXX   
0x44	, // d	 X   X  
0x44	, // d	 X   X  
0x28	, // d	  X X   
0x7F	, // e	 XXXXXXX
0x38	, // e	  XXX   
0x54	, // e	 X X X  
0x54	, // e	 X X X  
0x54	, // e	 X X X  
0x18	, // f	   XX   
0x00	, // f	        
0x08	, // f	    X   
0x7E	, // f	 XXXXXX 
0x09	, // f	    X  X
0x02	, // g	      X 
0x18	, // g	   XX   
0xA4	, // g	X X  X  
0xA4	, // g	X X  X  
0x9C	, // g	X  XXX  
0x78	, // h	 XXXX   
0x7F	, // h	 XXXXXXX
0x08	, // h	    X   
0x04	, // h	     X  
0x04	, // h	     X  
0x78	, // i	 XXXX   
0x00	, // i	        
0x44	, // i	 X   X  
0x7D	, // i	 XXXXX X
0x40	, // i	 X      
0x00	, // j	        
0x20	, // j	  X     
0x40	, // j	 X      
0x40	, // j	 X      
0x3D	, // j	  XXXX X
0x00	, // k	        
0x7F	, // k	 XXXXXXX
0x10	, // k	   X    
0x28	, // k	  X X   
0x44	, // k	 X   X  
0x00	, // l	        
0x00	, // l	        
0x41	, // l	 X     X
0x7F	, // l	 XXXXXXX
0x40	, // l	 X      
0x00	, // m	        
0x7C	, // m	 XXXXX  
0x04	, // m	     X  
0x78	, // m	 XXXX   
0x04	, // m	     X  
0x78	, // n	 XXXX   
0x7C	, // n	 XXXXX  
0x08	, // n	    X   
0x04	, // n	     X  
0x04	, // n	     X  
0x78	, // o	 XXXX   
0x38	, // o	  XXX   
0x44	, // o	 X   X  
0x44	, // o	 X   X  
0x44	, // o	 X   X  
0x38	, // p	  XXX   
0xFC	, // p	XXXXXX  
0x18	, // p	   XX   
0x24	, // p	  X  X  
0x24	, // p	  X  X  
0x18	, // q	   XX   
0x18	, // q	   XX   
0x24	, // q	  X  X  
0x24	, // q	  X  X  
0x18	, // q	   XX   
0xFC	, // r	XXXXXX  
0x7C	, // r	 XXXXX  
0x08	, // r	    X   
0x04	, // r	     X  
0x04	, // r	     X  
0x08	, // s	    X   
0x48	, // s	 X  X   
0x54	, // s	 X X X  
0x54	, // s	 X X X  
0x54	, // s	 X X X  
0x24	, // t	  X  X  
0x04	, // t	     X  
0x04	, // t	     X  
0x3F	, // t	  XXXXXX
0x44	, // t	 X   X  
0x24	, // u	  X  X  
0x3C	, // u	  XXXX  
0x40	, // u	 X      
0x40	, // u	 X      
0x20	, // u	  X     
0x7C	, // v	 XXXXX  
0x1C	, // v	   XXX  
0x20	, // v	  X     
0x40	, // v	 X      
0x20	, // v	  X     
0x1C	, // w	   XXX  
0x3C	, // w	  XXXX  
0x40	, // w	 X      
0x30	, // w	  XX    
0x40	, // w	 X      
0x3C	, // x	  XXXX  
0x44	, // x	 X   X  
0x28	, // x	  X X   
0x10	, // x	   X    
0x28	, // x	  X X   
0x44	, // y	 X   X  
0x4C	, // y	 X  XX  
0x90	, // y	X  X    
0x90	, // y	X  X    
0x90	, // y	X  X    
0x7C	, // z	 XXXXX  
0x44	, // z	 X   X  
0x64	, // z	 XX  X  
0x54	, // z	 X X X  
0x4C	, // z	 X  XX  
0x44	, // {	 X   X  
0x00	, // {	        
0x08	, // {	    X   
0x36	, // {	  XX XX 
0x41	, // {	 X     X
0x00	, // |	        
0x00	, // |	        
0x00	, // |	        
0x77	, // |	 XXX XXX
0x00	, // |	        
0x00	, // }	        
0x00	, // }	        
0x41	, // }	 X     X
0x36	, // }	  XX XX 
0x08	, // }	    X   
0x00	, // ~	        
0x02	, // ~	      X 
0x01	, // ~	       X
0x02	, // ~	      X 
0x04	, // ~	     X  
0x02	, //	      X 
0x3C	, //	  XXXX  
0x26	, //	  X  XX 
0x23	, //	  X   XX
0x26	, //	  X  XX 
0x3C	, //	  XXXX  
0x1E	, //	   XXXX 
0xA1	, //	X X    X
0xA1	, //	X X    X
0x61	, //	 XX    X
0x12	, //	   X  X 
0x3A	, //	  XXX X 
0x40	, //	 X      
0x40	, //	 X      
0x20	, //	  X     
0x7A	, //	 XXXX X 
0x38	, //	  XXX   
0x54	, //	 X X X  
0x54	, //	 X X X  
0x55	, //	 X X X X
0x59	, //	 X XX  X
0x21	, //	  X    X
0x55	, //	 X X X X
0x55	, //	 X X X X
0x79	, //	 XXXX  X
0x41	, //	 X     X
0x22	, //	  X   X 
0x54	, //	 X X X  
0x54	, //	 X X X  
0x78	, //	 XXXX   
0x42	, //	 X    X 
0x21	, //	  X    X
0x55	, //	 X X X X
0x54	, //	 X X X  
0x78	, //	 XXXX   
0x40	, //	 X      
0x20	, //	  X     
0x54	, //	 X X X  
0x55	, //	 X X X X
0x79	, //	 XXXX  X
0x40	, //	 X      
0x0C	, //	    XX  
0x1E	, //	   XXXX 
0x52	, //	 X X  X 
0x72	, //	 XXX  X 
0x12	, //	   X  X 
0x39	, //	  XXX  X
0x55	, //	 X X X X
0x55	, //	 X X X X
0x55	, //	 X X X X
0x59	, //	 X XX  X
0x39	, //	  XXX  X
0x54	, //	 X X X  
0x54	, //	 X X X  
0x54	, //	 X X X  
0x59	, //	 X XX  X
0x39	, //	  XXX  X
0x55	, //	 X X X X
0x54	, //	 X X X  
0x54	, //	 X X X  
0x58	, //	 X XX   
0x00	, //	        
0x00	, //	        
0x45	, //	 X   X X
0x7C	, //	 XXXXX  
0x41	, //	 X     X
0x00	, //	        
0x02	, //	      X 
0x45	, //	 X   X X
0x7D	, //	 XXXXX X
0x42	, //	 X    X 
0x00	, //	        
0x01	, //	       X
0x45	, //	 X   X X
0x7C	, //	 XXXXX  
0x40	, //	 X      
0x7D	, //	 XXXXX X
0x12	, //	   X  X 
0x11	, //	   X   X
0x12	, //	   X  X 
0x7D	, //	 XXXXX X
0xF0	, //	XXXX    
0x28	, //	  X X   
0x25	, //	  X  X X
0x28	, //	  X X   
0xF0	, //	XXXX    
0x7C	, //	 XXXXX  
0x54	, //	 X X X  
0x55	, //	 X X X X
0x45	, //	 X   X X
0x00	, //	        
0x20	, //	  X     
0x54	, //	 X X X  
0x54	, //	 X X X  
0x7C	, //	 XXXXX  
0x54	, //	 X X X  
0x7C	, //	 XXXXX  
0x0A	, //	    X X 
0x09	, //	    X  X
0x7F	, //	 XXXXXXX
0x49	, //	 X  X  X
0x32	, //	  XX  X 
0x49	, //	 X  X  X
0x49	, //	 X  X  X
0x49	, //	 X  X  X
0x32	, //	  XX  X 
0x3A	, //	  XXX X 
0x44	, //	 X   X  
0x44	, //	 X   X  
0x44	, //	 X   X  
0x3A	, //	  XXX X 
0x32	, //	  XX  X 
0x4A	, //	 X  X X 
0x48	, //	 X  X   
0x48	, //	 X  X   
0x30	, //	  XX    
0x3A	, //	  XXX X 
0x41	, //	 X     X
0x41	, //	 X     X
0x21	, //	  X    X
0x7A	, //	 XXXX X 
0x3A	, //	  XXX X 
0x42	, //	 X    X 
0x40	, //	 X      
0x20	, //	  X     
0x78	, //	 XXXX   
0x00	, //	        
0x9D	, //	X  XXX X
0xA0	, //	X X     
0xA0	, //	X X     
0x7D	, //	 XXXXX X
0x3D	, //	  XXXX X
0x42	, //	 X    X 
0x42	, //	 X    X 
0x42	, //	 X    X 
0x3D	, //	  XXXX X
0x3D	, //	  XXXX X
0x40	, //	 X      
0x40	, //	 X      
0x40	, //	 X      
0x3D	, //	  XXXX X
0x3C	, //	  XXXX  
0x24	, //	  X  X  
0xFF	, //	XXXXXXXX
0x24	, //	  X  X  
0x24	, //	  X  X  
0x48	, //	 X  X   
0x7E	, //	 XXXXXX 
0x49	, //	 X  X  X
0x43	, //	 X    XX
0x66	, //	 XX  XX 
0x2B	, //	  X X XX
0x2F	, //	  X XXXX
0xFC	, //	XXXXXX  
0x2F	, //	  X XXXX
0x2B	, //	  X X XX
0xFF	, //	XXXXXXXX
0x09	, //	    X  X
0x29	, //	  X X  X
0xF6	, //	XXXX XX 
0x20	, //	  X     
0xC0	, //	XX      
0x88	, //	X   X   
0x7E	, //	 XXXXXX 
0x09	, //	    X  X
0x03	, //	      XX
0x20	, //	  X     
0x54	, //	 X X X  
0x54	, //	 X X X  
0x79	, //	 XXXX  X
0x41	, //	 X     X
0x00	, //	        
0x00	, //	        
0x44	, //	 X   X  
0x7D	, //	 XXXXX X
0x41	, //	 X     X
0x30	, //	  XX    
0x48	, //	 X  X   
0x48	, //	 X  X   
0x4A	, //	 X  X X 
0x32	, //	  XX  X 
0x38	, //	  XXX   
0x40	, //	 X      
0x40	, //	 X      
0x22	, //	  X   X 
0x7A	, //	 XXXX X 
0x00	, //	        
0x7A	, //	 XXXX X 
0x0A	, //	    X X 
0x0A	, //	    X X 
0x72	, //	 XXX  X 
0x7D	, //	 XXXXX X
0x0D	, //	    XX X
0x19	, //	   XX  X
0x31	, //	  XX   X
0x7D	, //	 XXXXX X
0x26	, //	  X  XX 
0x29	, //	  X X  X
0x29	, //	  X X  X
0x2F	, //	  X XXXX
0x28	, //	  X X   
0x26	, //	  X  XX 
0x29	, //	  X X  X
0x29	, //	  X X  X
0x29	, //	  X X  X
0x26	, //	  X  XX 
0x30	, //	  XX    
0x48	, //	 X  X   
0x4D	, //	 X  XX X
0x40	, //	 X      
0x20	, //	  X     
0x38	, //	  XXX   
0x08	, //	    X   
0x08	, //	    X   
0x08	, //	    X   
0x08	, //	    X   
0x08	, //	    X   
0x08	, //	    X   
0x08	, //	    X   
0x08	, //	    X   
0x38	, //	  XXX   
0x2F	, //	  X XXXX
0x10	, //	   X    
0xC8	, //	XX  X   
0xAC	, //	X X XX  
0xBA	, //	X XXX X 
0x2F	, //	  X XXXX
0x10	, //	   X    
0x28	, //	  X X   
0x34	, //	  XX X  
0xFA	, //	XXXXX X 
0x00	, //	        
0x00	, //	        
0x7B	, //	 XXXX XX
0x00	, //	        
0x00	, //	        
0x08	, //	    X   
0x14	, //	   X X  
0x2A	, //	  X X X 
0x14	, //	   X X  
0x22	, //	  X   X 
0x22	, //	  X   X 
0x14	, //	   X X  
0x2A	, //	  X X X 
0x14	, //	   X X  
0x08	, //	    X   
0xAA	, //	X X X X 
0x00	, //	        
0x55	, //	 X X X X
0x00	, //	        
0xAA	, //	X X X X 
0xAA	, //	X X X X 
0x55	, //	 X X X X
0xAA	, //	X X X X 
0x55	, //	 X X X X
0xAA	, //	X X X X 
0x00	, //	        
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x00	, //	        
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0xFF	, //	XXXXXXXX
0x00	, //	        
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0xFF	, //	XXXXXXXX
0x00	, //	        
0x10	, //	   X    
0x10	, //	   X    
0xFF	, //	XXXXXXXX
0x00	, //	        
0xFF	, //	XXXXXXXX
0x10	, //	   X    
0x10	, //	   X    
0xF0	, //	XXXX    
0x10	, //	   X    
0xF0	, //	XXXX    
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0xFC	, //	XXXXXX  
0x00	, //	        
0x14	, //	   X X  
0x14	, //	   X X  
0xF7	, //	XXXX XXX
0x00	, //	        
0xFF	, //	XXXXXXXX
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x00	, //	        
0xFF	, //	XXXXXXXX
0x14	, //	   X X  
0x14	, //	   X X  
0xF4	, //	XXXX X  
0x04	, //	     X  
0xFC	, //	XXXXXX  
0x14	, //	   X X  
0x14	, //	   X X  
0x17	, //	   X XXX
0x10	, //	   X    
0x1F	, //	   XXXXX
0x10	, //	   X    
0x10	, //	   X    
0x1F	, //	   XXXXX
0x10	, //	   X    
0x1F	, //	   XXXXX
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0x1F	, //	   XXXXX
0x00	, //	        
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0xF0	, //	XXXX    
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x1F	, //	   XXXXX
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x1F	, //	   XXXXX
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0xF0	, //	XXXX    
0x10	, //	   X    
0x00	, //	        
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0xFF	, //	XXXXXXXX
0x10	, //	   X    
0x00	, //	        
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x14	, //	   X X  
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x00	, //	        
0xFF	, //	XXXXXXXX
0x00	, //	        
0x00	, //	        
0x1F	, //	   XXXXX
0x10	, //	   X    
0x17	, //	   X XXX
0x00	, //	        
0x00	, //	        
0xFC	, //	XXXXXX  
0x04	, //	     X  
0xF4	, //	XXXX X  
0x14	, //	   X X  
0x14	, //	   X X  
0x17	, //	   X XXX
0x10	, //	   X    
0x17	, //	   X XXX
0x14	, //	   X X  
0x14	, //	   X X  
0xF4	, //	XXXX X  
0x04	, //	     X  
0xF4	, //	XXXX X  
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x00	, //	        
0xF7	, //	XXXX XXX
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0xF7	, //	XXXX XXX
0x00	, //	        
0xF7	, //	XXXX XXX
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0x17	, //	   X XXX
0x14	, //	   X X  
0x10	, //	   X    
0x10	, //	   X    
0x1F	, //	   XXXXX
0x10	, //	   X    
0x1F	, //	   XXXXX
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0xF4	, //	XXXX X  
0x14	, //	   X X  
0x10	, //	   X    
0x10	, //	   X    
0xF0	, //	XXXX    
0x10	, //	   X    
0xF0	, //	XXXX    
0x00	, //	        
0x00	, //	        
0x1F	, //	   XXXXX
0x10	, //	   X    
0x1F	, //	   XXXXX
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x1F	, //	   XXXXX
0x14	, //	   X X  
0x00	, //	        
0x00	, //	        
0x00	, //	        
0xFC	, //	XXXXXX  
0x14	, //	   X X  
0x00	, //	        
0x00	, //	        
0xF0	, //	XXXX    
0x10	, //	   X    
0xF0	, //	XXXX    
0x10	, //	   X    
0x10	, //	   X    
0xFF	, //	XXXXXXXX
0x10	, //	   X    
0xFF	, //	XXXXXXXX
0x14	, //	   X X  
0x14	, //	   X X  
0x14	, //	   X X  
0xFF	, //	XXXXXXXX
0x14	, //	   X X  
0x10	, //	   X    
0x10	, //	   X    
0x10	, //	   X    
0x1F	, //	   XXXXX
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, //	        
0xF0	, //	XXXX    
0x10	, //	   X    
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0xF0	, //	XXXX    
0xF0	, //	XXXX    
0xF0	, //	XXXX    
0xF0	, //	XXXX    
0xF0	, //	XXXX    
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0xFF	, //	XXXXXXXX
0x0F	, //	    XXXX
0x0F	, //	    XXXX
0x0F	, //	    XXXX
0x0F	, //	    XXXX
0x0F	, //	    XXXX
0x38	, //	  XXX   
0x44	, //	 X   X  
0x44	, //	 X   X  
0x38	, //	  XXX   
0x44	, //	 X   X  
0xFC	, //	XXXXXX  
0x4A	, //	 X  X X 
0x4A	, //	 X  X X 
0x4A	, //	 X  X X 
0x34	, //	  XX X  
0x7E	, //	 XXXXXX 
0x02	, //	      X 
0x02	, //	      X 
0x06	, //	     XX 
0x06	, //	     XX 
0x02	, //	      X 
0x7E	, //	 XXXXXX 
0x02	, //	      X 
0x7E	, //	 XXXXXX 
0x02	, //	      X 
0x63	, //	 XX   XX
0x55	, //	 X X X X
0x49	, //	 X  X  X
0x41	, //	 X     X
0x63	, //	 XX   XX
0x38	, //	  XXX   
0x44	, //	 X   X  
0x44	, //	 X   X  
0x3C	, //	  XXXX  
0x04	, //	     X  
0x40	, //	 X      
0x7E	, //	 XXXXXX 
0x20	, //	  X     
0x1E	, //	   XXXX 
0x20	, //	  X     
0x06	, //	     XX 
0x02	, //	      X 
0x7E	, //	 XXXXXX 
0x02	, //	      X 
0x02	, //	      X 
0x99	, //	X  XX  X
0xA5	, //	X X  X X
0xE7	, //	XXX  XXX
0xA5	, //	X X  X X
0x99	, //	X  XX  X
0x1C	, //	   XXX  
0x2A	, //	  X X X 
0x49	, //	 X  X  X
0x2A	, //	  X X X 
0x1C	, //	   XXX  
0x4C	, //	 X  XX  
0x72	, //	 XXX  X 
0x01	, //	       X
0x72	, //	 XXX  X 
0x4C	, //	 X  XX  
0x30	, //	  XX    
0x4A	, //	 X  X X 
0x4D	, //	 X  XX X
0x4D	, //	 X  XX X
0x30	, //	  XX    
0x30	, //	  XX    
0x48	, //	 X  X   
0x78	, //	 XXXX   
0x48	, //	 X  X   
0x30	, //	  XX    
0xBC	, //	X XXXX  
0x62	, //	 XX   X 
0x5A	, //	 X XX X 
0x46	, //	 X   XX 
0x3D	, //	  XXXX X
0x3E	, //	  XXXXX 
0x49	, //	 X  X  X
0x49	, //	 X  X  X
0x49	, //	 X  X  X
0x00	, //	        
0x7E	, //	 XXXXXX 
0x01	, //	       X
0x01	, //	       X
0x01	, //	       X
0x7E	, //	 XXXXXX 
0x2A	, //	  X X X 
0x2A	, //	  X X X 
0x2A	, //	  X X X 
0x2A	, //	  X X X 
0x2A	, //	  X X X 
0x44	, //	 X   X  
0x44	, //	 X   X  
0x5F	, //	 X XXXXX
0x44	, //	 X   X  
0x44	, //	 X   X  
0x40	, //	 X      
0x51	, //	 X X   X
0x4A	, //	 X  X X 
0x44	, //	 X   X  
0x40	, //	 X      
0x40	, //	 X      
0x44	, //	 X   X  
0x4A	, //	 X  X X 
0x51	, //	 X X   X
0x40	, //	 X      
0x00	, //	        
0x00	, //	        
0xFF	, //	XXXXXXXX
0x01	, //	       X
0x03	, //	      XX
0xE0	, //	XXX     
0x80	, //	X       
0xFF	, //	XXXXXXXX
0x00	, //	        
0x00	, //	        
0x08	, //	    X   
0x08	, //	    X   
0x6B	, //	 XX X XX
0x6B	, //	 XX X XX
0x08	, //	    X   
0x36	, //	  XX XX 
0x12	, //	   X  X 
0x36	, //	  XX XX 
0x24	, //	  X  X  
0x36	, //	  XX XX 
0x06	, //	     XX 
0x0F	, //	    XXXX
0x09	, //	    X  X
0x0F	, //	    XXXX
0x06	, //	     XX 
0x00	, //	        
0x00	, //	        
0x18	, //	   XX   
0x18	, //	   XX   
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x10	, //	   X    
0x10	, //	   X    
0x00	, //	        
0x30	, //	  XX    
0x40	, //	 X      
0xFF	, //	XXXXXXXX
0x01	, //	       X
0x01	, //	       X
0x00	, //	        
0x1F	, //	   XXXXX
0x01	, //	       X
0x01	, //	       X
0x1E	, //	   XXXX 
0x00	, //	        
0x19	, //	   XX  X
0x1D	, //	   XXX X
0x17	, //	   X XXX
0x12	, //	   X  X 
0x00	, //	        
0x3C	, //	  XXXX  
0x3C	, //	  XXXX  
0x3C	, //	  XXXX  
0x3C	, //	  XXXX  
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	, //	        
0x00	  //	        
};
#endif // FONT5X7_H
