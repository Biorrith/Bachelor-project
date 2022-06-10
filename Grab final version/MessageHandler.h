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
	void takePictures(bool smallBox); //Take pictures of the whole thing, based on input
	void takeSample(); //Show a sample picture
	void takePicture(); //Take a single picture
	void goBack(bool smallBox);
	
	bool setExposureTime(double expoTime);
	double getExposureTime();
	~MessageHandler();

private:
	SimpleSerial *serialCom;
	ImageGrabber *grabber;
};