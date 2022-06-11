#include "ImageGrabber.h"
#include <ctime>

using namespace Pylon;

ImageGrabber::ImageGrabber(double expoTime){
	//Make a folder named the current date-time
	
	set_time();
	//Create a directory which has the name of the current timestamp.
	//https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
	wstring wdate_time = wstring(date_time.begin(), date_time.end());
	LPCWSTR date_time_w = wdate_time.c_str(); //define an array with size of my_str + 1
	
	if (!CreateDirectory(date_time_w, NULL))
	{
		printf("CreateDirectory failed (%d)\n", GetLastError());
		return;
	}
	else
		cout << "Directory successfully created." << endl;

	Pylon::PylonInitialize();

	try{
		camera = new CBaslerUniversalInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
		
		// The parameter MaxNumBuffer can be used to control the count of buffers
		// allocated for grabbing. The default value of this parameter is 10.
		camera->MaxNumBuffer = 5;		
		cout << "Device connected: " << camera->GetDeviceInfo().GetModelName() << endl;
		camera->Open();
		camera->ExposureTime.SetValue(expoTime);
		exposureTime = expoTime;
		initialConnect = true;
	}

	catch (const Pylon::GenericException& e){
		// Error handling.
		cerr << "An exception occurred with the camera." << endl
			<< e.GetDescription() << endl;
		cout << "Restarting the program is recommended" << endl;
		initialConnect = false;

	}
}


//https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
void ImageGrabber::set_time(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H-%M-%S", timeinfo);
	string str(buffer);
	date_time = str;
}




void ImageGrabber::grabImage(int y, int x){

	if (!camera->IsPylonDeviceAttached()){
		cout << "Device not attached." << endl;
		return;
	}

	try{
		camera->StartGrabbing(1); //Grab a single picture

		while (camera->IsGrabbing())
		{
			// Wait for an image and then retrieve it. A timeout of 5000 ms is used.
			camera->RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);


			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{				
				//Save image taken from Utility_ImageLoadAndSave file in the date_time directory
				
				string file_dest = date_time + "/" + to_string(y) + "x" + to_string(x) + ".png";
				//int n = file_dest.length();
				const char *file_dest_char = file_dest.c_str();

				CImagePersistence::Save(ImageFileFormat_Png, file_dest_char, ptrGrabResult);

				//cout << "Grabbed and stored image!" << endl;


#ifdef PYLON_WIN_BUILD
				// Display the grabbed image.
				Pylon::DisplayImage(1, ptrGrabResult);
#endif
			}

			//If it's not successfully grabbed
			else
			{
				
				cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
			}
		}

	}

	catch (const Pylon::GenericException& e){
		// Error handling.
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
	}
}


void ImageGrabber::grabSample(bool savePicture){
	if (!camera->IsPylonDeviceAttached()){
		cout << "Device not attached." << endl;
		return;
	}
	try{
		camera->StartGrabbing(1); //Grab a single picture

		while (camera->IsGrabbing())
		{
			// Wait for an image and then retrieve it. A timeout of 5000 ms is used.
			camera->RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{

				if (savePicture){
					string file_dest = date_time + "/sample.png";
					//int n = file_dest.length();
					const char *file_dest_char = file_dest.c_str();

					CImagePersistence::Save(ImageFileFormat_Png, file_dest_char, ptrGrabResult);
				}

#ifdef PYLON_WIN_BUILD
				// Display the grabbed image.
				Pylon::DisplayImage(1, ptrGrabResult);
#endif
			}

			//If it's not successfully grabbed
			else
			{

				cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
			}
		}

	}

	catch (const Pylon::GenericException& e){
		// Error handling.
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
	}
}



bool ImageGrabber::setExposureTime(double expoTime){
	try{
		camera->Open();
		camera->ExposureTime.SetValue(expoTime);
		exposureTime = expoTime;
		return true;
	}
	catch (const Pylon::GenericException& e){
		// Error handling.
		cerr << "An exception occurred with the camera." << endl
			<< e.GetDescription() << endl;
		return false;
	}
}

double ImageGrabber::getExposureTime(){
	return exposureTime;
}


bool ImageGrabber::isConnected(){

	if (initialConnect)
		return !camera->IsCameraDeviceRemoved();
	else
		return false;
}

ImageGrabber::~ImageGrabber(){
	//delete[] camera;
}