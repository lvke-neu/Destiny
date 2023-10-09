#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
namespace Destiny
{
	class GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();
	public:
		void initialize();
		void uninitialize();
	private:
		ID3D11Device* m_pD3D11Device;
		ID3D11DeviceContext* m_pD3D11DeviceContext;
		unsigned int m_4xMsaaQuality;
	};
}