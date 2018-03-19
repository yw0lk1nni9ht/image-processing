#include "stdafx.h"
#include "Contour.h"



/*************************************
//									//
//									//
//			图像匹配相关				//
//									//
//									//
*************************************/
Mat Contour_Src;
int Match_Method;
Mat Match_temp;
void Match_Change(int, void *);


Contour::Contour(Mat _src)
{
	Contour_Src = _src;
}

void Contour::Match(Mat _temp) {
	Match_temp = _temp;
	namedWindow("匹配窗口", 0);
	createTrackbar("匹配方法", "匹配窗口", &Match_Method, 5, Match_Change);
	Match_Change(0, 0);
}
void Match_Change(int ,void *) {
	Mat result;
	Mat _Contour_Src;
	_Contour_Src = Contour_Src;
	matchTemplate(_Contour_Src, Match_temp, result, Match_Method); //0-5

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// 通过函数 minMaxLoc 定位最匹配的位置
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	/// 让我看看您的最终结果
	rectangle(_Contour_Src, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	if (Match_Method == CV_TM_SQDIFF || Match_Method == CV_TM_SQDIFF_NORMED)
	{
	matchLoc = minLoc;
	}
	else
	{
	matchLoc = maxLoc;
	}
	//imshow("匹配窗口", result);
	imshow("匹配窗口", _Contour_Src);
}

Contour::~Contour()
{
}
