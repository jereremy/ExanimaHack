#ifndef U_COM
#define U_COM CONSOLE
namespace U_COM
{
	extern void Init(std::string consoleName);
	extern void UpdateBufferSize();
	extern void UnInit();
	extern void Print(std::string str);
	extern void FlushConsole();
	extern void PopLogs();
	extern void PushLog(std::string txt, double duration);
	extern void PushError(std::string header, double duration);
	extern void MakeConsoleBackgroundInvisible();
}
#endif