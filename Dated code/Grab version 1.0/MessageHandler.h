#pragma once
#include <iostream>
#include "SerialCom.h"

using namespace std;

class MessageHandler{

	public:
		MessageHandler(SimpleSerial *serial);
		void up();
		void down();
		void left();
		void right();
		bool connected();
		void connectionDisplay();


	private:
		SimpleSerial *serialCom;
		

};