#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;

class Arithmetic
{
public:
	Arithmetic(Mat _src);

	void Arithmetic_Sobel();	//����
	void Arithmetic_Laplacian();	//����
	void Arithmetic_Canny();	//��Ե

	~Arithmetic();
};

