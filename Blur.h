#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;

class Blur
{
public:
	Blur(Mat src);
	~Blur();
	//ȫ����ʾ����
	void View();
	//��ֵ
	void Blur_junzhi();
	//��˹
	void GaussianBlur();
	//��ֵ
	void MedianBlur();	
	//˫��
	void BilateralFilter();
};

