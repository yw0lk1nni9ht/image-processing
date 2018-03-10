#pragma once
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

/*************************************
//									//
//									//
//			ֱ��ͼ���				//
//									//
//									//
*************************************/

/*
ֱ��ͼ��ز�������ֱ��ͼ���⻯��ֱ��ͼ���㣬ֱ��ͼ�Աȣ�����ͶӰ
*/
class Histogram
{
public:
	Histogram(Mat src);

	/*ֱ��ͼ���⻯*/
	void Histogram_EqualizeHist();	

	/*ֱ��ͼ����*/
	void Histogram_Calc();	

	/*ֱ��ͼ�Ա�*/
	void Histogram_Compare(Mat pic1, Mat pic2);

	/*����ͶӰ*/
	void Histogram_BackProject();

	~Histogram();
};

