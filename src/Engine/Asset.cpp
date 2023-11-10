#include "Asset.h"

namespace Destiny
{
	Asset::Asset() : 
		m_assetLoader(nullptr),
		m_blobHolder(nullptr),
		m_state(loading_state_pending)
	{

	}
}