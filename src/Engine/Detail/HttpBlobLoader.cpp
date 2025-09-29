#include "HttpBlobLoader.h"
#include "../BlobHolder.h"
#include "../Blob.h"
#include "curl/curl.h"
#include <fstream>

namespace Destiny
{
	HttpBlobLoader::HttpBlobLoader(const std::string& storagePath) :
		BlobLoader(storagePath)
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

	HttpBlobLoader::~HttpBlobLoader()
	{
		curl_global_cleanup();
	}

	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) 
	{
		std::string* str = (std::string*)userp;
		size_t totalSize = size * nmemb;
		str->append((char*)contents, totalSize);
		return totalSize;
	}

	size_t HeaderCallback(void* contents, size_t size, size_t nmemb, void* userp) 
	{
		std::string* headerStr = (std::string*)userp;
		size_t totalSize = size * nmemb;
		headerStr->append((char*)contents, totalSize);
		return totalSize;
	}

	void HttpBlobLoader::doLoad(std::shared_ptr<BlobHolder> blobHolder)
	{
		if (!blobHolder)
		{
			return;
		}

		auto url = normalizedPath(blobHolder);

		CURL* curl = nullptr;
		CURLcode curlCode = CURLcode::CURL_LAST;
		std::string curlErrBuf(CURL_ERROR_SIZE, '\0');

		std::string readBuffer = "";
		std::string responseHeader = "";

		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrBuf.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeader);

		curlCode = curl_easy_perform(curl);

		if (curlCode != CURLcode::CURLE_OK)
		{
			blobHolder->loadFailed__();
			LOG_ERROR("Thread {0}, HttpBlobLoader failed : {1}, {2}", std::to_string((*(uint32_t*)&std::this_thread::get_id())), blobHolder->getPath(), curlErrBuf);
			return;
		}

		std::shared_ptr<Blob> blob = std::make_shared<Blob>(readBuffer.size());
		blob->copyfrom((char*)readBuffer.c_str(), blob->getLength());

		blobHolder->loadSucceeded__(blob);
	}

	std::string HttpBlobLoader::normalizedPath(std::shared_ptr<BlobHolder> blobHolder)
	{
		if (!blobHolder)
		{
			return "";
		}

		auto path = blobHolder->getPath();
		auto pos = path.find("://");
		if (pos != path.npos)
		{
			path = path.substr(pos + 3);

			return path;
		}

		return "";
	}
}