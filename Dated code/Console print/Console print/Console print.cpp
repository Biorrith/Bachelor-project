// Console print.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

void clearConsole() {
	system("cls");
}

void printSwitch() {
	cout << "#############################################" << endl << endl;
	cout << "Press 1 to start scanning." << endl;
	cout << "Press 2 to get a sample picture." << endl;
	cout << "Press 3 to set exposure time" << endl;
	cout << "Press 4 to check camera connection" << endl;
	cout << "Press 5 to check USB connection" << endl << endl;
	cout << "#############################################" << endl ;
}

//void printExposure(MessageHandler *handler) {
bool printExposure() {
	if (cout << "Please enter wanted exposure time. Current time is " << endl)// handler.getExposure() << endl
		return true;
	else
		return false;
}


int main()
{

	bool settingUp = true;
	while (settingUp) {
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
			cout << "Setup complete, starting scan" << endl;
			settingUp = false;
			break;
		case 2:
			clearConsole();
			//handler.takePicture();
			//cout << "Setup complete, starting scan" << endl;
			//settingUp = false;
			break;
		case 3:
			double exposureTime;
			clearConsole();
			//TODO: make it print current exposure time and change the speed of the pictures if lower exposure time.
			//TODO: make printExposure a function in handler/ImageGrabber
			//https://www.codegrepper.com/code-examples/cpp/c%2B%2B+check+if+cin+got+the+wrong+type
			while (printExposure() && !(std::cin >> exposureTime)) {
				clearConsole();
				std::cin.clear(); //clear bad input flag
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
				std::cout << "Invalid input; please re-enter.\n";
			}
			clearConsole();
			//handler.setExposureTime(exposureTime);
			break;

		case 4:
			//handler.microscopeConnection();
			clearConsole();
			break;
		case 5:
			//handler.usbConnection();
			clearConsole();
			break;
		default:
			clearConsole();
			cout << "Invalid input, please enter a valid number." << endl;
			break;
		}
	}

	return 0;
}





// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
