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
	cv::imshow("bla", threshPurple);

	bool isBlue = findCircles(&threshBlue, frame);
	bool isOrange = findCircles(&threshOrange, frame);
	bool isPurple = findCircles(&threshPurple, frame);
	if (isBlue && !isOrange && !isPurple)
	{
		cout << "Top is Blue" << endl;
		topColour = BLUE;
	}
	else if (isOrange && !isBlue && !isPurple) 
	{
		cout << "Top is Orange" << endl;
		topColour = ORANGE;
	}
	else if (isPurple && !isBlue && !isOrange)
	{
		cout << "Top is Purple" << endl;
		topColour = PURPLE;
	}
	else
	{
		cout << "Top is Nothing" << endl;
		topColour = NONE;
	}

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
	cv::imshow("window", frameRoiHsv);
	cv::inRange(frameRoiHsv, cv::Scalar(thresholdValues[0], thresholdValues[1], thresholdValues[2]), cv::Scalar(thresholdValues[3], thresholdValues[4], thresholdValues[5]), imgWorking);


	cv::morphologyEx(imgWorking, imgWorking, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::morphologyEx(imgWorking, *imgThresh, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	return;
}

bool Stack::findCircles(cv::Mat *frameThreshold, cv::Mat *frame)
{
	float stackArea = (radius-5) * (radius-5)* 3.1416;
	vector<vector<cv::Point>> cnts;
	vector<cv::Vec4i> hierachy;

	cv::findContours(*frameThreshold, cnts, hierachy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < cnts.size(); i++)
	{
		cv::drawContours(frameROI, cnts, i, cv::Scalar(255, 0, 0));
		cv::Moments contourMoments = cv::moments(cnts[i]);
		int cX = contourMoments.m10 / contourMoments.m00;
		int cY = contourMoments.m01 / contourMoments.m00;
		float areaDiff = abs(stackArea - contourMoments.m00);
		if (areaDiff > .5*contourMoments.m00)
		{
			continue;
		}
		for (int j = 0; j < cnts[i].size(); j++)
		{
			cnts[i][j].x = cnts[i][j].x + position.x - radius;
			cnts[i][j].y = cnts[i][j].y + position.y - radius;
		}
		//circle(*frame, position, radius, (0, 255, 0), -1);
		cv::drawContours(*frame, cnts, i, cv::Scalar(0, 255, 0), 2, 8, hierachy);
		return true;
	}
	return false;
}



