#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <stdlib.h>
#include <iostream>

using namespace std;

enum Colour
{
	BLUE,
	ORANGE,
	PURPLE,
	NONE
};

class Stack
{
public:
	string name;
	cv::Point position;
	Colour topColour;
	int stackCount;
	int radius;
	
	Stack(string stackName,int initialStackCount, int initialRadius);

	Stack(string stackName, int initialStackCount, int initialRadius, cv::Vec6f blue, cv::Vec6f orange, cv::Vec6f purple);
	void setPosition(cv::Point newPosition);

	void setRadius(int newRadius);

	void setThresholds(cv::Vec6f blue, cv::Vec6f orange, cv::Vec6f purple);

	void updateROI();

	void determineColour(cv::Mat* frame);

	void getROI(cv::Mat *frame);

	void getROI(cv::Mat *frame, cv::Mat *frame2);

	void getFrameRoiHsv()
	{
		cv::cvtColor(frameROI, frameRoiHsv, cv::COLOR_RGB2HSV);
	}

private:
	cv::Rect roi;


	/*Altered Frames*/
	cv::Mat frameROI;
	cv::Mat frameRoiHsv;
	cv::Mat threshBlue;
	cv::Mat threshOrange;
	cv::Mat threshPurple;

	/*Threshold Values*/
	cv::Vec6f blueThresh;
	cv::Vec6f orangeThresh;
	cv::Vec6f purpleThresh;

	
	void thresholdHsv(cv::Vec6f, cv::Mat *imgThresh);

	bool findCircles(cv::Mat *frameTheshold, cv::Mat *frame);
};

