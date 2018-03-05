#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

class Class_Fileter
{
public:
	Class_Fileter();
	~Class_Fileter();
	void first();
	//void Filter(int, void *);
	int _i;
	int _max_value;
	Mat _src;
	//Mat _dst_blur;	//均值
	//Mat _dst_GaussianBlur;//高斯
	//Mat _dst_medianBlur;//中值
	//Mat _dst_bilateralFilter;//双边

private:
	int max_value = 61;

};

