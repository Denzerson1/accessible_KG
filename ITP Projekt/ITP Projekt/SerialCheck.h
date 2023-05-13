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
	system("echo.");
}