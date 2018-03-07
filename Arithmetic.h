#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;

class Arithmetic
{
public:
	Arithmetic(Mat _src);

	void Arithmetic_Sobel();	//导数
	void Arithmetic_Laplacian();	//算子
	void Arithmetic_Canny();	//边缘
	void Arithmetic_HoughLines();	//标准霍夫线变换
	//void Arithmetic_HoughLinesP();	//统计概率霍夫线变换

	~Arithmetic();
};

