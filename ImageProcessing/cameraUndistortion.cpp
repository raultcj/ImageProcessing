#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

static void onMouse(int event, int x, int y, int, void*) {
	if (event != EVENT_LBUTTONDOWN) {
		return;
	}

	x = x - 320;
	//Assuming y is 0, since the slider will be on the same y axis.
	//y = (y - 240) * -1;
	//double distance = pow((pow(x, 2) + pow(y, 2)), .5);

	double distance = x;
	double angle = distance * 0.05625;

	if (x < 0 || y < 0) {
		angle = -angle;
	}

	cout << angle << endl;
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
	VideoCapture capture(0);

	if (!capture.isOpened()) {
		cout << "Camera not detected, please connect a single camera and relaunch the program." << endl;
		return;
	}

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

		waitKey(10);
	}
	/*
	//namedWindow("Target Detection", CV_WINDOW_AUTOSIZE); //FOR CALIBRATION PURPOSES, SAME WITH THE TRACKBARS.

	//Hue
	int iLowH = 170;
	int iHighH = 179;
	//Saturation
	int iLowS = 150;
	int iHighS = 255;
	//Value
	int iLowV = 60;
	int iHighV = 255;

	cvCreateTrackbar("LowH", "Target Detection", &iLowH, 179);
	cvCreateTrackbar("HighH", "Target Detection", &iHighH, 179);

	cvCreateTrackbar("LowS", "Target Detection", &iLowS, 255);
	cvCreateTrackbar("HighS", "Target Detection", &iHighS, 255);

	cvCreateTrackbar("LowV", "Target Detection", &iLowV, 255);
	cvCreateTrackbar("HighV", "Target Detection", &iHighV, 255);

	while (true) {
		Mat frame;

		capture >> frame;

		if (frame.empty()) {
			cout << "Cannot read from camera." << endl;
			break;
		}

		Mat imgHSV;
		cvtColor(frame, imgHSV, COLOR_BGR2HSV);

		Mat imgThreshold;
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThreshold);

		erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		Moments moms = moments(imgThreshold);

		double m01 = moms.m01;
		double m10 = moms.m10;
		double mA = moms.m00;

		if (mA > 10000) {
			int posX = m10 / mA;
			int posY = m01 / mA;

			cout << posX << endl;
			cout << posY << endl;
		}

		imshow("Threshold Image", imgThreshold);
		imshow("Original Image", frame);

		if (waitKey(30) == 27) {
			break;
		}
	}
	*/
}