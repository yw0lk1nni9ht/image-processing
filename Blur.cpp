#include "stdafx.h"
#include "Blur.h"

#include<opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"

using namespace cv;


//声明
Mat Blur_Src;
int Blur_i;
void callback(int, void *);
Mat dst_blur;
Mat dst_GaussianBlur;
Mat dst_medianBlur;
Mat dst_bilateralFileter;

Blur::Blur(Mat _src)
{
	Blur_Src = _src;
}


void Blur::View()
{
	namedWindow("控制滑条");
	createTrackbar("值", "控制滑条", &Blur_i, 100, callback);

	namedWindow("均值平滑显示", 0);
	namedWindow("高斯平滑显示", 0);
	namedWindow("中值平滑显示", 0);
	namedWindow("双边平滑显示", 0);
}


void callback(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	///使用均值平滑
	blur(Blur_Src, dst_blur, Size(Blur_i, Blur_i), Point(-1, -1));
	imshow("均值平滑显示", dst_blur);

	/// 使用高斯平滑
	GaussianBlur(Blur_Src, dst_GaussianBlur, Size(Blur_i, Blur_i), 0, 0);
	imshow("高斯平滑显示", dst_GaussianBlur);

	/// 使用中值平滑
	medianBlur(Blur_Src, dst_medianBlur, Blur_i);
	imshow("中值平滑显示", dst_medianBlur);

	/// 使用双边平滑
	bilateralFilter(Blur_Src, dst_bilateralFileter, Blur_i, Blur_i * 2, Blur_i / 2);
	imshow("双边平滑显示", dst_bilateralFileter);
}

//均值处理
void blur_change(int ,void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	blur(Blur_Src, dst_blur, Size(Blur_i, Blur_i), Point(-1, -1));
	imshow("均值平滑处理",dst_blur);
}
void Blur::Blur_junzhi() {
	namedWindow("均值平滑处理", 0);
	createTrackbar("值", "均值平滑处理", &Blur_i, 100, blur_change);
}

//高斯处理
void Gaussian_change(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	GaussianBlur(Blur_Src, dst_GaussianBlur, Size(Blur_i, Blur_i), 0, 0);
	imshow("高斯平滑处理", dst_GaussianBlur);
}
void Blur::GaussianBlur() {
	namedWindow("高斯平滑处理", 0);
	createTrackbar("值", "高斯平滑处理", &Blur_i, 100, Gaussian_change);
}


//中值处理
void MedianBlur_change(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	medianBlur(Blur_Src, dst_medianBlur, Blur_i);
	imshow("中值平滑处理", dst_medianBlur);
}
void Blur::MedianBlur() {
	namedWindow("中值平滑处理", 0);
	createTrackbar("值", "中值平滑处理", &Blur_i, 100, MedianBlur_change);
}


//双边处理
void BilateralFilter_change(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	bilateralFilter(Blur_Src, dst_bilateralFileter, Blur_i, Blur_i * 2, Blur_i / 2);
	imshow("双边平滑处理", dst_bilateralFileter);
}
void Blur::BilateralFilter() {
	namedWindow("双边平滑处理", 0);
	createTrackbar("值", "双边平滑处理", &Blur_i, 100, BilateralFilter_change);
}


Blur::~Blur()
{
}
