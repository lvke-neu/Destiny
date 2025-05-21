# Destiny
learn directx11 and cmake
https://www.cnblogs.com/X-Jun/p/9028764.html
https://www.bilibili.com/video/BV15j411b7F8/?spm_id_from=333.337.search-card.all.click&vd_source=358b090c831d6b487c6e4d997109e415


cmake -G "Visual Studio 15 2017 Win64" ..

DX11简单理解学习:
	1、纹理是一种资源，在创建时设置纹理的绑定标志值（D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE），通过创建纹理对应的资源视图（ID3D11RenderTargetView，ID3D11ShaderResourceView），将纹理绑定到渲染管线的不同阶段。
	2、	获取后备缓冲区的纹理，为纹理创建ID3D11RenderTargetView，绑定该ID3D11RenderTargetView到渲染管线。Device/DeviceContext ----->DXGI_SWAP_CHAIN----->Window
	3、各种状态，采样状态、光栅化状态、混合状态、深度模板状态


简单烟花粒子效果：https://www.cnblogs.com/graphics/archive/2012/07/09/2570431.html



简单测试了实例化绘制与视锥体裁剪，视锥体裁剪确实能裁剪不少visual，但是本身裁剪耗费cpu时间较多，虽然会把大多数visual裁剪掉但是帧数没有太大的提升，
需要学习一下加速结构，加速裁剪(八叉树？)。

多线程不阻塞主线程，提高加载任务的速度