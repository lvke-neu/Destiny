#pragma once
#include "Engine/Asset.h"

struct ID3D11InputLayout;
namespace Destiny
{
	class Blob;
	class InputLayout : public Asset
	{
	public:
		InputLayout(std::shared_ptr<Blob> inputElements);
		virtual ~InputLayout();
	public:
		virtual void doLoad() override;
	private:
		ID3D11InputLayout* m_inputLayout;
		std::shared_ptr<Blob> m_inputElements;
	};
}