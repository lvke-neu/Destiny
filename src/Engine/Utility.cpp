#include "Utility.h"

wchar_t* multiByteToWideChar(const std::string& pKey)
{
	const char* pCStrKey = pKey.c_str();

	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, NULL, 0);
	wchar_t* pWCStrKey = new wchar_t[pSize];

	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}


char* wideCharToMultiByte(const wchar_t* pWCStrKey)
{

	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, (int)wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];

	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, (int)wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;

}