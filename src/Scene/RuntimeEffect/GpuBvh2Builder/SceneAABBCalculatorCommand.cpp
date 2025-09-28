#include "SceneAABBCalculatorCommand.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/GraphicsSystem.h"
#include "Engine/Blob.h"
#include "Graphics/Texture.h"
#include "StructDefine.h"
#include "Math/Math.h"
#include <d3d11.h>

namespace Destiny
{
	SceneAABBCalculatorCommand::SceneAABBCalculatorCommand() :
		m_numElements(0),
		m_calculateSceneAABBFromBVHs(nullptr),
		m_calculateSceneAABBFromAABBs(nullptr)
	{
		m_scratchBuffers[0] = nullptr;
		m_scratchBuffers[1] = nullptr;
	}

	SceneAABBCalculatorCommand::~SceneAABBCalculatorCommand()
	{

	}

	void SceneAABBCalculatorCommand::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!m_calculateSceneAABBFromBVHs || !m_calculateSceneAABBFromAABBs || !m_scratchBuffers[0] || !m_scratchBuffers[1])
		{
			return;
		}

		std::vector<std::shared_ptr<Texture>> uavs;
		uavs.resize(2);
		uavs[0] = m_aabbBuffer;



		UINT outputScratchBufferIndex = 0;
		UINT inputScratchBufferIndex = 1;
		UINT threadsNeeded;
		UINT elementsToProcess = m_numElements;
		bool bCalculateSceneAABBFromAABB = false;

		for (; elementsToProcess > 1; elementsToProcess = threadsNeeded)
		{
			threadsNeeded = Math::GetNumAABBsOutputFromPass(elementsToProcess, ElementsSummedPerThread);

			const bool bFirstPass = (elementsToProcess == m_numElements);
			const bool bLastPass = (threadsNeeded == 1);

			m_calculateSceneAABBFromBVHs->setConstant("Constants", elementsToProcess);
			m_calculateSceneAABBFromAABBs->setConstant("Constants", elementsToProcess);
		
			uavs[1] = bLastPass? m_outputBuffer: m_scratchBuffers[outputScratchBufferIndex];
			if (!bFirstPass)
			{
				uavs[0] = m_scratchBuffers[inputScratchBufferIndex];
			}

			m_calculateSceneAABBFromBVHs->setUnorderedAccessViews(uavs, {0,0});
			m_calculateSceneAABBFromAABBs->setUnorderedAccessViews(uavs, { 0,0 });

			const UINT dispatchWidth = Math::DivideAndRoundUp<UINT>(threadsNeeded, THREAD_GROUP_1D_WIDTH);
			m_calculateSceneAABBFromBVHs->setThreadGroupCount(dispatchWidth, 1, 1);
			m_calculateSceneAABBFromAABBs->setThreadGroupCount(dispatchWidth, 1, 1);
			
			if (!bCalculateSceneAABBFromAABB)
			{
				m_calculateSceneAABBFromBVHs->execute(deviceContext);
			}
			else
			{
				if (!bFirstPass)
				{
					m_calculateSceneAABBFromAABBs->execute(deviceContext);
				}
			}
			
			std::swap(outputScratchBufferIndex, inputScratchBufferIndex);
			if (bFirstPass)
			{
				m_calculateSceneAABBFromAABBs->execute(deviceContext);
				bCalculateSceneAABBFromAABB = true;
			}
		}
	}

	void SceneAABBCalculatorCommand::setParam(unsigned int numElements, std::shared_ptr<Texture> aabbBuffer, std::shared_ptr<Texture> outputBuffer, std::shared_ptr<ComputerCommand> calculateSceneAABBFromBVHs, std::shared_ptr<ComputerCommand> calculateSceneAABBFromAABBs)
	{
		m_numElements = numElements;
		m_aabbBuffer = aabbBuffer;
		m_outputBuffer = outputBuffer;
		m_calculateSceneAABBFromBVHs = calculateSceneAABBFromBVHs;
		m_calculateSceneAABBFromAABBs = calculateSceneAABBFromAABBs;
		
		m_scratchBuffers[0] = Texture::CreateStructured(sizeof(AABB), sizeof(AABB) * m_numElements);
		m_scratchBuffers[0]->load();

		m_scratchBuffers[1] = Texture::CreateStructured(sizeof(AABB), sizeof(AABB) * m_numElements);
		m_scratchBuffers[1]->load();
	}
}
