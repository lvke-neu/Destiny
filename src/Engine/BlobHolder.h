#pragma once
#include "Object.h"
#include <string>
#include <memory>

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
		BlobHolder() = default;
		BlobHolder(std::shared_ptr<BlobLoader> blobLoader, const std::string& path);
		virtual ~BlobHolder() = default;
	public:
		void					load(int priority = 1);
		void					loadSucceeded__(std::shared_ptr<Blob> blob);
		void					loadFailed__();
	public:
		const std::string&		getPath() const;
		void					setPath(const std::string& path);
		const std::string&		getFullPath() const;
		void					setFullPath(const std::string& fullPath);
		std::shared_ptr<Blob>	getBlob() const;
		bool					isLoadingSucceed();
		bool					isLoadingFailed();
		bool					isLoadingPending();
	protected:
		std::shared_ptr<Blob>			m_blob;
		std::shared_ptr<BlobLoader>		m_blobLoader;
		LoadingState					m_state;
		std::string						m_path;
		std::string						m_fullPath;
	};

	inline const std::string& BlobHolder::getPath() const
	{
		return m_path;
	}

	inline void BlobHolder::setPath(const std::string& path)
	{
		m_path = path;
	}

	inline const std::string& BlobHolder::getFullPath() const
	{
		return m_fullPath;
	}

	inline void BlobHolder::setFullPath(const std::string& fullPath)
	{
		m_fullPath = fullPath;
	}

	inline std::shared_ptr<Blob> BlobHolder::getBlob() const
	{
		return m_blob;
	}

	inline bool BlobHolder::isLoadingSucceed()
	{
		return m_state == loading_state_succeeded;
	}

	inline bool BlobHolder::isLoadingFailed()
	{
		return m_state == loading_state_failed;
	}

	inline bool BlobHolder::isLoadingPending()
	{
		return m_state == loading_state_pending;
	}
}