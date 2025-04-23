#include "global.h"

namespace U_WIN
{
	DWORD processID = 0x0;
	HWND processWindow = nullptr;

    WNDPROC windowProc;
    float mousePos[2] = {0,0};
    bool keyDown[VK_OEM_CLEAR];
    bool keyHeld[VK_OEM_CLEAR];
    bool keyToggle[VK_OEM_CLEAR];
    int scrollAmount = 0;
    LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
		switch (msg)
		{
		case WM_MOUSEMOVE:
		{
			mousePos[0] = GET_X_LPARAM(lParam);
			mousePos[0] = GET_Y_LPARAM(lParam);
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		{
			if (wParam > 0 && wParam <= VK_OEM_CLEAR)
			{
				keyDown[wParam] = true;
				keyHeld[wParam] = true;
				keyToggle[wParam] = !keyToggle[wParam];
			}
			break;
		}
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		{
			keyDown[VK_MBUTTON] = true;
			keyHeld[VK_MBUTTON] = true;
			keyToggle[VK_MBUTTON] = !keyToggle[VK_MBUTTON];
			break;
		}
		case WM_XBUTTONDOWN:
		case WM_XBUTTONDBLCLK:
		{
			int curButton = GET_XBUTTON_WPARAM(wParam) + 4;
			keyDown[curButton] = true;
			keyHeld[curButton] = true;
			keyToggle[curButton] = !keyToggle[curButton];


			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			if (wParam > 0 && wParam <= VK_OEM_CLEAR)
			{
				keyHeld[wParam] = false;
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			keyHeld[VK_LBUTTON] = false;
			break;
		}
		case WM_RBUTTONUP:
		{
			keyHeld[VK_RBUTTON] = false;
			break;
		}
		case WM_MBUTTONUP:
		{
			keyHeld[VK_MBUTTON] = false;

			std::cout << "erm\n";
			break;
		}
		case 0x020A: // WM_MOUSEWHEEL the correct 1
		{
			scrollAmount += -(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		}
		case WM_XBUTTONUP:
		{
			int curButton = GET_XBUTTON_WPARAM(wParam) + 4;
			keyHeld[curButton] = false;
			break;
		}
		}

        return CallWindowProcA(windowProc, hwnd, msg, wParam, lParam);
    }

	void Init()
    {	
		processWindow = PidToHwnd(GetCurrentProcessId());
		windowProc = (WNDPROC)SetWindowLongPtr(processWindow, GWL_WNDPROC, (LONG_PTR)WindowProcedure);

		FlushKeys();
    }

    void UnInit()
    {
		SetWindowLongPtr(processWindow, GWLP_WNDPROC, (LONG_PTR)windowProc);

    }

    void FlushKeys()
    {
        for (int i = 0; i < VK_OEM_CLEAR; i++)
        {
            keyDown[i] = false;
        }
        scrollAmount = 0;
    }

	HWND PidToHwnd(DWORD processID)
	{
		HWND currentHwnd = GetTopWindow(nullptr);
		while (currentHwnd)
		{
			DWORD windowPID;
			GetWindowThreadProcessId(currentHwnd, &windowPID);
			if (windowPID == processID)
				return currentHwnd;
			currentHwnd = GetNextWindow(currentHwnd, GW_HWNDNEXT);
		}
		return nullptr;
	}
}