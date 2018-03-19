// testpic.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"
#include "Histogram.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;



int main()
{
	// 载入原图像
	//Mat src = imread("testpicture\\num2.jpg", 1);
	Mat src = imread("piaohao.jpg", 1);
	



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
	
	/*Blur b(src);
	b.View();
	b.BilateralFilter();
	b.Blur_junzhi();
	b.GaussianBlur();
	b.MedianBlur();*/

	//Arithmetic c(src);
	//c.Arithmetic_Sobel();
	//c.Arithmetic_Laplacian();
	//c.Arithmetic_Canny();
	//c.Arithmetic_HoughLines();
	//c.Arithmetic_HoughLinesP();
	//c.Arithmetic_EqualizeHist();

	//Histogram d(src);
	//d.Histogram_EqualizeHist();
	//d.Histogram_Calc();
	//Mat src2 = imread("piaohao2.jpg", 1);
	//d.Histogram_Compare(src,src2 );
	//d.Histogram_BackProject();

	Mat temp = imread("match.jpg");
	Mat result;
	matchTemplate(src, temp, result, 0); //0-5
	
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	imshow("result", result);

	/// 通过函数 minMaxLoc 定位最匹配的位置
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	/// 让我看看您的最终结果
	rectangle(src, matchLoc, Point(matchLoc.x + temp.cols, matchLoc.y + temp.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + temp.cols, matchLoc.y + temp.rows), Scalar::all(0), 2, 8, 0);
	/*if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}*/
	imshow("result", src);
	waitKey();
	return 0;
}