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
	//Æ¥Åä£¬Èë²Î£ºÄ£°åÍ¼
	void Match(Mat temp);

	//Ñ°ÕÒÂÖÀª
	void FindContour();
};

