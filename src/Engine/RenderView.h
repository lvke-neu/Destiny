#pragma once

namespace Destiny
{
	class RenderView
	{
	public:
		RenderView(long long nativeWindow, unsigned int windowWidth, unsigned int windowHeight);
	public:
		long long m_nativeWindow;
		unsigned int m_windowWidth;
		unsigned int m_windowHeight;
	};
}