#pragma once
#include<opencv2\opencv.hpp>
using namespace cv;


/*************************************
//									//
//			图像平滑处理				//
//									//
//			图像降噪处理				//
//									//
*************************************/

/*
图像降噪处理
同时显示4种降噪
单独使用 ：
		均值平滑 高斯平滑 中值平滑 双边平滑
*/
class Blur
{
public:
	Blur(Mat src);
	~Blur();

	/*全部显示处理*/
	void View();

	/*均值平滑*/
	void Blur_junzhi();

	/*高斯平滑*/
	void GaussianBlur();

	/*中值平滑*/
	void MedianBlur();	

	/*双边平滑*/
	void BilateralFilter();
};

