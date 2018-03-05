#include "stdafx.h"
#include "Class_Fileter.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

/*************************************
//									//
//									//
//			图像平滑处理			//
//									//
//									//
*************************************/

Mat src_temp;
int Filter_i;
int max_value;
Mat dst_blur2;	//均值
Mat dst_GaussianBlur2;//高斯
Mat dst_medianBlur2;//中值
Mat dst_bilateralFilter2;//双边

void Filter(int, void *) {
	if ((Filter_i % 2) != 0)
	{
		//归一化滤波器
		blur(src_temp, dst_blur2, Size(Filter_i, Filter_i));
		//高斯滤波器
		GaussianBlur(src_temp, dst_GaussianBlur2, Size(Filter_i, Filter_i), 0, 0);
		//中值滤波器
		medianBlur(src_temp, dst_medianBlur2, Filter_i);
		//双边滤波
		bilateralFilter(src_temp, dst_bilateralFilter2, Filter_i, Filter_i * 2, Filter_i / 2);
		imshow("原图", src_temp);
		imshow("均值模糊", dst_blur2);
		imshow("高斯模糊", dst_GaussianBlur2);
		imshow("中值模糊", dst_medianBlur2);
		imshow("双边模糊", dst_bilateralFilter2);
	}
}

Class_Fileter::Class_Fileter()
{
	

}

void Class_Fileter::first(){
	src_temp = Class_Fileter::_src;
	Filter_i = Class_Fileter::_i;
	max_value = Class_Fileter::_max_value;
	namedWindow("控制滑条", 0);
	createTrackbar("模糊程度", "控制滑条", &Filter_i, max_value,Filter);
	namedWindow("原图", 0);
	namedWindow("均值模糊", 0);
	namedWindow("高斯模糊", 0);
	namedWindow("中值模糊", 0);
	namedWindow("双边模糊", 0);
	Filter(0, 0);
}

Class_Fileter::~Class_Fileter()
{
}

