#include "MessageHandler.h"


MessageHandler::MessageHandler(SimpleSerial *serial)
{
	serialCom = serial;
}


void MessageHandler::left(){
	char message[] = {"G91G0X1\n"}; 
	cout << "bla2" << endl;
	bool is_sent = serialCom->WriteSerialPort(message);
	cout << "Send success is: " << is_sent << endl;
}

void MessageHandler::right(){
	char message[] = { "G91G0X-1\n" };

	bool is_sent = serialCom->WriteSerialPort(message);

	cout << "Send success is: " << is_sent << endl;
}

void MessageHandler::up(){
	char message[] = { "G91G0Y-1\n" };

	bool is_sent = serialCom->WriteSerialPort(message);

	cout << "Send success is: " << is_sent << endl;
}

void MessageHandler::down(){
	char message[] = { "G91G0Y1\n" };

	bool is_sent = serialCom->WriteSerialPort(message);

	cout << "Send success is: " << is_sent << endl;
}

bool MessageHandler::connected(){
	return serialCom->connected_;
}


void MessageHandler::connectionDisplay(){
	if (connected()){
		cout << "Error connecting! Try and run again" << endl;
	}
	else
	{
		cout << "Connected succesfully!" << endl;
	}


}