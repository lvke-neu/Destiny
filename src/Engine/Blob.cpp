#include "Blob.h"
#include <Windows.h>

namespace Destiny
{
	Blob::Blob(size_t length)
	{
		m_data = new char[length];
		m_length = length;
	}

	Blob::~Blob()
	{
		delete[] m_data;
	}

	void Blob::copyfrom(void* data, size_t length)
	{
		if (length <= m_length)
		{
			memcpy_s(m_data, length, data, length);
		}
	}
}