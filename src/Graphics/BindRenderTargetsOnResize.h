#pragma once
#include "BindRenderTargets.h"

namespace Destiny
{
	class BindRenderTargetsOnResize : public BindRenderTargets
	{
	public:
		BindRenderTargetsOnResize();
		virtual ~BindRenderTargetsOnResize();
	private:
		void onResize(void* data);
	};
}