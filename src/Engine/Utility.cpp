#include "Utility.h"
#include <combaseapi.h>

namespace Destiny
{
	std::wstring Utility::MultiByte2WideChar(const std::string& pKey)
	{
		const char* pCStrKey = pKey.c_str();

		int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, NULL, 0);
		wchar_t* pWCStrKey = new wchar_t[pSize];

		MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, pWCStrKey, pSize);

		std::wstring wstr(pWCStrKey);
		delete[] pWCStrKey;
		return wstr;
	}

	std::string Utility::WideChar2MultiByte(const std::wstring& pWCStrKey)
	{

		int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey.c_str(), (int)wcslen(pWCStrKey.c_str()), NULL, 0, NULL, NULL);
		char* pCStrKey = new char[pSize + 1];

		WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey.c_str(), (int)wcslen(pWCStrKey.c_str()), pCStrKey, pSize, NULL, NULL);
		pCStrKey[pSize] = '\0';

		std::string str(pCStrKey);
		delete[] pCStrKey;
		return str;
	}

	std::string Utility::GenerateUUID()
	{
		GUID guid;
		CoCreateGuid(&guid);
		const UINT bufferLength = 256;
		char* guidStr = new char[bufferLength];
		_snprintf_s(guidStr, bufferLength, bufferLength - 1,
			"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2],
			guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		std::string str(guidStr);
		delete[] guidStr;
		return str;
	}
}

