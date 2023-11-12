#pragma once
#include <string>
#include <d3d11.h>

namespace Destiny
{
	class GraphicsFormatUtility
	{
	public:
		static std::string DXGI_FORMAT_TO_STRING(DXGI_FORMAT Format);
		static std::string D3D11_INPUT_CLASSIFICATION_TO_STRING(D3D11_INPUT_CLASSIFICATION Format);
	};	
}