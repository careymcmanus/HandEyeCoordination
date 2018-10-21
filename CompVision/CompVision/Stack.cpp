#include "Stack.h"



Stack::Stack(string stackName, int initialStackCount, int initialRadius)
{
	name = stackName;
	stackCount = initialStackCount;
	position = cv::Point(50, 50);
	radius = initialRadius;
	roi = cv::Rect(position.x + radius + 20, position.y + radius + 20, 2 * radius + 20, 2 * radius + 20);
}

Stack::Stack(string stackName, int initialStackCount, int initialRadius, cv::Vec6f blue, cv::Vec6f orange, cv::Vec6f purple)
{
	name = stackName;
	stackCount = initialStackCount;
	position = cv::Point(50, 50);
	radius = initialRadius;
	updateROI();
	blueThresh = blue;
	orangeThresh = orange;
	purpleThresh = purple;
}

void Stack::setPosition(cv::Point newPosition)
{
	position = newPosition;
}

void Stack::setRadius(int newRadius)
{
	radius = newRadius;
}

void Stack::setThresholds(cv::Vec6f blue, cv::Vec6f orange, cv::Vec6f purple)
{
	blueThresh = blue;
	orangeThresh = orange;
	purpleThresh = purple;
}

void Stack::updateROI()
{
	roi = cv::Rect(position.x - radius , position.y - radius , radius*2, radius*2);
}

void Stack::determineColour(cv::Mat *frame)
{

	getROI(frame);
	getFrameRoiHsv();

	thresholdHsv(blueThresh, &threshBlue);
	thresholdHsv(orangeThresh, &threshOrange);
	thresholdHsv(purpleThresh, &threshPurple);

	
	bool blue = findCircles(&threshBlue, frame);
	cout << "is blue: " << blue << endl;
	/*if (findCircles(&threshBlue, frame))
	{
		topColour = BLUE;
	}
	if (findCircles(&threshOrange, frame)) 
	{
		topColour = ORANGE;
	}
	if (findCircles(&threshPurple, frame))
	{
		topColour = PURPLE;
	}*/

}


void Stack::getROI(cv::Mat *frame, cv::Mat *frame2)
{
	updateROI();
	*frame2 = (*frame)(roi);
}

void Stack::getROI(cv::Mat *frame)
{
	updateROI();
	frameROI = (*frame)(roi);
}

void Stack::thresholdHsv(cv::Vec6f thresholdValues, cv::Mat *imgThresh)
{
	cv::Mat imgWorking;
	cv::imshow("bla", frameRoiHsv);
	cv::inRange(frameRoiHsv, cv::Scalar(thresholdValues[0], thresholdValues[1], thresholdValues[2]), cv::Scalar(thresholdValues[1], thresholdValues[2], thresholdValues[3]), imgWorking);

	cv::GaussianBlur(imgWorking, imgWorking, cv::Size(9, 9), 2, 2);

	cv::morphologyEx(imgWorking, imgWorking, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::morphologyEx(imgWorking, *imgThresh, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	return;
}

bool Stack::findCircles(cv::Mat *frameThreshold, cv::Mat *frame)
{
	float stackArea = radius * radius* 3.1416;
	vector<vector<cv::Point>> cnts;
	vector<cv::Vec4i> hierachy;

	cv::findContours(*frameThreshold, cnts, hierachy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < cnts.size(); i++)
	{
		cv::Moments contourMoments = cv::moments(cnts[i]);
		int cX = contourMoments.m10 / contourMoments.m00;
		int cY = contourMoments.m01 / contourMoments.m00;
		float areaDiff = abs(stackArea - contourMoments.m00);
		if (areaDiff > .4*contourMoments.m00)
		{
			continue;
		}
		//circle(*frame, position, radius, (0, 255, 0), -1);
		cv::drawContours(*frame, cnts, i, cv::Scalar(0, 255, 0), 2, 8, hierachy);
		return true;
	}
	return false;
}



