#include "stdafx.h"
#include "Class_Eroding_and_Dilating.h"
#include <math.h>

#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"highgui.h"
using namespace cv;

/*腐蚀膨胀*/
Mat src2;
Mat dst_Dilate;
Mat dst_Erode;
Mat kernel;
int para1;
int para2;
void E_and_D2(int ,void *);

/*其他形态*/
Mat dst_Other;
int op;
void Morphology2(int, void *);

/*放大缩小*/
Mat dst_Up;
Mat dst_Down;
int num_Up;
int num_Down;
void Up(int ,void *);
void Down(int, void *);
Mat temp;

/*阈值操作*/
Mat dst_Threshold;
int threshold_value;
int threshold_type;
void Threshold2(int, void *);

/*定制滤波器*/
Mat dst_filter;
int Self_Filter_size;
void Fileter2(int, void *);


Class_Eroding_and_Dilating::Class_Eroding_and_Dilating(Mat _src)
{
	src2 = _src;
}

/*腐蚀膨胀*/
void Class_Eroding_and_Dilating::E_and_D()
{
	E_and_D2(0,0);
}

void E_and_D2(int ,void *) {
	namedWindow("控制条", 0);
	namedWindow("原图显示", 0);
	namedWindow("膨胀显示", 0);
	namedWindow("腐蚀显示", 0);
	imshow("原图显示", src2);
	createTrackbar("矩阵大小拖条", "控制条", &para1, 50, E_and_D2);
	createTrackbar("膨胀拖条", "控制条", &para2, 2, E_and_D2);
	if (para1 == 0)
		return;
	if (para2 == 0) {
		/*kernel = getStructuringElement(0, Size(para1*2+1, para1 * 2 + 1));*/
		kernel = getStructuringElement(0, Size(para1 , para1 ));
	}
	if (para2 == 1) {
		//kernel = getStructuringElement(1, Size(para1 * 2 + 1, para1 * 2 + 1));
		kernel = getStructuringElement(1, Size(para1 * 2 + 1, para1 * 2 + 1));
	}
	if (para2 == 2) {
		//kernel = getStructuringElement(2, Size(para1 * 2 + 1, para1 * 2 + 1));
		kernel = getStructuringElement(2, Size(para1 * 2 + 1, para1 * 2 + 1));
	}
	dilate(src2, dst_Dilate, kernel);
	erode(src2, dst_Erode, kernel);
	imshow("膨胀显示", dst_Dilate);
	imshow("腐蚀显示", dst_Erode);
}


/*其他形态*/
void Class_Eroding_and_Dilating::Morphology()
{
	Morphology2(0, 0);
}

void Morphology2(int, void *)
{
	namedWindow("形态变换",0);
	namedWindow("其他形态",0);
	createTrackbar("控制条", "形态变换", &op, 4, Morphology2);
	createTrackbar("数值", "形态变换", &para1, 100, Morphology2);
	int operation = op + 2;
	if (para1 == 0)
		para1 = 1;
	kernel = getStructuringElement(0, Size(para1, para1));
	morphologyEx(src2, dst_Other, operation, kernel);
	imshow("其他形态", dst_Other);
}


/*放大缩小*/
void Class_Eroding_and_Dilating::Up_and_Down() {
	Mat tmp_Up;
	Mat tmp_Down;
	namedWindow("控制台", 0);
	temp = src2;
	
	createTrackbar("放大倍数", "控制台", &num_Up, 20,Up);
	createTrackbar("缩小倍数", "控制台", &num_Down, 20, Down);

	namedWindow("放大", 0);
	namedWindow("缩小", 0);
}

void Up(int ,void *)
{
	pyrUp(temp, dst_Up, Size(temp.cols * 2, temp.rows * 2));
	temp = dst_Up;
	imshow("放大", dst_Up);
}

void Down(int, void *)
{
	Mat temp;
	temp = src2;
	
	for (int i = 0; i<num_Down; i++)
	{
		pyrDown(temp, dst_Up, Size(temp.cols / 2, temp.rows / 2));
		temp = dst_Up;
	}
	imshow("缩小", dst_Up);
}


/*阈值操作*/
void Class_Eroding_and_Dilating::Threshold() {
	namedWindow("控制", 0);
	namedWindow("显示", 0);
	createTrackbar("阈值大小", "控制", &threshold_value, 255, Threshold2);
	createTrackbar("阈值类型", "控制", &threshold_type, 4, Threshold2);
}

void Threshold2(int, void *) {
	threshold(src2, dst_Threshold,threshold_value,255,threshold_type);
	imshow("显示", dst_Threshold);
}


/*定制滤波器*/
void Class_Eroding_and_Dilating::Self_Filter()
{
	namedWindow("滤波器", 0);
	namedWindow("控制器");
	createTrackbar("内核拖条", "控制器", &Self_Filter_size, 2000, Fileter2);
}

void Fileter2(int ,void *)
{
	int kernel_size = 3 + 2 * (Self_Filter_size % 5);
	kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);
	filter2D(src2, dst_filter, -1, kernel);
	imshow("滤波器", dst_filter);
}

Class_Eroding_and_Dilating::~Class_Eroding_and_Dilating()
{
}
