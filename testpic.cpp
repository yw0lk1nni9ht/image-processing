// testpic.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

//Mat src;


/*腐蚀与膨胀*/
Mat src, dst, tmp;
char* window_name = "Pyramids Demo";

int main()
{
	// 载入原图像
	src = imread("pic2.jpg", 1);

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
	
	Blur b(src);
	//b.View();
	//b.BilateralFilter();
	//b.Blur_junzhi();
	//b.GaussianBlur();
	//b.MedianBlur();

	Arithmetic c(src);
	//c.Arithmetic_Sobel();
	//c.Arithmetic_Laplacian();
	//c.Arithmetic_Canny();


	waitKey();
	return 0;
}