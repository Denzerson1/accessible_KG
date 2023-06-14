#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


DWORD GetProcessIDTaskMgr(const char* procName) {
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_stricmp(procEntry.szExeFile, procName)) {

					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

void runHook() {
	const char* dllPath = "WinHook.dll";
	const char* procName = "Taskmgr.exe";
	DWORD procId = 0;

	while (!procId) {
		procId = GetProcessIDTaskMgr(procName);
		Sleep(30);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE) {
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc) {
			WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
		}

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread) {
			CloseHandle(hThread);
		}

	}
	if (hProc) {
		CloseHandle(hProc);
	}
}

bool IsProcessRunning(const wchar_t* processName)
{
	PROCESSENTRY32 processEntry = { 0 };
	processEntry.dwSize = sizeof(processEntry);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	if (!Process32First(snapshot, &processEntry)) {
		CloseHandle(snapshot);
		return false;
	}

	while (Process32Next(snapshot, &processEntry)) {
		if (!_wcsicmp((wchar_t*)processEntry.szExeFile, processName)) {
			CloseHandle(snapshot);
			return true;
		}
	}

	CloseHandle(snapshot);
	return false;
}