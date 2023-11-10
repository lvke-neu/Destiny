#pragma once
#include <memory>

namespace Destiny
{
	class AssetLoader;
	class BlobHolder;
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
		Asset();
		virtual ~Asset() = default;
	private:
		std::shared_ptr<AssetLoader> m_assetLoader;
		std::shared_ptr<BlobHolder> m_blobHolder;
		LoadingState m_state;
	};
}