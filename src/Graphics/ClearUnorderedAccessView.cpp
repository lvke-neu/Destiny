#include "ClearUnorderedAccessView.h"
#include "Graphics/Texture.h"
#include <d3d11.h>

namespace Destiny
{
	ClearUnorderedAccessView::ClearUnorderedAccessView() :
		m_clearType(ClearType::None),
		m_texture(nullptr)
	{

	}

	ClearUnorderedAccessView::~ClearUnorderedAccessView()
	{
		
	}

	void ClearUnorderedAccessView::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!deviceContext || !m_texture)
		{
			return;
		}

		auto uav = *m_texture->getUnorderedAccessView();
		if (!uav)
		{
			return;
		}

		float clearValuesF[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT clearValuesU[4] = { 0, 0, 0, 0 };

		if (m_clearType == ClearType::ClearFloat)
		{
			deviceContext->ClearUnorderedAccessViewFloat(uav, clearValuesF);
		}
		else if(m_clearType == ClearType::ClearUint)
		{
			deviceContext->ClearUnorderedAccessViewUint(uav, clearValuesU);
		}
	}

	void ClearUnorderedAccessView::setParam(ClearType clearType, std::shared_ptr<Texture> texture)
	{
		m_clearType = clearType;
		m_texture = texture;
	}
}