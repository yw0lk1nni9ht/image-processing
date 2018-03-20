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

	///匹配
	void Match(Mat temp);
	
	///寻找轮廓
	void FindContours();

	//计算物体的凸包
	void ConvexHull();
};

