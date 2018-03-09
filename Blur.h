#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;


/*************************************
//									//
//			ͼ��ƽ������				//
//									//
//			ͼ���봦��				//
//									//
*************************************/

/*
ͼ���봦��
ͬʱ��ʾ4�ֽ���
����ʹ�� ��
		��ֵƽ�� ��˹ƽ�� ��ֵƽ�� ˫��ƽ��
*/
class Blur
{
public:
	Blur(Mat src);
	~Blur();

	/*ȫ����ʾ����*/
	void View();

	/*��ֵƽ��*/
	void Blur_junzhi();

	/*��˹ƽ��*/
	void GaussianBlur();

	/*��ֵƽ��*/
	void MedianBlur();	

	/*˫��ƽ��*/
	void BilateralFilter();
};

