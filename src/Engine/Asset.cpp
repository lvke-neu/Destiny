#include "Asset.h"
#include "VirtualDestructor.h"

namespace Destiny 
{
	Asset::Asset(std::shared_ptr<VirtualDestructor> creationParam) :
		m_creationParam(creationParam),
		m_state(loading_state_pending)
	{

	}

}