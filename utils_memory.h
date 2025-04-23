#ifndef U_MEM
#define U_MEM MEM
namespace U_MEM
{
	extern void Init();
	extern BYTE* Scan(const char* signature);
	extern SIZE_T GetProcessMemoryUsage();

	extern std::string WSToString(const std::wstring& wstr);
	extern std::wstring	SToWString(const std::string& s);
	extern std::string ToBitString(uintptr_t value);
}
#endif 