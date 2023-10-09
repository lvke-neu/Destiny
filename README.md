# Destiny
learn directx11 and cmake
https://www.cnblogs.com/X-Jun/p/9028764.html
https://www.bilibili.com/video/BV15j411b7F8/?spm_id_from=333.337.search-card.all.click&vd_source=358b090c831d6b487c6e4d997109e415


cmake -G "Visual Studio 15 2017 Win64" ..

DX11简单理解学习:
	1、纹理是一种资源，在创建时设置纹理的绑定标志值（D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE），通过创建纹理对应的资源视图（ID3D11RenderTargetView，ID3D11ShaderResourceView），将纹理绑定到渲染管线的不同阶段。
	2、	获取后备缓冲区的纹理，为纹理创建ID3D11RenderTargetView，绑定该ID3D11RenderTargetView到渲染管线。Device/DeviceContext ----->DXGI_SWAP_CHAIN----->Window