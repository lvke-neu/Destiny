#pragma once

namespace Destiny
{
	class Blob
	{ 
	public:
		Blob(unsigned long long length);
		~Blob();
	public:
		void*	getData();
		unsigned long long	getLength();
	public:
		void	copyfrom(void* data, unsigned long long length);
		void	memset0();
	private:
		void*	m_data{ nullptr };
		unsigned long long	m_length{ 0 };
	};

	inline void* Blob::getData()
	{
		return m_data;
	}

	inline unsigned long long Blob::getLength()
	{
		return m_length;
	}
}