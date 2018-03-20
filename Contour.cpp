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

//ƥ��
int Match_Method = 0;
Mat Match_temp;
void Match_Change(int, void *);

//Ѱ������
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
void FindContours_callback(int, void *);

//����͹��
void ConvexHull_callback(int, void *);

Contour::Contour(Mat _src)
{
	Contour_Src = _src;
}

///ƥ��
void Contour::Match(Mat _temp) {
	Match_temp = _temp;
	namedWindow("ƥ�䴰��", 0);
	createTrackbar("ƥ�䷽��", "ƥ�䴰��", &Match_Method, 5, Match_Change);
	Match_Change(0, 0);
}
void Match_Change(int ,void *) {
	Mat result;
	Mat _Contour_Src;
	_Contour_Src = Contour_Src.clone();
	//_Contour_Src.clone();
	matchTemplate(_Contour_Src, Match_temp, result, Match_Method); //0-5

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// ͨ������ minMaxLoc ��λ��ƥ���λ��
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
	/// ���ҿ����������ս��
	rectangle(_Contour_Src, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + Match_temp.cols, matchLoc.y + Match_temp.rows), Scalar::all(0), 2, 8, 0);
	
	//imshow("ƥ�䴰��", result);
	imshow("ƥ�䴰��", _Contour_Src);
}


/// Ѱ������
void Contour::FindContours() {
	/// ת�ɻҶȲ�ģ��������
	cvtColor(Contour_Src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	/// ��������
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

	/// ��Canny���Ӽ���Ե
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	imshow("Canny����", canny_output);
	/// Ѱ������
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// �������
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
	}
	/// �ڴ�������ʾ���
	namedWindow("Contours", 0);
	imshow("Contours", drawing);

}


//����͹��
void Contour::ConvexHull() {
	/// ת�ɻҶ�ͼ������ģ������
	cvtColor(Contour_Src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// ��������
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

	/// ��ͼ����ж�ֵ��
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	imshow("threshold", threshold_output);
	/// Ѱ������
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// ��ÿ������������͹��
	vector<vector<Point> >hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
	}

	/// �����������͹��
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}

	/// �ѽ����ʾ�ڴ���
	namedWindow("Hull demo", 0);
	imshow("Hull demo", drawing);
}

Contour::~Contour()
{
}
