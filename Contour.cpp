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

//匹配
int Match_Method;
Mat Match_temp;
void Match_Change(int, void *);

//轮廓
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

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
	_Contour_Src = Contour_Src.clone();
	matchTemplate(_Contour_Src, Match_temp, result, Match_Method); //0-5

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	
	/// 通过函数 minMaxLoc 定位最匹配的位置
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	
	//matchLoc = minLoc;
	if (Match_Method == CV_TM_SQDIFF || Match_Method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	/// 让我看看您的最终结果
	rectangle(_Contour_Src, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	
	imshow("result", result);
	imshow("匹配窗口", _Contour_Src);
}


void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// 用Canny算子检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// 寻找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// 绘出轮廓
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// 在窗体中显示结果
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}
void Contour::FindContour() {
	
	/// 转成灰度并模糊化降噪
	cvtColor(Contour_Src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// 创建窗体
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, Contour_Src);

	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);
}
Contour::~Contour()
{
}
