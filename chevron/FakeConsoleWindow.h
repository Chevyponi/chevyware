#pragma once
#include <mutex>
#include <string>
#include <windows.h>


class FakeConsoleWindow
{
	HWND m_hWnd;
	
public:
	FakeConsoleWindow();
	~FakeConsoleWindow();

	std::string text = "";
	std::mutex text_lock;
	
	void Print(const std::string& newText);

	void QuitEverything() const;
};
