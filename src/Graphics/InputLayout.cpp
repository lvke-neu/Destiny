#include "InputLayout.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{
	InputLayout::InputLayout() :
		m_inputLayout(nullptr)
	{

	}

	InputLayout::~InputLayout()
	{
		SAFE_RELEASE(m_inputLayout);
	}

	void InputLayout::doLoad()
	{
	}
}