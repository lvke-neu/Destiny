#include "Texture.h"
#include "Engine/Utility.h"

namespace Destiny
{
	Texture::Texture() :
		m_resource(nullptr),
		m_shaderResourceView(nullptr)
	{

	}

	Texture::~Texture()
	{
		SAFE_RELEASE(m_resource);
		SAFE_RELEASE(m_shaderResourceView);
	}
}