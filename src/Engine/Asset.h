#pragma once
#include <memory>

namespace Destiny
{
	class VirtualDestructor;
	class Asset
	{
	public:
		enum LoadingState
		{
			loading_state_pending,
			loading_state_failed,
			loading_state_succeeded
		};
	public:
		Asset(std::shared_ptr<VirtualDestructor> creationParam);
		virtual ~Asset() = default;
	private:
		std::shared_ptr<VirtualDestructor> m_creationParam;
		LoadingState m_state;
	};
}