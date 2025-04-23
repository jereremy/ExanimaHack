#ifndef U_WIN
#define U_WIN WINDOW
namespace U_WIN
{
    extern float mousePos[2];
    extern bool keyDown[VK_OEM_CLEAR];
    extern bool keyHeld[VK_OEM_CLEAR];
    extern bool keyToggle[VK_OEM_CLEAR];
    LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    extern void Init();
    extern void UnInit();
    extern void FlushKeys();
    extern HWND PidToHwnd(DWORD processID);
}
#endif