#include "MessageHandler.h"


MessageHandler::MessageHandler()
{
	//Create a new image grabber.
	grabber = new ImageGrabber();
	Sleep(1000);

	//Com port to be changed
	char com_port[] = "\\\\.\\COM4";
	//Baud rate to communicate with CNC machine
	DWORD COM_BAUD_RATE = CBR_115200;
	//Class handling the COM communication
	serialCom = new SimpleSerial(com_port, COM_BAUD_RATE);

}

//Taking the images of the tray. Smallbox represents if its the quadrant or the whole tray.
void MessageHandler::takeImages(bool smallBox){
	Sleep(500);
	cout << "Starting pictuer grabbing" << endl;
	int x, y;
	if (smallBox){
		 x = 9; //9 mm
		 y = 10; //10 mm
	}
	else{
		x = 90; //90 mm
		y = 51; //51 mm
	}

	//For moving the camera in the right directions to follow the proper traverse path
	for (int i = 0; i < y; i++){
		for (int j = 0; j < x; j++){
			if ((i % 2) == 0){
				cout << "(" << i << "," << j << ")1" << endl;
				grabber->grabImage(i, j);
				Sleep(100);
				right();
				Sleep(500);
			}
			else{
				cout << "(" << i << "," << x-j << ")2" << endl;
				grabber->grabImage(i, (x - j));
				Sleep(100);
				left();
				Sleep(500);
			}
		}
		if ((i % 2) == 0){
			cout << "(" << i << "," << x << ")3" << endl;
			grabber->grabImage(i, y-1);
			Sleep(100);
		} else{
			cout << "(" << i << "," << 0 << ")4" << endl;
			grabber->grabImage(i, 0);
			Sleep(100);
		}
		down();
		Sleep(500);
	}

}


//Go back to the starting position after the grabbing is done.
void MessageHandler::goBack(bool smallBox){
	Sleep(200);
	cout << "Going back to starting position" << endl;
	int x, y;
	if (smallBox){
		x = 9;
		y = 10;
	}
	else{
		x = 90;
		y = 51;
	}
	for (int i = 0; i < x; i++){
		left();
		Sleep(200);
	}
	for (int i = 0; i < y; i++){
		up();
		Sleep(200);
	}
}






//#################################################
//Functions regarding the camera connection.

void MessageHandler::takeSample (){
	grabber->grabSample(false); //False, don't store
}

void MessageHandler::takePicture(){
	grabber->grabSample(true); //True, do store
}

bool MessageHandler::setExposureTime(double expoTime){
	return grabber->setExposureTime(expoTime);
}


double MessageHandler::getExposureTime(){
	return grabber->getExposureTime();
}

bool MessageHandler::connectedCamera(){
	return grabber->isConnected();
}


//#################################################
//Functions regarding the USB connection to the CNC.
void MessageHandler::left(){
	char message[] = { "G91G0X1\n" }; //G-code to move 1 in x direction.
	bool is_sent = serialCom->WriteSerialPort(message);
	cout << "Send success is: " << is_sent << endl;
}

void MessageHandler::right(){
	char message[] = { "G91G0X-1\n" }; //G-code to move -1 in x direction.

	bool is_sent = serialCom->WriteSerialPort(message);

	cout << "Send success is: " << is_sent << endl;
}

void MessageHandler::down(){
	char message[] = { "G91G0Y-1\n" }; //G-code to move -1 in y direction.

	bool is_sent = serialCom->WriteSerialPort(message);

	cout << "Send success is: " << is_sent << endl;
}

void MessageHandler::up(){
	char message[] = { "G91G0Y1\n" }; //G-code to move 1 in y direction.

	bool is_sent = serialCom->WriteSerialPort(message);

	cout << "Send success is: " << is_sent << endl;
}


bool MessageHandler::connectedUSB(){
	return serialCom->getConnected();
}


MessageHandler::~MessageHandler(){
	delete[] grabber;
}