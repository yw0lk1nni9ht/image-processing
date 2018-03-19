#pragma once
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

class Contour
{
public:
	Contour(Mat _src);
	~Contour();
	void Match(Mat temp);
};

