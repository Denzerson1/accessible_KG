#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <Shlwapi.h>
#include "global.h"


namespace registry
{

	bool registry_delete(HKEY hKeyRoot, LPCTSTR subkey);

	void registry_read(HKEY hKeyRoot, LPCTSTR subkey, LPCSTR name, DWORD type);

	void registry_write(HKEY hKeyRoo, LPCTSTR subkey, LPCTSTR name, DWORD type, const char* value);

	void registry_delete_key(HKEY key, std::string subkey);

	void CleanRegistry();
}
