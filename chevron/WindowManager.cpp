#include "WindowManager.h"

#include <vector>
#include <algorithm>

static std::vector<HWND> g_ownedWindows;

void WindowManager::RegisterWindow(HWND window)
{
	g_ownedWindows.push_back(window);
}

void WindowManager::DeregisterWindow(HWND window)
{
	g_ownedWindows.erase(std::remove(g_ownedWindows.begin(), g_ownedWindows.end(), window), g_ownedWindows.end());
}

bool WindowManager::IsOwnedWindow(HWND window)
{
	return std::find(g_ownedWindows.begin(), g_ownedWindows.end(), window) != g_ownedWindows.end();
}

BOOL CALLBACK windowCallbackMinimizeAllThirdPartyWindows(HWND hWnd, LPARAM lParam) {
	if (!WindowManager::IsOwnedWindow(hWnd) && IsWindowVisible(hWnd)) {
		PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	return TRUE;
}


void WindowManager::MinimizeAllThirdPartyWindows()
{
	EnumWindows(windowCallbackMinimizeAllThirdPartyWindows, 0);
}
