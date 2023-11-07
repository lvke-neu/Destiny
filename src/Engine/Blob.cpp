#include "Blob.h"

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
}