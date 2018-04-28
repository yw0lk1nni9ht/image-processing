#pragma once

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"
#include "Histogram.h"
#include "Contour.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class 集成测试
{
public:
	//获取票号区域并返回矩形
	集成测试(Mat _src, int type);	//type决定2种票对应得内核选择
		
	//一系列处理获取号码框
	void pichandle(Mat handle_src);
	//号码框过滤并排序
	void clean();
	

	Rect Need;		//区域的框
	vector<Rect> saved_contours;		//存放所需要的框形




	~集成测试();
	
	
};

