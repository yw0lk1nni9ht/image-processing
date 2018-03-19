#include "stdafx.h"
#include "Contour.h"



/*************************************
//									//
//									//
//			ͼ��ƥ�����				//
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
	namedWindow("ƥ�䴰��", 0);
	createTrackbar("ƥ�䷽��", "ƥ�䴰��", &Match_Method, 5, Match_Change);
	Match_Change(0, 0);
}
void Match_Change(int ,void *) {
	Mat result;
	Mat _Contour_Src;
	_Contour_Src = Contour_Src;
	matchTemplate(_Contour_Src, Match_temp, result, Match_Method); //0-5

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// ͨ������ minMaxLoc ��λ��ƥ���λ��
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	/// ���ҿ����������ս��
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
	//imshow("ƥ�䴰��", result);
	imshow("ƥ�䴰��", _Contour_Src);
}

Contour::~Contour()
{
}
