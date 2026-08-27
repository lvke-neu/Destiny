#pragma once
#include "Object.h"
#include <mutex>

namespace Destiny
{
	class Blob;
	class BlobLoader;
	class BlobHolder : public std::enable_shared_from_this<BlobHolder>, public Object
	{
	public:
		enum LoadingState
		{
			loading_state_pending,
			loading_state_failed,
			loading_state_succeeded
		};
	public:
		BlobHolder();
		BlobHolder(std::shared_ptr<BlobLoader> blobLoader, const std::string& path);
		virtual ~BlobHolder() = default;
	public:
		void							load(int priority = 1);
		void							loadSucceeded__(std::shared_ptr<Blob> blob);
		void							loadFailed__();
	public:
		const std::string&				getPath() const;
		void							setPath(const std::string& path);
		std::shared_ptr<Blob>			getBlob() const;
		std::shared_ptr<BlobLoader>		getBlobLoader() const;
		bool							isLoadingSucceed();
		bool							isLoadingFailed();
		bool							isLoadingPending();
	protected:
		std::shared_ptr<Blob>			m_blob;
		std::shared_ptr<BlobLoader>		m_blobLoader;
		LoadingState					m_state;
		std::string						m_path;
		mutable std::mutex				m_lock;
	};

	inline const std::string& BlobHolder::getPath() const
	{
		return m_path;
	}

	inline void BlobHolder::setPath(const std::string& path)
	{
		m_path = path;
	}

	inline std::shared_ptr<Blob> BlobHolder::getBlob() const
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		return m_blob;
	}

	inline std::shared_ptr<BlobLoader> BlobHolder::getBlobLoader() const
	{
		return m_blobLoader;
	}

	inline bool BlobHolder::isLoadingSucceed()
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		return m_state == loading_state_succeeded;
	}

	inline bool BlobHolder::isLoadingFailed()
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		return m_state == loading_state_failed;
	}

	inline bool BlobHolder::isLoadingPending()
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		return m_state == loading_state_pending;
	}
}
