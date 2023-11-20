#pragma once
#include <windows.h>

/**
 * The window manager class is a special type I've introduced that simply keeps track of
 * all open windows our program has created, for the purpose of doing fuckery to other windows.
 */
class WindowManager
{
public:
	static void RegisterWindow(HWND window);
	static void DeregisterWindow(HWND window);
	static bool IsOwnedWindow(HWND window);
	static void MinimizeAllThirdPartyWindows();
};
