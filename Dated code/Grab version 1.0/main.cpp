#include <iostream>
#include "MessageHandler.h";
#include <Windows.h>
#include "SerialCom.h"
#include <conio.h>

using namespace std;


int main(){
	
	char com_port[] = "\\\\.\\COM3";
	DWORD COM_BAUD_RATE = CBR_115200;
	SimpleSerial *serialCom = new SimpleSerial(com_port, COM_BAUD_RATE);
	MessageHandler handler(serialCom);
	handler.connectionDisplay();
	
	
	char tast;
	do
	{
		cout << "W, A, S or D for up, left, down or right, respectively. Q for exit." << endl;
		tast = _getch();

		switch (tast)
		{
		case'w':
		case'W':
			handler.up();
			break;
		
		case's':
		case'S':
			handler.down();
			break;

		case'a':
		case'A':
			handler.left();
			break;

		case'd':
		case'D':
			handler.right();
			break;
		}
	} while (tast != 'q' && tast != 'Q');

	cout << "Hit enter to exit." << endl;
	std::cin.ignore();
	return 0;
}