#include "Interface.h"


Interface::Interface()
{ 
	handler = new MessageHandler();
}

void Interface::clearConsole() {
	system("cls");
}

void Interface::printSwitch() {
	cout << "#############################################" << endl << endl;
	cout << "Press 1 to start scanning." << endl;
	cout << "Press 2 to get a sample picture." << endl;
	cout << "Press 3 to set exposure time" << endl;
	cout << "Press 4 to check camera connection" << endl;
	cout << "Press 5 to check CNC connection" << endl;
	cout << "Press 6 to to exit program" << endl << endl;
	cout << "#############################################" << endl;
}

void Interface::provideUI()
{
	bool connected = false;
	if (handler->connectedCamera()){
		connected = true;
	}

	bool settingUp = true;
	int scanSpace;
	while (settingUp && connected) {
		printSwitch();
		int input = 0;
		if (!(cin >> input)) {
			cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			input = -1;
		}

		switch (input)
		{
		case 1:
			cout << "Press 1 if you wish to scan a single square of the board, press 0 if you wish to scan the whole board." << endl;

			while (settingUp){
				scanSpace = -1;
				if (!(std::cin >> scanSpace)) {
					clearConsole();
					std::cin.clear(); //clear bad input flag
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
					std::cout << "Invalid input; please re-enter either 0 or 1.\n";
				}
				else if (scanSpace != 0 && scanSpace != 1){
					clearConsole();
					std::cout << "Invalid input; please re-enter either 0 or 1.\n";
				}
				else{
					settingUp = false;
				}
			}

			break;
		case 2:
			clearConsole();
			handler->takeSample();
			clearConsole();
			break;
		case 3:
			double exposureTime;
			clearConsole();
			//TODO: change the speed of the pictures if lower exposure time.
			//TODO: make printExposure a function in handler/ImageGrabber
			//https://www.codegrepper.com/code-examples/cpp/c%2B%2B+check+if+cin+got+the+wrong+type
			while (cout << "Please enter wanted exposure time. Current exposure time is " << handler->getExposureTime() << endl
				&& !(std::cin >> exposureTime)) {
				clearConsole();
				std::cin.clear(); //clear bad input flag
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
				std::cout << "Invalid input; please re-enter.\n";
			}
			clearConsole();
			if (handler->setExposureTime(exposureTime))
				cout << "Set exposure time successfully" << endl;
			else
				cout << "Something went wrong. Please try again, perhaps with another number" << endl;
			break;

		case 4:
			clearConsole();
			if (handler->connectedCamera())
				cout << "A camera is connected." << endl;
			else
				cout << "No camera connection detected. Please connect the camera and restart the program." << endl;
			break;
		case 5:

			clearConsole();
			if (handler->connectedUSB())
				cout << "A USB connection is detected" << endl;
			else
				cout << "No USB connection connection detected. Please connect the USB and restart the program." << endl;

			break;
		case 6:
			return;
		default:
			clearConsole();
			cout << "Invalid input, please enter a valid number." << endl;
			break;
		}
	}

	//Go back to the start and grab a new picture
	Sleep(2000);
	if (connected){
		handler->goBack(scanSpace);
		//handler.takePicture(); //Grabbing an image to compare to the original 0x0 position picture.
	}


}



Interface::~Interface()
{
	delete handler;
}
