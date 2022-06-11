#pragma once

#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif
#include <iostream>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <Windows.h>

using namespace std;
using namespace Basler_UniversalCameraParams;

//:
//public Pylon::CImageEventHandler,
//public Pylon::CConfigurationEventHandler
class ImageGrabber{

	public:
		ImageGrabber(double = 25000.0);
		void grabImage(int y, int x);
		void grabSample(bool savePicture);
		bool setExposureTime(double expoTime);
		double getExposureTime();
		bool isConnected();
		//const Pylon::CGrabResultPtr GetGrabResultPtr() const;
		//const Pylon::CPylonBitmapImage& GetBitmapImage() const;
		//string datetime();
		~ImageGrabber();


	private:
		Pylon::CGrabResultPtr ptrGrabResult;
		Pylon::CBaslerUniversalInstantCamera *camera;
		void set_time();
		string date_time;
		double exposureTime;
		bool initialConnect;

};