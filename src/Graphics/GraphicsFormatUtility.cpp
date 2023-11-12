#include "GraphicsFormatUtility.h"


namespace Destiny
{
	std::string GraphicsFormatUtility::DXGI_FORMAT_TO_STRING(DXGI_FORMAT Format)
	{
		switch (Format)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return "DXGI_FORMAT_R32G32B32A32_FLOAT";
		case DXGI_FORMAT_R32G32B32_FLOAT:
			return "DXGI_FORMAT_R32G32B32_FLOAT";
		case DXGI_FORMAT_R32G32_FLOAT:
			return "DXGI_FORMAT_R32G32_FLOAT";
		case DXGI_FORMAT_R32_FLOAT:
			return "DXGI_FORMAT_R32_FLOAT";

		}
		return "";
	}

	std::string GraphicsFormatUtility::D3D11_INPUT_CLASSIFICATION_TO_STRING(D3D11_INPUT_CLASSIFICATION Format)
	{
		switch (Format)
		{
		case D3D11_INPUT_PER_VERTEX_DATA:
			return "D3D11_INPUT_PER_VERTEX_DATA";
		case D3D11_INPUT_PER_INSTANCE_DATA:
			return "D3D11_INPUT_PER_INSTANCE_DATA";

		}
		return "";
	}

}


