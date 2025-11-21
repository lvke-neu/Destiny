#include "Md5.h"
const unsigned long MD5_T01 = 0xd76aa478;
const unsigned long MD5_T02 = 0xe8c7b756;
const unsigned long MD5_T03 = 0x242070db;
const unsigned long MD5_T04 = 0xc1bdceee;
const unsigned long MD5_T05 = 0xf57c0faf;
const unsigned long MD5_T06 = 0x4787c62a;
const unsigned long MD5_T07 = 0xa8304613;
const unsigned long MD5_T08 = 0xfd469501;
const unsigned long MD5_T09 = 0x698098d8;
const unsigned long MD5_T10 = 0x8b44f7af;
const unsigned long MD5_T11 = 0xffff5bb1;
const unsigned long MD5_T12 = 0x895cd7be;
const unsigned long MD5_T13 = 0x6b901122;
const unsigned long MD5_T14 = 0xfd987193;
const unsigned long MD5_T15 = 0xa679438e;
const unsigned long MD5_T16 = 0x49b40821;

const unsigned long MD5_T17 = 0xf61e2562;
const unsigned long MD5_T18 = 0xc040b340;
const unsigned long MD5_T19 = 0x265e5a51;
const unsigned long MD5_T20 = 0xe9b6c7aa;
const unsigned long MD5_T21 = 0xd62f105d;
const unsigned long MD5_T22 = 0x02441453;
const unsigned long MD5_T23 = 0xd8a1e681;
const unsigned long MD5_T24 = 0xe7d3fbc8;
const unsigned long MD5_T25 = 0x21e1cde6;
const unsigned long MD5_T26 = 0xc33707d6;
const unsigned long MD5_T27 = 0xf4d50d87;
const unsigned long MD5_T28 = 0x455a14ed;
const unsigned long MD5_T29 = 0xa9e3e905;
const unsigned long MD5_T30 = 0xfcefa3f8;
const unsigned long MD5_T31 = 0x676f02d9;
const unsigned long MD5_T32 = 0x8d2a4c8a;

const unsigned long MD5_T33 = 0xfffa3942;
const unsigned long MD5_T34 = 0x8771f681;
const unsigned long MD5_T35 = 0x6d9d6122;
const unsigned long MD5_T36 = 0xfde5380c;
const unsigned long MD5_T37 = 0xa4beea44;
const unsigned long MD5_T38 = 0x4bdecfa9;
const unsigned long MD5_T39 = 0xf6bb4b60;
const unsigned long MD5_T40 = 0xbebfbc70;
const unsigned long MD5_T41 = 0x289b7ec6;
const unsigned long MD5_T42 = 0xeaa127fa;
const unsigned long MD5_T43 = 0xd4ef3085;
const unsigned long MD5_T44 = 0x04881d05;
const unsigned long MD5_T45 = 0xd9d4d039;
const unsigned long MD5_T46 = 0xe6db99e5;
const unsigned long MD5_T47 = 0x1fa27cf8;
const unsigned long MD5_T48 = 0xc4ac5665;

const unsigned long MD5_T49 = 0xf4292244;
const unsigned long MD5_T50 = 0x432aff97;
const unsigned long MD5_T51 = 0xab9423a7;
const unsigned long MD5_T52 = 0xfc93a039;
const unsigned long MD5_T53 = 0x655b59c3;
const unsigned long MD5_T54 = 0x8f0ccc92;
const unsigned long MD5_T55 = 0xffeff47d;
const unsigned long MD5_T56 = 0x85845dd1;
const unsigned long MD5_T57 = 0x6fa87e4f;
const unsigned long MD5_T58 = 0xfe2ce6e0;
const unsigned long MD5_T59 = 0xa3014314;
const unsigned long MD5_T60 = 0x4e0811a1;
const unsigned long MD5_T61 = 0xf7537e82;
const unsigned long MD5_T62 = 0xbd3af235;
const unsigned long MD5_T63 = 0x2ad7d2bb;
const unsigned long MD5_T64 = 0xeb86d391;

inline void MD5_Init(MD5CheckSum& md5CheckSum)
{
	md5CheckSum.state[0] = 0x67452301;
	md5CheckSum.state[1] = 0xefcdab89;
	md5CheckSum.state[2] = 0x98badcfe;
	md5CheckSum.state[3] = 0x10325476;
}

inline unsigned long MD5_F(unsigned long b, unsigned long c, unsigned long d)
{
	return ((b & c) | (~b & d));
}

inline unsigned long MD5_G(unsigned long b, unsigned long c, unsigned long d)
{
	return ((b & d) | (c & ~d));
}

inline unsigned long MD5_H(unsigned long b, unsigned long c, unsigned long d)
{
	return (b ^ c ^ d);
}

inline unsigned long MD5_I(unsigned long b, unsigned long c, unsigned long d)
{
	return (c ^ (b | ~d));
}

inline unsigned long MD5_CYCLIC(unsigned long w, unsigned long s)
{
	return (w << s) | (w >> (32 - s));
}

inline void MD5_FF(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long k, unsigned long s, unsigned long t)
{
	a = b + MD5_CYCLIC((a + MD5_F(b, c, d) + k + t), s);
}

inline void MD5_GG(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long k, unsigned long s, unsigned long t)
{
	a = b + MD5_CYCLIC((a + MD5_G(b, c, d) + k + t), s);
}

inline void MD5_HH(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long k, unsigned long s, unsigned long t)
{
	a = b + MD5_CYCLIC((a + MD5_H(b, c, d) + k + t), s);
}

inline void MD5_II(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long k, unsigned long s, unsigned long t)
{
	a = b + MD5_CYCLIC((a + MD5_I(b, c, d) + k + t), s);
}

inline void MD5_Packet(MD5CheckSum& md5CheckSum, const unsigned long* pPacket)
{
	unsigned long a = md5CheckSum.state[0];
	unsigned long b = md5CheckSum.state[1];
	unsigned long c = md5CheckSum.state[2];
	unsigned long d = md5CheckSum.state[3];

	MD5_FF(a, b, c, d, pPacket[0],   7, MD5_T01);
	MD5_FF(d, a, b, c, pPacket[1],  12, MD5_T02);
	MD5_FF(c, d, a, b, pPacket[2],  17, MD5_T03);
	MD5_FF(b, c, d, a, pPacket[3],  22, MD5_T04);
	MD5_FF(a, b, c, d, pPacket[4],   7, MD5_T05);
	MD5_FF(d, a, b, c, pPacket[5],  12, MD5_T06);
	MD5_FF(c, d, a, b, pPacket[6],  17, MD5_T07);
	MD5_FF(b, c, d, a, pPacket[7],  22, MD5_T08);
	MD5_FF(a, b, c, d, pPacket[8],   7, MD5_T09);
	MD5_FF(d, a, b, c, pPacket[9],  12, MD5_T10);
	MD5_FF(c, d, a, b, pPacket[10], 17, MD5_T11);
	MD5_FF(b, c, d, a, pPacket[11], 22, MD5_T12);
	MD5_FF(a, b, c, d, pPacket[12],  7, MD5_T13);
	MD5_FF(d, a, b, c, pPacket[13], 12, MD5_T14);
	MD5_FF(c, d, a, b, pPacket[14], 17, MD5_T15);
	MD5_FF(b, c, d, a, pPacket[15], 22, MD5_T16);

	MD5_GG(a, b, c, d, pPacket[1],   5, MD5_T17);
	MD5_GG(d, a, b, c, pPacket[6],   9, MD5_T18);
	MD5_GG(c, d, a, b, pPacket[11], 14, MD5_T19);
	MD5_GG(b, c, d, a, pPacket[0],  20, MD5_T20);
	MD5_GG(a, b, c, d, pPacket[5],   5, MD5_T21);
	MD5_GG(d, a, b, c, pPacket[10],  9, MD5_T22);
	MD5_GG(c, d, a, b, pPacket[15], 14, MD5_T23);
	MD5_GG(b, c, d, a, pPacket[4],  20, MD5_T24);
	MD5_GG(a, b, c, d, pPacket[9],   5, MD5_T25);
	MD5_GG(d, a, b, c, pPacket[14],  9, MD5_T26);
	MD5_GG(c, d, a, b, pPacket[3],  14, MD5_T27);
	MD5_GG(b, c, d, a, pPacket[8],  20, MD5_T28);
	MD5_GG(a, b, c, d, pPacket[13],  5, MD5_T29);
	MD5_GG(d, a, b, c, pPacket[2],   9, MD5_T30);
	MD5_GG(c, d, a, b, pPacket[7],  14, MD5_T31);
	MD5_GG(b, c, d, a, pPacket[12], 20, MD5_T32);

	MD5_HH(a, b, c, d, pPacket[5],   4, MD5_T33);
	MD5_HH(d, a, b, c, pPacket[8],  11, MD5_T34);
	MD5_HH(c, d, a, b, pPacket[11], 16, MD5_T35);
	MD5_HH(b, c, d, a, pPacket[14], 23, MD5_T36);
	MD5_HH(a, b, c, d, pPacket[1],   4, MD5_T37);
	MD5_HH(d, a, b, c, pPacket[4],  11, MD5_T38);
	MD5_HH(c, d, a, b, pPacket[7],  16, MD5_T39);
	MD5_HH(b, c, d, a, pPacket[10], 23, MD5_T40);
	MD5_HH(a, b, c, d, pPacket[13],  4, MD5_T41);
	MD5_HH(d, a, b, c, pPacket[0],  11, MD5_T42);
	MD5_HH(c, d, a, b, pPacket[3],  16, MD5_T43);
	MD5_HH(b, c, d, a, pPacket[6],  23, MD5_T44);
	MD5_HH(a, b, c, d, pPacket[9],   4, MD5_T45);
	MD5_HH(d, a, b, c, pPacket[12], 11, MD5_T46);
	MD5_HH(c, d, a, b, pPacket[15], 16, MD5_T47);
	MD5_HH(b, c, d, a, pPacket[2],  23, MD5_T48);

	MD5_II(a, b, c, d, pPacket[0],   6, MD5_T49);
	MD5_II(d, a, b, c, pPacket[7],  10, MD5_T50);
	MD5_II(c, d, a, b, pPacket[14], 15, MD5_T51);
	MD5_II(b, c, d, a, pPacket[5],  21, MD5_T52);
	MD5_II(a, b, c, d, pPacket[12],  6, MD5_T53);
	MD5_II(d, a, b, c, pPacket[3],  10, MD5_T54);
	MD5_II(c, d, a, b, pPacket[10], 15, MD5_T55);
	MD5_II(b, c, d, a, pPacket[1],  21, MD5_T56);
	MD5_II(a, b, c, d, pPacket[8],   6, MD5_T57);
	MD5_II(d, a, b, c, pPacket[15], 10, MD5_T58);
	MD5_II(c, d, a, b, pPacket[6],  15, MD5_T59);
	MD5_II(b, c, d, a, pPacket[13], 21, MD5_T60);
	MD5_II(a, b, c, d, pPacket[4],   6, MD5_T61);
	MD5_II(d, a, b, c, pPacket[11], 10, MD5_T62);
	MD5_II(c, d, a, b, pPacket[2],  15, MD5_T63);
	MD5_II(b, c, d, a, pPacket[9],  21, MD5_T64);

	md5CheckSum.state[0] += a;
	md5CheckSum.state[1] += b;
	md5CheckSum.state[2] += c;
	md5CheckSum.state[3] += d;
}

void MD5_Block(MD5CheckSum& md5CheckSum, const void* pBuffer, size_t bufferSize)
{
	byte* pSrc = (byte*)pBuffer;
	byte packet[64];
	size_t count = bufferSize/64;
	for(size_t i = 0; i < count; ++i)
	{
		memcpy(packet, pSrc, 64);
		MD5_Packet(md5CheckSum, (unsigned long*)packet);
		pSrc += 64;
	}
}

void MD5_RemainderBlock(MD5CheckSum& md5CheckSum, const void* pBuffer, size_t bufferSize)
{
	byte packet[64];
	size_t remainder = bufferSize % 64;	
	byte* pSrc = (byte*)pBuffer + bufferSize - remainder;

	if(remainder < 56)
	{
		memcpy(packet, pSrc, remainder);
		packet[remainder] = 0x80;
		memset(&packet[remainder+1], 0, 56 - remainder - 1);
		//rsize_t len = 56 - remainder - 1;
		//memset(&packet[remainder + 1], len,0);
		*((__int64*)&packet[56]) = bufferSize << 3;
		MD5_Packet(md5CheckSum, (unsigned long*)packet);
	}
	else
	{
		memcpy(packet, pSrc, remainder);
		packet[remainder] = 0x80;
		memset(&packet[remainder+1], 0, 64 - remainder - 1);
		//rsize_t len = 64 - remainder - 1;
		//memset(&packet[remainder + 1], len, 0);
		MD5_Packet(md5CheckSum, (unsigned long*)packet);
		memset(packet, 56, 56);		
		*((__int64*)&packet[56]) = bufferSize << 3;
		MD5_Packet(md5CheckSum, (unsigned long*)packet);
	}
}


void MD5(MD5CheckSum& md5CheckSum, const void* pBuffer, unsigned long long bufferSize)
{
	MD5_Init(md5CheckSum);
	MD5_Block(md5CheckSum, pBuffer, bufferSize);
	MD5_RemainderBlock(md5CheckSum, pBuffer, bufferSize);
}


void MD5ToString(char buffer[33], const MD5CheckSum& md5CheckSum)
{
	byte* pMd5 = (byte*)&md5CheckSum;
	for( int i = 0; i < 16; ++i)
	{
		byte highHalf = pMd5[i] >> 4;
		if(highHalf < 10)
		{
			buffer[i*2] = highHalf + '0';
		}
		else
		{
			buffer[i*2] = highHalf - 10 + 'a';
		}
		byte lowHalf = pMd5[i] & 0xF;
		if(lowHalf < 10)
		{
			buffer[i*2 + 1] = lowHalf + '0';
		}
		else
		{
			buffer[i*2 + 1] = lowHalf - 10 + 'a';
		}
	}
	buffer[32] = '\0';
}

int COMPARE_MD5( const MD5CheckSum& code1, const MD5CheckSum& code2 )
{
	if ( code1.state[0] > code2.state[0] )
		return 1;
	else if ( code1.state[0] < code2.state[0] )
		return -1;

	//	 state[0]
	if ( code1.state[1] > code2.state[1] )
		return 1;
	else if ( code1.state[1] < code2.state[1] )
		return -1;

	//	 state[0], state[1]
	if ( code1.state[2] > code2.state[2] )
		return 1;
	else if ( code1.state[2] < code2.state[2] )
		return -1;

	//	 state[0], state[1], state[2]
	if ( code1.state[3] > code2.state[3] )
		return 1;
	else if ( code1.state[3] < code2.state[3] )
		return -1;

	return 0;
}
