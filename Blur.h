#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;

class Blur
{
public:
	Blur(Mat src);
	~Blur();
	//全部显示处理
	void View();
	//均值
	void Blur_junzhi();
	//高斯
	void GaussianBlur();
	//中值
	void MedianBlur();	
	//双边
	void BilateralFilter();
};

