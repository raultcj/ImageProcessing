#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>

#include "arduinoSerialConnection.h"

using namespace cv;
using namespace std;

static void onMouse(int event, int x, int y, int, void*) {
	if (event != EVENT_LBUTTONDOWN) {
		return;
	}

	x = x - 320;

	double cameraConst = 78 / sqrt(pow(640, 2) + pow(480, 2));

	double distance = x;
	double angle = distance * cameraConst;

	double depth = 1;
	double movRequired = depth * tan(angle);

	cout << angle << endl;
	cout << movRequired << endl;

	connect(movRequired);
}

void undistortCamera(InputArray src, OutputArray dst) {
	Mat cameraMatrix;
	Mat distCoeffs;

	string fileName = "out_camera_data.xml";
	FileStorage fs(fileName, FileStorage::READ);

	fs["Camera_Matrix"] >> cameraMatrix;
	fs["Distortion_Coefficients"] >> distCoeffs;

	undistort(src, dst, cameraMatrix, distCoeffs);
}

void startUndist(OutputArray dst) {
	destroyAllWindows();
	VideoCapture capture(0);

	if (!capture.isOpened()) {
		cout << "Camera not detected, please connect a single camera and relaunch the program." << endl;
		return;
	}
	//Click mode
	/*
	while (true) {
		Mat orig, undist;

		capture >> orig;

		if (orig.empty()) {
			cout << "Cannot read from camera." << endl;
			break;
		}

		undistortCamera(orig, undist);

		namedWindow("Undistorted Image", CV_WINDOW_AUTOSIZE);
		imshow("Undistorted Image", undist);
		setMouseCallback("Undistorted Image", onMouse, 0);

		if (waitKey(30) == 27) {
			break;
		}
	}
	*/

	//Color Mode

	//Hue
	int iLowH = 0;
	int iHighH = 179;
	//Saturation
	int iLowS = 161;
	int iHighS = 183;
	//Value
	int iLowV = 90;
	int iHighV = 144;

	namedWindow("Target Detection", CV_WINDOW_AUTOSIZE); //FOR CALIBRATION PURPOSES, SAME WITH THE TRACKBARS.

	cvCreateTrackbar("LowH", "Target Detection", &iLowH, 179);
	cvCreateTrackbar("HighH", "Target Detection", &iHighH, 179);

	cvCreateTrackbar("LowS", "Target Detection", &iLowS, 255);
	cvCreateTrackbar("HighS", "Target Detection", &iHighS, 255);

	cvCreateTrackbar("LowV", "Target Detection", &iLowV, 255);
	cvCreateTrackbar("HighV", "Target Detection", &iHighV, 255); //END CALIBRATION PURPOSES

	while (true) {
		Mat orig, undist;

		if (!capture.read(orig)) {
			cout << "Cannot read from camera." << endl;
			break;
		}

		undistortCamera(orig, undist);

		Mat imgHSV;
		cvtColor(undist, imgHSV, COLOR_BGR2HSV);

		Mat imgThreshold;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThreshold);

		erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		imshow("Threshold Image", imgThreshold);
		imshow("Undistorted Image", undist);

		Moments moms = moments(imgThreshold);

		double m01 = moms.m01;
		double m10 = moms.m10;
		double mA = moms.m00;

		if (mA > 10000) {
			int posX = m10 / mA;
			int posY = m01 / mA;

			if (posX < 300) {
				connect(1);
			}
			else if (posX > 340) {
				connect(0);
			}
			else {
				connect(2);
				cout << "Acceptable position" << endl;
			}
		}

		if (waitKey(1) == 27) {
			break;
		}
	}
}