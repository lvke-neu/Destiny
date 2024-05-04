#pragma once
#include "Engine/Asset.h"

namespace Destiny
{
	class Blob;
	class Renderer;
	class RenderStates;
	class RenderParameters;
	class Effect : public Asset
	{
	public:
		Effect(const char* path);
		virtual ~Effect();
	public:
		virtual void doLoad() override;
	public:
		void fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters);
		std::shared_ptr<Blob> getInputSignatureBlob();
	private:
		std::shared_ptr<Renderer>		m_renderer;
		std::shared_ptr<RenderStates>	m_renderStates;
	};
}