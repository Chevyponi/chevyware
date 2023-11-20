#pragma once
#include <windows.h>

class BlobResource
{
public:
	static BlobResource LoadResourceById(int resourceId, const wchar_t* lpType);

	bool WriteToFile(const char* pPath) const;

private:
	HGLOBAL hgHandle;
	LPVOID lpData;
	DWORD dwSize;

	BlobResource(HGLOBAL hgHandle, LPVOID lpData, DWORD dwSize);

};
