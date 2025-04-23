#include "global.h"

namespace U_COM
{
	static CONSOLE_SCREEN_BUFFER_INFO csbi{};
	static const COORD resetPos = COORD{ 0, 0 };
	static DWORD cellCount = 0;
	static DWORD countWrite = 0;

	HWND consoleWindow = nullptr;
	HANDLE stdOutHandle = nullptr;
	std::vector<WCHAR> buffer = std::vector<WCHAR>{};
	std::vector<char> printStack = std::vector<char>{};
	std::vector<std::pair<std::string, double>> logs = std::vector<std::pair<std::string, double>>{};
	int lastBuffIndex = 0;
	int bufWidth = 0;
	int bufHeight = 0;

	ULONG_PTR gdiplusStartupToken = NULL;
	Gdiplus::Graphics* gdipGraphics = nullptr;

	void Init(std::string consoleName)
	{
		Gdiplus::GdiplusStartupInput gdiInput;
		Gdiplus::GdiplusStartupOutput gdiOutput;
		Gdiplus::GdiplusStartup(&gdiplusStartupToken, &gdiInput, &gdiOutput);
		
		AllocConsole();
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		SetConsoleTitleA(consoleName.c_str());

		consoleWindow = GetConsoleWindow();
		stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		

		gdipGraphics = new Gdiplus::Graphics(consoleWindow);
		

		MakeConsoleBackgroundInvisible();
		UpdateBufferSize();
	}

	void UpdateBufferSize()
	{	
		GetConsoleScreenBufferInfo(stdOutHandle, &csbi);

		const int newBufWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		const int newBufHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		if (newBufWidth != bufWidth || newBufHeight != bufHeight)
		{	
			bufWidth = newBufWidth;
			bufHeight = newBufHeight;
			cellCount = bufWidth * bufHeight;
			lastBuffIndex = cellCount;
			if (cellCount > buffer.size())
				buffer.resize(cellCount);
			if (cellCount > printStack.size())
				printStack.reserve(cellCount);

			std::memset(buffer.data(), L' ', sizeof(WCHAR) * cellCount);
		}
	}

	void UnInit()
	{
		delete gdipGraphics;
		Gdiplus::GdiplusShutdown(gdiplusStartupToken);
	
		int closeAttempts = 0;
		while ((consoleWindow = GetConsoleWindow(), consoleWindow != nullptr))
		{
			FreeConsole();
			PostMessage(consoleWindow, WM_CLOSE, 0, 0);		
			Sleep(500);
			if (++closeAttempts >= 3) // idk
				break;
		}
		CloseHandle(stdOutHandle);
		CloseHandle(consoleWindow);
	}

	void Print(std::string str)
	{
		printStack.insert(printStack.end(), str.begin(), str.end());
	}

	void FlushConsole() 
	{		
		UpdateBufferSize();

		int bufIndex = 0;
		int stackSize = printStack.size();
		bool waitForNewLine = false;
		int skipQueue = 0;
		int finalBuffIndex = 0;
		for (int i = 0; i < cellCount; i++)
		{
			bool stackBig = bufIndex < stackSize;
			if (!stackBig && lastBuffIndex < i)
				break;

			bool lineWrap = i != 0 && (i % bufWidth) == 0;
			bool writing = false;
			
			if (stackBig && !lineWrap && skipQueue < 1)
			{
				char curChar = printStack[bufIndex];
				if (curChar == '\n')
				{
					waitForNewLine = true;	
				}
				if (!waitForNewLine)
				{
					if (curChar != '\t')
					{
						writing = true;
						buffer[i] = WCHAR(printStack[bufIndex]);
						finalBuffIndex = i;
					}
					else
						skipQueue += 4; // how many spaces for a tab
					bufIndex += 1;
				}
			}
			else
				skipQueue -= 1;

			if (lineWrap)
			{
				buffer[i] = L' ';
				if (waitForNewLine)
					bufIndex += 1;
				waitForNewLine = false;
			}
			else if (!writing)
			{
				buffer[i] = L' ';
			}
		}

		lastBuffIndex = finalBuffIndex;
		
		WriteConsoleOutputCharacter(stdOutHandle, buffer.data(), cellCount, resetPos, &countWrite);
		SetConsoleCursorPosition(stdOutHandle, { 0, 0 });

		printStack.clear();

		//gdipGraphics->Clear(Gdiplus::Color{0,0,0,0});
		//Gdiplus::Pen newPen = Gdiplus::Pen(Gdiplus::Color(255, 255, 255), 1.f);
		//gdipGraphics->DrawLine(&newPen, Gdiplus::Point{ 10, 10 }, Gdiplus::Point{ 500, 500 });
		//gdipGraphics->Flush(FlushIntention::FlushIntentionSync);
	}

	void PopLogs()
	{
		for (int i = 0; i < logs.size(); i++)
		{
			if (logs[i].second < GLOBAL::lastTimeStamp)
			{
				logs.erase(logs.begin() + i);		
				i--;
				continue;
			}

			Print(logs[i].first);
		}
	}
	void PushLog(std::string txt, double duration)
	{
		logs.push_back({ txt + "\n", GLOBAL::lastTimeStamp + duration });
	}

	void PushError(std::string header, double duration)
	{
		DWORD errCode = GetLastError();
		wchar_t errMsg[256];
		FormatMessageW(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			errCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			errMsg,
			sizeof(errMsg) / sizeof(wchar_t),
			nullptr
		);
		U_COM::PushLog(std::format("{}: {}",header, U_MEM::WSToString(errMsg)), duration);
	}

	void MakeConsoleBackgroundInvisible()
	{
		//HWND consoleWindow = GetConsoleWindow();
		//HDC consoleDC = GetDC(consoleWindow);

		//if (!consoleWindow) {
		//	PushLog("Failed to get console window!", 5.f);
		//	return;
		//}

		// Retrieve the current extended window style.
		//LONG exStyle = GetWindowLong(consoleWindow, GWL_EXSTYLE);

		// Add the WS_EX_LAYERED style to allow transparency.
		//SetWindowLong(consoleWindow, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW);
		//COLORREF transparentColor = RGB(0, 0, 0);
		//BYTE transparency = 128;  
		//if (!SetLayeredWindowAttributes(consoleWindow, transparentColor, transparency, LWA_COLORKEY | LWA_ALPHA)) {
		//	PushError("Error: Failed to set layered window attributes!", 5.f);
		//	return;
		//}

		//system("color 0F");
		//SetWindowLong(consoleWindow, GWL_EXSTYLE,
		//GetWindowLong(consoleWindow, GWL_EXSTYLE) | WS_EX_LAYERED);
		//COLORREF transparentColor = RGB(0, 0, 0); // Black
		//SetLayeredWindowAttributes(consoleWindow, transparentColor, 0, LWA_COLORKEY | ULW_ALPHA);
		//system("color 0F");  // Black background, white text
	}


}