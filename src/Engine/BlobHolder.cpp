#include "BlobHolder.h"
#include "Blob.h"
#include "BlobLoader.h"
#include "ThreadPool.h"
#include "Engine.h"

namespace Destiny
{
	BlobHolder::BlobHolder(std::shared_ptr<BlobLoader> blobLoader, const std::string& path) :
		m_blob(nullptr),
		m_blobLoader(blobLoader),
		m_state(loading_state_pending),
		m_path(path)
	{

	}

	void BlobHolder::load(int priority)
	{
		if (m_blobLoader)
		{
			if (priority == 0)
			{
				m_blobLoader->doLoad(shared_from_this());
			}
			else
			{
				Engine::GetInstance()->getThreadPool()->commitTask(std::bind(&BlobLoader::doLoad, m_blobLoader, shared_from_this()));
			}
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