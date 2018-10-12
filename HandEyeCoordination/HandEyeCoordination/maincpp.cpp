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

#include "trackbar.h"

#define VNAME(x) #x

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

	Vec3f rmin;
	Vec3f rmax;
	Vec3f bmin;
	Vec3f bmax;
	Vec3f gmin;
	Vec3f gmax;


	void cannyEdgeDetection(Mat *frame, Mat *imgCanny)
	{
		Mat frameGrayscale;
		Mat frameBlurred;

		cv::cvtColor(*frame, frameGrayscale, CV_BGR2GRAY);
		cv::GaussianBlur(frameGrayscale, frameBlurred, cv::Size(5, 5), 1.8);
		cv::Canny(frameBlurred, *imgCanny, 50, 100);
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

	void saveProperties()
	{
		std::ofstream savefile;
		savefile.open("HSV Values.txt");

		savefile << VNAME(rmin) << "," << rmin[0] << "," << rmin[1] << "," << rmin[2] << endl;
		savefile << VNAME(rmax) << "," << rmax[0] << "," << rmax[1] << "," << rmax[2] << endl;
		savefile << VNAME(bmin) << "," << bmin[0] << "," << bmin[1] << "," << bmin[2] << endl;
		savefile << VNAME(bmax) << "," << bmax[0] << "," << bmax[1] << "," << bmax[2] << endl;
		savefile << VNAME(gmin) << "," << gmin[0] << "," << gmin[1] << "," << gmin[2] << endl;
		savefile << VNAME(gmax) << "," << gmax[0] << "," << gmax[1] << "," << gmax[2] << endl;
		savefile.close();
		return;
	}

	void loadProperties()
	{
		string value;
		string intvalue;
		std::ifstream loadfile;
		loadfile.open("HSV Values.txt");

		while (loadfile.good())
		{
			getline(loadfile, value, ',');
			cout << value << endl;
			if (value == "rmin") {
				for (int i = 0; i < 3; i++) {
					getline(loadfile, value, ',');

					rmin[i] = atoi(value.c_str());
				}
				cout << rmin << endl;

			}
			if (value == "rmax")
			{
				for (int i = 0; i < 3; i++)
				{
					getline(loadfile, intvalue, ',');
					cout << "rmax " << "int value" << intvalue << endl;
					rmax[i] = atoi(intvalue.c_str());
				}
				cout << rmax << endl;
			}

		}
		cout << "rmin" << rmin << endl;
		cout << "rmax" << rmax << endl;
		loadfile.close();
	}



	int process(VideoCapture& capture) {
		int n = 0;
		char filename[200];
		string window_name1 = "video | q or esc to quit";
		string window_name2 = "Threshold";
		string window_name3 = "Red Threshhold";


		cout << "press space to save a picture. q or esc to quit" << endl;
		namedWindow(window_name1, CV_WINDOW_KEEPRATIO); //resizable window;
		cv::namedWindow(window_name2, CV_WINDOW_AUTOSIZE);
		cv::namedWindow(window_name3, CV_WINDOW_KEEPRATIO);
		trackbar::CreateTrackbars(window_name2, &H_MIN, &H_MAX, &S_MIN, &S_MAX, &V_MIN, &V_MAX);

		loadProperties();

		Mat frame;
		Mat frameGray;
		Mat frameHsv;
		Mat frameThresh;
		Mat frameRed;



		for (;;) {
			capture >> frame;
			if (frame.empty())
				break;

			/****************** Entry point for Swinburne RSD students ****************/

			/*The code contained here reads and outputs a single pixel value at (10,15)*/
			Vec3b intensity = frame.at<Vec3b>(200, 200);
			int blue = intensity.val[0];
			int green = intensity.val[1];
			int red = intensity.val[2];
			/*cout << "Intensity = " << endl << " " << blue << " " << green << " " << red << endl << endl;*/
			/*End of modifying pixel values*/
			cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
			cv::cvtColor(frame, frameHsv, cv::COLOR_BGR2HSV);


			/****************** End of Swinburne modifications ****************/

			/*HSV Thresholding*/
			thresholdHsv(&frameHsv, &frameThresh, Vec3f(H_MIN, S_MIN, V_MIN), Vec3f(H_MAX, S_MAX, V_MAX));
			thresholdHsv(&frameHsv, &frameRed, rmin, rmax);
			/*cout << "HSV: " << H_MIN << " : " << H_MAX << " : " << S_MIN << " : " << S_MAX << endl;*/
			imshow(window_name1, frame);
			imshow(window_name2, frameThresh);
			imshow(window_name3, frameRed);
			char key = (char)waitKey(5); //delay N millis, usually long enough to display and capture input
			switch (key) {
			case 'q':
				return 0;
			case 'Q':
				return 0;
			case 'r':
				rmin = Vec3f(H_MIN, S_MIN, V_MIN);
				rmax = Vec3f(H_MAX, S_MAX, V_MAX);
				cout << rmin[0] << " : " << rmin[1] << endl;
				break;
			case 'S':
				saveProperties();
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

int main(int ac, char** av) {

	if (ac != 2) {
		help(av);
		return 1;
	}
	/*createTrackbars();*/
	std::string arg = av[1];
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
