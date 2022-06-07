#include "Interface.h"
#include <iostream>

using namespace std;

int main(){
	Interface UI;

	UI.provideUI();

	cout << "Press enter to exit." << endl;
	std::cin.ignore();
	return 0;
}