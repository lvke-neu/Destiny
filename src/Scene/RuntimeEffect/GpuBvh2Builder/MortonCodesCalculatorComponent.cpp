#include "MortonCodesCalculatorComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/Texture.h"
#include "Graphics/SamplerState.h"
#include "Graphics/ComputerCommand.h"
#include "Graphics/RenderSystem.h"
#include "Math/Math.h"
#include "SceneAABBCalculatorCommand.h"
#include "StructDefine.h"
#include <numeric> 
#include <random>

namespace Destiny
{
	MortonCodesCalculatorComponent::MortonCodesCalculatorComponent() :
		m_calcuateMortonCodesForAABBs(nullptr),
		m_graphicsCommandList(std::make_shared<GraphicsCommandList>())
	{
		calcuateMortonCodesForAABBsCS();

		m_graphicsCommandList->addGraphicsCommand(m_calcuateMortonCodesForAABBs);
		std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem())->addBeforePipelineCommandList(L"MortonCodesCalculate", m_graphicsCommandList);
	}

	void MortonCodesCalculatorComponent::calcuateMortonCodesForAABBsCS()
	{
		m_calcuateMortonCodesForAABBs = std::make_shared<ComputerCommand>();
		m_calcuateMortonCodesForAABBs->setDebugName(L"CalcuateMortonCodesForAABBs");
		m_calcuateMortonCodesForAABBs->setComputerEffectPath("builtin://renderer/GpuBvh2Builder/morton_codes_calculator/CalculateMortonCodesForAABBs.hlsl");
	
		m_calcuateMortonCodesForAABBs->setThreadGroupCount(1, 2, 3);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<MortonCodesCalculatorComponent>("MortonCodesCalculatorComponent")
			.constructor<>();
	}
}