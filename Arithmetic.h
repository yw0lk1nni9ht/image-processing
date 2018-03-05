#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;

class Arithmetic
{
public:
	Arithmetic(Mat _src);

	void Arithmetic_Sobel();	//µ¼Êý
	void Arithmetic_Laplacian();	//Ëã×Ó
	void Arithmetic_Canny();	//±ßÔµ

	~Arithmetic();
};

