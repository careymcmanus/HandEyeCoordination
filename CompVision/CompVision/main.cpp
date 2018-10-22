#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <math.h>

#include "Stack.h"


using namespace std;
using namespace cv;

namespace {

	Vec6f blueThresh = Vec6f(0,50, 90, 50, 225, 2225);
	Vec6f orangeThresh = Vec6f(111, 0, 0, 153, 255, 255);
	Vec6f purpleThresh = Vec6f(60, 50, 60, 255, 160, 120);

	Stack stack1 = Stack("Stack 1", 3, 30, blueThresh, orangeThresh, purpleThresh);
	Stack stack2 = Stack("Stack 2", 0, 30, blueThresh, orangeThresh, purpleThresh);
	Stack stack3 = Stack("Stack 3", 0, 30, blueThresh, orangeThresh, purpleThresh);


	int H_MIN = 0;
	int H_MAX = 256;
	int S_MIN = 0;
	int S_MAX = 256;
	int V_MIN = 0;
	int V_MAX = 256;

	void onTrackbar(int, void*)
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

	static int activeStack = 0;

	void onMouse(int event, int x, int y, int, void *value)
	{
		if (event != EVENT_LBUTTONDOWN)
			return;


		switch (activeStack) {
		case 1:
			stack1.setPosition(Point(x, y));
			break;
		case 2:
			stack2.setPosition(Point(x,y));
			break;
		case 3:
			stack3.setPosition(Point(x, y));
			break;
		default:
			break;
		}
		return;

	}

	string toString(Colour colour)
	{
		switch (colour)
		{
		case BLUE:
			return "Blue";
		case ORANGE:
			return "Orange";
		case PURPLE:
			return "Purple";
		default:
			return "";
		}
	}

	void drawSpots(Mat *frame, Stack stack1, Stack stack2, Stack stack3)
	{
		
		string isActive = "active stack" + to_string(activeStack);
		putText(*frame, isActive, Point(50, 50), FONT_HERSHEY_PLAIN, 2.0, CV_RGB(0, 0, 0), 2.0);
		string stack1Text = "S1: " + toString(stack1.topColour);
		string stack2Text = "S2: " + toString(stack2.topColour);
		string stack3Text = "S3: " + toString(stack3.topColour);
		circle(*frame, stack1.position, stack1.radius, Scalar(0, 0, 0), 2);
		putText(*frame, stack1Text, stack1.position, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 2.0);
		circle(*frame, stack2.position, 30, Scalar(0, 0, 0), 2);
		putText(*frame, stack2Text, stack2.position, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 2.0);
		circle(*frame, stack3.position, 30, Scalar(0, 0, 0), 2);
		putText(*frame, stack3Text, stack3.position, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 2.0);


	}

	int process(VideoCapture& capture)
	{
		int n = 0;
		char filename[200];
		string window_name1 = "video | q or esc to quit";
		string window_name2 = "region of interest";
		string window_name3 = "bla";
		

		cout << "press space to save a picture. q or esc to quit" << endl;
		namedWindow(window_name1, CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow(window_name2, CV_WINDOW_KEEPRATIO);
		namedWindow(window_name3, CV_WINDOW_KEEPRATIO);
		namedWindow("window", CV_WINDOW_KEEPRATIO);

		createTrackbars(window_name1);


		Mat frame;
		Mat frameROI;

		for (;;)
		{
			capture >> frame;
			if (frame.empty())
				break;

			
			stack1.determineColour(&frame);
			stack2.determineColour(&frame);
			stack3.determineColour(&frame);
			//stack1.getROI(&frame, &frameROI);
			drawSpots(&frame, stack1, stack2, stack3);
			imshow(window_name1,frame);
			//imshow(window_name2, frameROI);

			char key = (char)waitKey(5); //delay N millis, usually long enough to display and capture input
			switch (key) {
			case 'q':
				return 0;
			case 'Q':
				return 0;
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
			case 'b':
				blueThresh = Vec6f(H_MIN, S_MIN, V_MIN, H_MAX, S_MAX, V_MAX);
				stack1.setThresholds(blueThresh, orangeThresh, purpleThresh);
				break;
			case 'f':
				orangeThresh = Vec6f(H_MIN, S_MIN, V_MIN, H_MAX, S_MAX, V_MAX);
				stack1.setThresholds(blueThresh, orangeThresh, purpleThresh);
				break;
			case 'p':
				purpleThresh = Vec6f(H_MIN, S_MIN, V_MIN, H_MAX, S_MAX, V_MAX);
				stack1.setThresholds(blueThresh, orangeThresh, purpleThresh);
				break;
			case 'c':
				activeStack = 0;
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


}

int main(int ac, char** av) {

	if (ac != 2) {
		help(av);
		return 1;
	}
	/*createTrackbars();*/
	std::string arg = av[0];
	VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
	if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
		capture.open(atoi(arg.c_str()));
	if (!capture.isOpened()) {
		cerr << "Failed to open a video device or video file!\n" << endl;
		help(av);
		return 1;
	}
	return process(capture);
}
