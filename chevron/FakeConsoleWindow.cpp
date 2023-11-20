#include "FakeConsoleWindow.h"

#include <windows.h>

#include "WindowManager.h"

#define WM_LOW_TIER_GOD WM_USER

static bool wndClassRegistered = false;
static const wchar_t CLASS_NAME[] = L"FakeConsoleWindowClass";

HBRUSH blackBrush, whiteBrush;

LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_PAINT:
		{

			RECT textArea;
			GetClientRect(hWnd, &textArea);
			textArea.bottom += 12;
			textArea.top += 12;
			textArea.left += 12;
			textArea.right += 12;

			auto* fcw = reinterpret_cast<FakeConsoleWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			FillRect(hdc, &ps.rcPaint, blackBrush);
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkColor(hdc, RGB(0, 0, 0));
			{
				std::lock_guard<std::mutex> guard(fcw->text_lock);
				DrawTextA(hdc, fcw->text.c_str(), -1, &textArea, DT_LEFT | DT_TOP);
			}
			EndPaint(hWnd, &ps);
		}
		break;
		
	case WM_LOW_TIER_GOD:
		// You should kill yourself NOW!!!
		PostQuitMessage(0);
		break;
		
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void registerWindowClass()
{
	if (wndClassRegistered) return;

	WNDCLASS wc = { };
	wc.lpfnWndProc   = windowProc;
	wc.hInstance     = GetModuleHandle(nullptr);
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

	wndClassRegistered = true;
}

FakeConsoleWindow::FakeConsoleWindow()
{
	registerWindowClass();

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Window Title",
		WS_VISIBLE | WS_EX_NOACTIVATE,

		CW_USEDEFAULT, CW_USEDEFAULT, 512, 200,

		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		this);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE | WS_EX_NOACTIVATE);

	WindowManager::RegisterWindow(m_hWnd);
}

FakeConsoleWindow::~FakeConsoleWindow()
{
	WindowManager::DeregisterWindow(m_hWnd);
	CloseWindow(m_hWnd);
	DestroyWindow(m_hWnd);
}

void FakeConsoleWindow::Print(const std::string& newText)
{
	{
		std::lock_guard<std::mutex> guard(text_lock);
		text += newText;
	}
	InvalidateRect(m_hWnd, nullptr, TRUE);
}

void FakeConsoleWindow::QuitEverything() const
{
	PostMessage(m_hWnd, WM_LOW_TIER_GOD, 0, 0);
}
