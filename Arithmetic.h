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
	void Arithmetic_HoughLines();	//��׼�����߱任
	//void Arithmetic_HoughLinesP();	//ͳ�Ƹ��ʻ����߱任

	~Arithmetic();
};

