#pragma once
#include <iostream>
#include "SerialCom.h"
#include "ImageGrabber.h"

using namespace std;

class MessageHandler{

public:
	MessageHandler();
	void up();
	void down();
	void left();
	void right();
	bool connectedUSB();
	bool connectedCamera();
	void takePictures(bool smallBox);
	void goBack(bool smallBox);
	void takeSample();
	void takePicture();
	bool setExposureTime(double expoTime);
	double getExposureTime();
	~MessageHandler();

private:
	SimpleSerial *serialCom;
	ImageGrabber *grabber;
};