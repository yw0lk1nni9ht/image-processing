// testpic.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"
#include "Histogram.h"
#include "Contour.h"

#include "集成测试.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;



int main()
{
	// 载入原图像
	//Mat src = imread("testpicture\\num2.jpg", 1);
	Mat src = imread("piaohao.jpg", 1);
	//Mat src = imread("pic2.jpg", 1);



	//平滑处理
	/*Class_Fileter a;
	a._src = src;
	a._i = 1;
	a._max_value = 71;
	a.first();*/

	
	//Class_Eroding_and_Dilating b(src);
	//膨胀
	//b.E_and_D();
	//更多形态变化
	//b.Morphology();
	//放大和缩小
	//b.Up_and_Down();
	//阈值操作
	//b.Threshold();
	//自制滤波
	//b.Self_Filter();
	
	//Blur b(src);
	//b.View();
	//b.BilateralFilter();
	//b.Blur_junzhi();
	//b.GaussianBlur();
	//b.MedianBlur();

	//Arithmetic c(src);
	//c.Arithmetic_Sobel();
	//c.Arithmetic_Laplacian();
	//c.Arithmetic_Canny();
	//c.Arithmetic_HoughLines();
	//c.Arithmetic_HoughLinesP();

	//Histogram d(src);
	//d.Histogram_EqualizeHist();
	//d.Histogram_Calc();
	//Mat src2 = imread("piaohao2.jpg", 1);
	//d.Histogram_Compare(src,src2 );
	//d.Histogram_BackProject();

	//Contour e(src);
	//Mat temp = imread("match.jpg");
	//namedWindow("样本", 0);
	//imshow("样本", temp);
	//e.Match(temp);
	//e.FindContours();
	//e.ConvexHull();

	集成测试 a;
	Rect test = a.Need;
	a.~集成测试();

	Mat src2 = src.clone();
	int y = test.y + test.height - 90;
	int height = test.y + test.height - y;
	test.y = y;
	test.height = height;
	rectangle(src2, test, Scalar(148, 0, 211),5);
	imshow("最终", src2);

	Mat newpic = src(Range(test.y, test.y + height), Range(test.x, test.x + test.width));
	imshow("裁剪", newpic);
	cvtColor(newpic, newpic, 7);
	threshold(newpic, newpic, 110, 255, THRESH_BINARY);

	imshow("裁剪", newpic);
	/*GaussianBlur(newpic, newpic, Size(3, 3), 0, 0);
	imshow("裁剪", newpic);*/

	Mat kernel = getStructuringElement(0, Size(3, 3));
	dilate(newpic, newpic, kernel);
	//blur(newpic, newpic, Size(4, 4), Point(-1, -1));
	Mat kernel2 = getStructuringElement(0, Size(2, 2));
	erode(newpic, newpic, kernel2);

	//Canny(newpic, newpic, 10, 10 * 3, 3);
	///*Laplacian(newpic, newpic, 2, 1);
	//convertScaleAbs(newpic, newpic);*/
	imshow("裁剪", newpic);

	waitKey();
	return 0;
}