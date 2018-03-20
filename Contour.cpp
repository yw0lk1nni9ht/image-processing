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
int Match_Method = 0;
Mat Match_temp;
void Match_Change(int, void *);

//寻找轮廓
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
void FindContours_callback(int, void *);

//计算凸包
void ConvexHull_callback(int, void *);

Contour::Contour(Mat _src)
{
	Contour_Src = _src;
}

///匹配
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
	//_Contour_Src.clone();
	matchTemplate(_Contour_Src, Match_temp, result, Match_Method); //0-5

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// 通过函数 minMaxLoc 定位最匹配的位置
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	if (Match_Method == CV_TM_SQDIFF || Match_Method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	//matchLoc = minLoc;
	/// 让我看看您的最终结果
	rectangle(_Contour_Src, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	
	//imshow("匹配窗口", result);
	imshow("匹配窗口", _Contour_Src);
}


/// 寻找轮廓
void Contour::FindContours() {
	/// 转成灰度并模糊化降噪
	cvtColor(Contour_Src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	/// 创建窗体
	char* source_window = "Source";
	namedWindow(source_window, 0);
	imshow(source_window, Contour_Src);

	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, FindContours_callback);
	FindContours_callback(0, 0);
}
void FindContours_callback(int, void *) {
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// 用Canny算子检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	imshow("Canny算子", canny_output);
	/// 寻找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// 绘出轮廓
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
	}
	/// 在窗体中显示结果
	namedWindow("Contours", 0);
	imshow("Contours", drawing);

}


//计算凸包
void Contour::ConvexHull() {
	/// 转成灰度图并进行模糊降噪
	cvtColor(Contour_Src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// 创建窗体
	char* source_window = "Source";
	namedWindow(source_window, 0);
	imshow(source_window, Contour_Src);

	createTrackbar(" Threshold:", "Source", &thresh, max_thresh, ConvexHull_callback);
	ConvexHull_callback(0, 0);
}
void ConvexHull_callback(int, void *) {
	Mat src_copy = Contour_Src.clone();
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// 对图像进行二值化
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	imshow("threshold", threshold_output);
	/// 寻找轮廓
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// 对每个轮廓计算其凸包
	vector<vector<Point> >hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
	}

	/// 绘出轮廓及其凸包
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}

	/// 把结果显示在窗体
	namedWindow("Hull demo", 0);
	imshow("Hull demo", drawing);
}

Contour::~Contour()
{
}
