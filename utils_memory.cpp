#include "global.h"

namespace U_MEM
{
	bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask) {
		for (; *szMask; ++szMask, ++pData, ++bMask) {
			if (*szMask == 'x' && *pData != *bMask) {
				return false;
			}
		}

		return (*szMask) == NULL;
	}

	BYTE* FindPattern(BYTE* dwAddress, SIZE_T dwSize, BYTE* pbSig, const char* szMask) {
		SIZE_T length = strlen(szMask);
		for (SIZE_T i = 0; i < dwSize - length; i++) {
			if (Compare(dwAddress + i, pbSig, szMask)) {
				return dwAddress + i;
			}
		}
		return nullptr;
	}

	static uintptr_t sizeOfProcess = 0;
	static const uintptr_t baseAddr = 0x00400000;
	void Init()
	{
		sizeOfProcess = GetProcessMemoryUsage() - baseAddr;
	}

	BYTE* Scan(const char* signature)
	{
		if (sizeOfProcess < 1) {
			std::cerr << "Failed to get sizeOfProcess." << std::endl;
			return nullptr;
		}

		BYTE* pbSig = new BYTE[strlen(signature) / 2];
		char* szMask = new char[strlen(signature) / 2 + 1];
		SIZE_T j = 0;

		for (SIZE_T i = 0; i < strlen(signature); i += 3) {
			if (signature[i] == '?') {
				pbSig[j] = 0;
				szMask[j] = '?';
			}
			else {
				pbSig[j] = (BYTE)strtol(&signature[i], NULL, 16);
				szMask[j] = 'x';
			}
			j++;
		}
		szMask[j] = '\0';

		BYTE* result = FindPattern((BYTE*)baseAddr, sizeOfProcess, pbSig, szMask);

		delete[] pbSig;
		delete[] szMask;

		if (!result)
		{
			U_COM::PushLog("failed to get address " + std::string(signature), 15.f);
		}

		return result;
	}

	SIZE_T GetProcessMemoryUsage() {

		// Open the process with the specified PID
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)GetCurrentProcessId());
		if (hProcess == NULL) {
			U_COM::PushError("Failed to open process", 15.f);
			return 0;
		}

		// Get process memory information
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
			return pmc.WorkingSetSize;
		else
			return 0;
		
	}

	std::string WSToString(const std::wstring& wstr) {
		if (wstr.empty())return std::string();
		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (sizeNeeded == 0) return std::string();
		std::string strTo(sizeNeeded, 0);
		int result = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], sizeNeeded, nullptr, nullptr);
		if (result == 0)return std::string(); 
		if (!strTo.empty() && strTo.back() == '\0')strTo.pop_back();
		return strTo;
	}
	std::wstring SToWString(const std::string& s) {
		int bufferSize = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
		std::wstring ws(bufferSize, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &ws[0], bufferSize);
		ws.resize(std::wcslen(ws.c_str()));
		return ws;
	}
	std::string ToBitString(uintptr_t value) {
		std::bitset<32> bits(value);
		std::string finalStr = bits.to_string();
		return finalStr.substr(0, 32);
	}
}