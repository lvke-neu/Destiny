#include "BlobHolder.h"
#include "Blob.h"
#include "BlobLoader.h"
#include "ThreadPool.h"

namespace Destiny
{
	BlobHolder::BlobHolder(std::shared_ptr<BlobLoader> blobLoader, const std::string& path) :
		m_blob(nullptr),
		m_blobLoader(blobLoader),
		m_state(loading_state_pending),
		m_path(path)
	{
		auto pos = m_path.find("://");
		if (pos != m_path.npos)
		{
			m_path = m_path.substr(pos + 3);
		}
	}

	void BlobHolder::load()
	{
		if (m_blobLoader)
		{
			m_blobLoader->doLoad(shared_from_this());
		}
		else
		{
			loadFailed__();
		}
	}

	void BlobHolder::loadSucceeded__(std::shared_ptr<Blob> blob)
	{
		m_blob.reset();
		m_blob = blob;
		m_state = loading_state_succeeded;
	}

	void BlobHolder::loadFailed__()
	{
		m_blob.reset();
		m_state = loading_state_failed;
	}
}