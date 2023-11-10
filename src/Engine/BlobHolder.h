#pragma once
#include <string>
#include <memory>

namespace Destiny
{
	class Blob;
	class BlobLoader;
	class BlobHolder : public std::enable_shared_from_this<BlobHolder>
	{
	public:
		enum LoadingState
		{
			loading_state_pending,
			loading_state_failed,
			loading_state_succeeded
		};
	public:
		BlobHolder(std::shared_ptr<BlobLoader> blobLoader, const std::string& path);
	public:
		void load(int priority = 1);
		void loadSucceeded__(std::shared_ptr<Blob> blob);
		void loadFailed__();
	public:
		const std::string& getPath() const;
		const std::shared_ptr<Blob>& getBlob() const;
		bool isLoadingSucceed();
	protected:
		std::shared_ptr<Blob> m_blob;
		std::shared_ptr<BlobLoader> m_blobLoader;
		LoadingState m_state;
		std::string m_path;
	};

	inline const std::string& BlobHolder::getPath() const
	{
		return m_path;
	}

	inline const std::shared_ptr<Blob>& BlobHolder::getBlob() const
	{
		return m_blob;
	}

	inline bool BlobHolder::isLoadingSucceed()
	{
		return m_state == loading_state_succeeded;
	}
}