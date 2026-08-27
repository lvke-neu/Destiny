#pragma once
#include <windows.h>

struct MD5CheckSum
{
	unsigned long state[4];
public:
	MD5CheckSum& operator = (const MD5CheckSum& arg)
	{
		if (this != &arg)
		{
			state[0] = arg.state[0];
			state[1] = arg.state[1];
			state[2] = arg.state[2];
			state[3] = arg.state[3];
		}
		return *this;
	}
	bool operator == (const MD5CheckSum& arg) const
	{
		return (state[0] == arg.state[0] && state[1] == arg.state[1] && state[2] == arg.state[2] && state[3] == arg.state[3]);
	}
	bool operator != (const MD5CheckSum& arg) const
	{
		return (state[0] != arg.state[0] || state[1] != arg.state[1] || state[2] != arg.state[2] || state[3] != arg.state[3]);
	}
	bool operator < (const MD5CheckSum& arg) const
	{
		if (state[0] != arg.state[0])
			return (state[0] < arg.state[0]);
		if (state[1] != arg.state[1])
			return (state[1] < arg.state[1]);
		if (state[2] != arg.state[2])
			return (state[2] < arg.state[2]);
		if (state[3] != arg.state[3])
			return (state[3] < arg.state[3]);
		return false;
	}
	bool operator > (const MD5CheckSum& arg) const
	{
		if (state[0] != arg.state[0])
			return (state[0] > arg.state[0]);
		if (state[1] != arg.state[1])
			return (state[1] > arg.state[1]);
		if (state[2] != arg.state[2])
			return (state[2] > arg.state[2]);
		if (state[3] != arg.state[3])
			return (state[3] > arg.state[3]);
		return false;
	}
};

void   MD5(MD5CheckSum& md5CheckSum, const void* pBuffer, unsigned long long bufferSize);
void   MD5ToString(char buffer[33], const MD5CheckSum& md5CheckSum);
int   COMPARE_MD5(const MD5CheckSum& code1, const MD5CheckSum& code2);