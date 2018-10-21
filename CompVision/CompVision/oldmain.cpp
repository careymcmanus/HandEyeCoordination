
/*
* RSD_starter_video.cpp
*
* This code was developed for students studying RME40003 Robot Systems Design at Swinburne
* University of Technology in Melbourne, Australia
*
* This code is largely based on the 'starter_video.cpp' code created by Ethan Rublee, Nov 23,
* 2010 for use with OpenCV software. Ethan Rublee's code comes packaged with the OpenCV files
* and can be found in ..\OpenCV\sources\samples\cpp\
*
* This is a simple starter sample  for using opencv, getting a video stream, displaying images,
* reading and outputting pixel values, and modifying pixel values.
*
*/

/*
* CODE MODIFICATIONS AND BUG FIXES
* 24/07/2014 Code added by Michelle Dunn to read, output and modify pixel values
* 21/08/2014 Comments added by Michelle Dunn for Swinburne students
*/


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;



//hide the local functions in an anon namespace
namespace {

	int H_MIN = 0;
	int H_MAX = 256;
	int S_MIN = 0;
	int S_MAX = 256;
	int V_MIN = 0;
	int V_MAX = 256;

	Vec3f omin = Vec3f(0,117,105);
	Vec3f omax = Vec3f(21, 255, 255);
	Vec3f bmin = Vec3f(49,45,63);
	Vec3f bmax = Vec3f(117, 165, 217);
	Vec3f gmin;
	Vec3f gmax;

	static Point stack1;
	static Point stack2;
	static Point stack3;
	static int activeStack = 0;
	void onTrackbar(int , void*)
	{

	}

	void createTrackbars(string name)
	{
		createTrackbar("H Min", name, &H_MIN, 255, onTrackbar);
		createTrackbar("H Max", name, &H_MAX, 255, onTrackbar);
		createTrackbar("S Min", name, &S_MIN, 255, onTrackbar);
		createTrackbar("S Max", name, &S_MAX, 255, onTrackbar);
		createTrackbar("V Min", name, &V_MIN, 255, onTrackbar);
		createTrackbar("V Max", name, &V_MAX, 255, onTrackbar);

	}


	void cannyEdgeDetection(Mat *frame, Mat *imgCanny)
	{
		Mat frameGrayscale;
		Mat frameBlurred;

		cv::cvtColor(*frame, frameGrayscale, CV_BGR2GRAY);
		cv::GaussianBlur(frameGrayscale, frameBlurred, cv::Size(5, 5), 1.8);
		cv::Canny(frameBlurred, *imgCanny, 50, 100);
		return;
	}

	void findCenter(Mat *frame, Mat *frameThreshold)
	{
		const int MAX_OBJECTS = 50;
		const int MIN_AREA = 1000;
		vector<vector<Point>> cnts;
		
		vector<Vec4i> hierachy;

		medianBlur(*frameThreshold, *frameThreshold, 5);
		imshow("Orange Threshhold", *frameThreshold);
		findContours(*frameThreshold, cnts, hierachy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
		if (hierachy.size() > 0) {
			int numObjects = hierachy.size();
			if (numObjects < MAX_OBJECTS) {
				for (int i = 0; i >= 0; i = hierachy[i][0])
				{	
					
					
					Moments M = moments(cnts[i]);
				
					drawContours(*frame, cnts, i, Scalar(0, 255, 255), 2, 8, hierachy);
					Point2f center;
					float radius;
					minEnclosingCircle(cnts[i], center, radius);
					circle(*frame, center, radius, (0, 0, 255), 3);
					float circleArea = radius * radius* 3.1416;
					float diff = circleArea - M.m00;
					//cout << circleArea << " :c " << M.m00 << " :m " << diff << " :difference"<< endl;
					if (M.m00 < MIN_AREA || diff > .4*M.m00)
					{
						
						continue;
					}
					int cX = M.m10 / M.m00;
					int cY = M.m01 / M.m00;
					Point center2 = Point(cX, cY);

					circle(*frame, center2, 7, (0, 255, 0), -1);
					drawContours(*frame, cnts, i, Scalar(0, 255, 0), 2, 8, hierachy);
				}

			}
		}
		
		return;
	}

	void drawSpots(Mat *frame, Point stack1, Point stack2, Point stack3)
	{
		string isActive = "active stack" + to_string(activeStack);
		putText(*frame, isActive, Point(50, 50), FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0, 0, 0), 2.0);

		circle(*frame, stack1, 30, Scalar(0, 0, 0), 2);
		putText(*frame, "S1", stack1, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 2.0);
		circle(*frame, stack2, 30, Scalar(0, 0, 0), 2);
		putText(*frame, "S2", stack2, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 2.0);
		circle(*frame, stack3, 30, Scalar(0, 0, 0), 2);
		putText(*frame, "S3", stack3, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 2.0);


	}

	void onMouse(int event, int x, int y, int, void *value)
	{
		if (event != EVENT_LBUTTONDOWN)
			return;

		
		switch (activeStack) {
		case 1:
			stack1 = Point(x, y);
			break;
		case 2:
			stack2 = Point(x, y);
			break;
		case 3:
			stack3 = Point(x, y);
			break;
		default:
			break;
		}
		return;
		
	}

	void thresholdHsv(Mat *frameHsv, Mat *imgThresh, Vec3f minvalues, Vec3f maxvalues)
	{
		Mat imgWorking;
		cv::inRange(*frameHsv, Scalar(minvalues[0], minvalues[1], minvalues[2]), Scalar(maxvalues[0], maxvalues[1], maxvalues[2]), imgWorking);

		cv::GaussianBlur(imgWorking, imgWorking, Size(9, 9), 2, 2);

		cv::morphologyEx(imgWorking, imgWorking, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, Size(5, 5)));
		cv::morphologyEx(imgWorking, *imgThresh, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, Size(5, 5)));
		return;

	}

	void help(char** av) {
		cout << "\nThis program justs gets you started reading images from video\n"
			"Usage:\n./" << av[0] << " <video device number>\n"
			<< "q,Q,esc -- quit\n"
			<< "space   -- save frame\n\n"
			<< "\tThis is a starter sample, to get you up and going in a copy pasta fashion\n"
			<< "\tThe program captures frames from a camera connected to your computer.\n"
			<< "\tTo find the video device number, try ls /dev/video* \n"
			<< "\tYou may also pass a video file, like my_vide.avi instead of a device number"
			<< endl;
	}


	int process(VideoCapture& capture) {
		int n = 0;
		char filename[200];
		string window_name1 = "video | q or esc to quit";
		string window_name2 = "Threshold";
		string window_name3 = "Blue Threshhold";
		string window_name4 = "Orange Threshhold";
		string window_name5 = "Purple Threshold";


		cout << "press space to save a picture. q or esc to quit" << endl;
		namedWindow(window_name1, CV_WINDOW_KEEPRATIO); //resizable window;
		cv::namedWindow(window_name2, CV_WINDOW_AUTOSIZE);
		cv::namedWindow(window_name3, CV_WINDOW_KEEPRATIO);
		cv::namedWindow(window_name4, CV_WINDOW_KEEPRATIO);
		cv::namedWindow(window_name5, CV_WINDOW_KEEPRATIO);
		createTrackbars(window_name2);
		
		

		Mat frame;
		Mat frameGray;
		Mat frameHsv;
		Mat frameThresh;
		Mat frameRed;
		Mat frameBlue;



		for (;;) {
			capture >> frame;
			if (frame.empty())
				break;

			/****************** Entry point for Swinburne RSD students ****************/

		
			/*cout << "Intensity = " << endl << " " << blue << " " << green << " " << red << endl << endl;*/
			/*End of modifying pixel values*/
			cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
			cv::cvtColor(frame, frameHsv, cv::COLOR_BGR2HSV);


			/****************** End of Swinburne modifications ****************/

			/*HSV Thresholding*/
			thresholdHsv(&frameHsv, &frameThresh, Vec3f(H_MIN, S_MIN, V_MIN), Vec3f(H_MAX, S_MAX, V_MAX));
			thresholdHsv(&frameHsv, &frameRed, omin, omax);
			thresholdHsv(&frameHsv, &frameBlue, bmin, bmax);

			/*findCenter(&frame, &frameRed);
			findCenter(&frame, &frameBlue);*/
			/*cout << "HSV: " << H_MIN << " : " << H_MAX << " : " << S_MIN << " : " << S_MAX << endl;*/

			drawSpots(&frame, stack1, stack2, stack3);
			imshow(window_name1, frame);
			imshow(window_name2, frameThresh);
			imshow(window_name3, frameBlue);
			imshow(window_name4, frameRed);
			char key = (char)waitKey(5); //delay N millis, usually long enough to display and capture input
			switch (key) {
			case 'q':
				return 0;
			case 'Q':
				return 0;
			case 'r':
				omin = Vec3f(H_MIN, S_MIN, V_MIN);
				omax = Vec3f(H_MAX, S_MAX, V_MAX);
				cout << omin[0] << " : " << omin[1] << endl;
				break;
			case '1':
				activeStack = 1;
				setMouseCallback(window_name1, onMouse);
				break;
			case '2':
				activeStack = 2;
				setMouseCallback(window_name1, onMouse);
				break;
			case '3':
				activeStack = 3;
				setMouseCallback(window_name1, onMouse);
				break;
			case 'c':
				activeStack = 0;
				break;
			case 'S':
			
				break;
			case 27: //escape key
				return 0;
			case ' ': //Save an image
				sprintf_s(filename, "filename%.3d.jpg", n++);
				imwrite(filename, frame);
				cout << "Saved " << filename << endl;
				break;
			default:
				break;
			}
		}
		return 0;
	}

}

//int main(int ac, char** av) {
//
//	if (ac != 2) {
//		help(av);
//		return 1;
//	}
//	/*createTrackbars();*/
//	std::string arg = av[1];
//	VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
//	if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
//		capture.open(atoi(arg.c_str()));
//	if (!capture.isOpened()) {
//		cerr << "Failed to open a video device or video file!\n" << endl;
//		help(av);
//		return 1;
//	}
//	return process(capture);
//}


