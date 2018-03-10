#pragma once
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

/*************************************
//									//
//									//
//			直方图相关				//
//									//
//									//
*************************************/

/*
直方图相关操作，如直方图均衡化，直方图计算，直方图对比，方向投影
*/
class Histogram
{
public:
	Histogram(Mat src);

	/*直方图均衡化*/
	void Histogram_EqualizeHist();	

	/*直方图计算*/
	void Histogram_Calc();	

	/*直方图对比*/
	void Histogram_Compare(Mat pic1, Mat pic2);

	/*反向投影*/
	void Histogram_BackProject();

	~Histogram();
};

