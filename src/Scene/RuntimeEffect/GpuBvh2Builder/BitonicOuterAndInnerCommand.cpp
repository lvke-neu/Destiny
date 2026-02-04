#include "BitonicOuterAndInnerCommand.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/GraphicsSystem.h"
#include "Engine/Blob.h"
#include <d3d11.h>

namespace Destiny
{
	BitonicOuterAndInnerCommand::BitonicOuterAndInnerCommand() :
		m_cIndirectArgStride(0),
		m_alignedNumElements(0),
		m_outerSortCS(nullptr),
		m_innerSortCS(nullptr),
		m_indirectArgsTexture(nullptr)
	{

	}

	BitonicOuterAndInnerCommand::~BitonicOuterAndInnerCommand()
	{

	}

	void BitonicOuterAndInnerCommand::execute(std::shared_ptr<GraphicsContext> deviceContext)
	{
#if 0
		if (!m_outerSortCS || !m_innerSortCS)
		{
			return;
		}

		uint32_t IndirectArgsOffset = m_cIndirectArgStride;

		for (uint32_t k = 4096; k <= m_alignedNumElements; k *= 2)
		{
			//Engine::GetInstance()->getGraphicsSystem()->beginEvent(L"OuterSortCS");
			for (uint32_t j = k / 2; j >= 2048; j /= 2)
			{
				m_outerSortCS->setConstant("k", k);
				m_outerSortCS->setConstant("j", j);
				m_outerSortCS->setIndirectMode(true, m_indirectArgsTexture, { IndirectArgsOffset });
				m_outerSortCS->execute(deviceContext);
				IndirectArgsOffset += m_cIndirectArgStride;
			}
			//Engine::GetInstance()->getGraphicsSystem()->endEvent();

			//Engine::GetInstance()->getGraphicsSystem()->beginEvent(L"InnerSort");
			m_innerSortCS->setConstant("k", k);
			m_innerSortCS->setIndirectMode(true, m_indirectArgsTexture, { IndirectArgsOffset });
			m_innerSortCS->execute(deviceContext);
			IndirectArgsOffset += m_cIndirectArgStride;
			//Engine::GetInstance()->getGraphicsSystem()->endEvent();
		}
#endif
	}

	void BitonicOuterAndInnerCommand::setParam
	(
		unsigned int cIndirectArgStride,
		unsigned int alignedNumElements,
		std::shared_ptr<ComputerCommand> outerSortCS,
		std::shared_ptr<ComputerCommand> innerSortCS,
		std::shared_ptr<Texture> indirectArgsTexture
	)
	{
		m_cIndirectArgStride = cIndirectArgStride;
		m_alignedNumElements = alignedNumElements;
		m_outerSortCS = outerSortCS;
		m_innerSortCS = innerSortCS;
		m_indirectArgsTexture = indirectArgsTexture;
	}
}
