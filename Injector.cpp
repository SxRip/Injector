#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
using namespace std;


enum MsgColor
{
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = 6,

	LRED = FOREGROUND_RED | FOREGROUND_INTENSITY,
	LGREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	LBLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	LYELLOW = 6 | FOREGROUND_INTENSITY,
};

void ColoredMessage(const char* _Msg, unsigned int _dwColor, bool _Endl = false)
{
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStd == INVALID_HANDLE_VALUE)
		return;

	SetConsoleTextAttribute(hStd, _dwColor);

	cout << _Msg << (_Endl ? '\n' : '\0');

	SetConsoleTextAttribute(hStd, 15);

	//CloseHandle(hStd);
}

DWORD GetProcessID(const string& _Proc)
{
	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap == INVALID_HANDLE_VALUE)
		return false;

	while (Process32Next(hSnap, &pe))
		if (_Proc == pe.szExeFile)
			return pe.th32ProcessID;

	CloseHandle(hSnap);
	return 0;
}

pair<string, string> InjectData(const char* _pName)
{
	size_t iDots = 0;

	pair<string, string> InjectInfo;
	for (size_t i = strlen(_pName) - 1; _pName[i] != '\\'; --i)
	{
		InjectInfo.first.push_back(_pName[i]);

		if (_pName[i] == '.')
			++iDots;
	}

	if (iDots != 2)
		return make_pair("0", "0");

	reverse(InjectInfo.first.begin(), InjectInfo.first.end());

	auto itDot = find(InjectInfo.first.begin(),
		InjectInfo.first.end(), '.');

	if (itDot == InjectInfo.first.end())
		return make_pair("0", "0");

	for (string::iterator it = itDot + 1; *it != '.'; ++it)
		InjectInfo.second.push_back(*it);

	InjectInfo.first.erase(itDot, InjectInfo.first.end());

	InjectInfo.first.append(".exe");
	InjectInfo.second.append(".dll");

	return InjectInfo;
}

bool IsFileExists(const char* _Path)
{
	return GetFileAttributes(_Path) != INVALID_FILE_ATTRIBUTES &&
		!(GetFileAttributes(_Path) & FILE_ATTRIBUTE_DIRECTORY);
}

int main(int* agrc, char** args)
{
	SetConsoleTitle("1337 Injector");
	pair<string, string> iData = InjectData(*args);

	if (iData.first == "0")
	{
		ColoredMessage("Error getting process name/dll.", RED, true);
		cout << "The injector have this name form: ProcessToInject.DllName.exe" << endl;
		cout << "For example: "; ColoredMessage("xrEngine.hack.exe", LGREEN);
		Sleep(10000);
		return -1;
	}

	bool FileEx = IsFileExists(iData.second.c_str());

	cout << "\n Injector by "; ColoredMessage("Sora", LYELLOW, true);
	cout << " Waiting for "; ColoredMessage(iData.first.c_str(), LRED, true);
	cout << " DLL to inject "; ColoredMessage(iData.second.c_str(), FileEx ? LGREEN : LRED, true);
	cout << "\n\n "; ColoredMessage("Discord", LBLUE); cout << ": __neverland#0001" << endl;

	HANDLE hProc = nullptr;
	HANDLE hThread = nullptr;

	try
	{
		if (!FileEx)
			throw(exception("\n DLL not found in this directory!"));

		char cPath[MAX_PATH]{};
		GetCurrentDirectory(MAX_PATH, cPath);
		strcat_s(cPath, "\\");
		iData.second = cPath + iData.second;

		DWORD procID = 0;
		while (!procID)
			procID = GetProcessID(iData.first);

		system("cls");
		ColoredMessage("\n Trying to inject...", YELLOW, true);

		hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);

		if (!hProc)
			throw(exception(" Failed opening the process"));

		LPVOID Alloc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT
			| MEM_RESERVE, PAGE_READWRITE);

		if (!Alloc)
			throw(exception(" Failed to prepare memory for injection"));

		bool isWirten = WriteProcessMemory(hProc, Alloc, iData.second.c_str(),
			iData.second.size(), 0);

		if (!isWirten)
			throw(exception(" Filed to write data to memory"));

		hThread = CreateRemoteThread(hProc, NULL, NULL,
			(LPTHREAD_START_ROUTINE)LoadLibraryA, Alloc, 0, 0);

		if (!hThread)
			throw(exception(" Failed to inject DLL"));
	}
	catch (const std::exception& ex)
	{
		system("cls");
		ColoredMessage(ex.what(), RED);

		if (hProc)
			CloseHandle(hProc);

		if (hThread)
			CloseHandle(hThread);

		Sleep(5000);
		return -1;
	}
	system("cls");
	ColoredMessage(" Successfully!", LGREEN);
	Sleep(2500);

	if (hProc)
		CloseHandle(hProc);

	if (hThread)
		CloseHandle(hThread);

	return 0;
}
