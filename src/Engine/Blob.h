#pragma once

namespace Destiny
{
	class Blob
	{ 
	public:
		Blob(size_t length);
		~Blob();
	public:
		void*	getData();
		size_t	getLength();
	public:
		void	copyfrom(void* data, size_t length);
		void	memset0();
	private:
		void*	m_data{ nullptr };
		size_t	m_length{ 0 };
	};

	inline void* Blob::getData()
	{
		return m_data;
	}

	inline size_t Blob::getLength()
	{
		return m_length;
	}
}