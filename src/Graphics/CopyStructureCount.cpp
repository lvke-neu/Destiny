#include "CopyStructureCount.h"
#include "Graphics/Texture.h"
#include <d3d11.h>

namespace Destiny
{
	CopyStructureCount::CopyStructureCount() :
		m_dstTexture(nullptr),
		m_srcTexture(nullptr)
	{

	}

	void CopyStructureCount::setDstAndSrcTexture(std::shared_ptr<Texture> dstTexture, std::shared_ptr<Texture> srcTexture)
	{
		if (!dstTexture || !srcTexture)
		{
			return;
		}

		auto creationParamDst = std::static_pointer_cast<TextureCreationParam>(dstTexture->getCreationParam());
		if (!creationParamDst || creationParamDst->m_type != TextureCreationParam::CreateTextureType::CreateBuffer)
		{
			return;
		}

		auto creationParamSrc = std::static_pointer_cast<TextureCreationParam>(srcTexture->getCreationParam());
		if (!creationParamSrc ||
			creationParamSrc->m_type < TextureCreationParam::CreateTextureType::CreateStructured ||
			creationParamSrc->m_type > TextureCreationParam::CreateTextureType::CreateRaw)
		{
			return;
		}

		m_dstTexture = dstTexture;
		m_srcTexture = srcTexture;
	}

	void CopyStructureCount::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!m_dstTexture || !m_srcTexture)
		{
			return;
		}

		ID3D11Buffer* pDstBuffer = (ID3D11Buffer*)m_dstTexture->getResource();
		ID3D11UnorderedAccessView* pSrcView = *m_srcTexture->getUnorderedAccessView();

		deviceContext->CopyStructureCount(pDstBuffer, 0, pSrcView);
	}
}