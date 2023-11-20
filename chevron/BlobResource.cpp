#define _CRT_SECURE_NO_WARNINGS

#include "BlobResource.h"

#include <cstdio>

#include "FakeConsoleWindow.h"

BlobResource BlobResource::LoadResourceById(int resourceId, const wchar_t* lpType)
{
	const HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceId), lpType);
	const DWORD dResourceSize = SizeofResource(NULL, hResource);
	const HGLOBAL hgResource = LoadResource(NULL, hResource);
	const LPVOID pResourceData = LockResource(hgResource);

	return {hgResource, pResourceData, dResourceSize};
}

BlobResource::BlobResource(HGLOBAL hgHandle, LPVOID lpData, DWORD dwSize) : hgHandle(hgHandle), lpData(lpData), dwSize(dwSize)
{
}

bool BlobResource::WriteToFile(const char* pPath) const
{
	FILE* fWallpaperFile = fopen(pPath, "wb");
	if (!fWallpaperFile) return false;
	
	fwrite(lpData, 1, dwSize, fWallpaperFile);
	fclose(fWallpaperFile);

	return true;
}
