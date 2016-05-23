#include <sys/stat.h>
#include <string>
#include <iostream>
#include <opencv2\core.hpp>

#include "cameraCalibration.h"
#include "cameraUndistortion.h"

using namespace std;
using namespace cv;

bool checkOutput() {
	string outputFile = "out_camera_data.xml";
	struct stat buffer;

	return (stat(outputFile.c_str(), &buffer) == 0);
}

int main() {
	Mat dst;
	if (checkOutput()) {
		cout << "Camera specific settings detected, would you like to recalibrate? (Y/N)" << endl;

		string ans;
		cin >> ans;

		if (ans.find("y") == 0) {
			cout << "Starting recalibration..." << endl;
			start(0, 0);
			startUndist(dst);
		}
		else {
			startUndist(dst);
		}
	}
	else {
		cout << "Starting calibration..." << endl;
		start(0, 0);
		startUndist(dst);
	}
}