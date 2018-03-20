#include "stdafx.h"
#include "Arithmetic.h"

#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;



/*************************************
//									//
//									//
//			ͼ���㷨					//
//									//
//									//
*************************************/


//ȫ�ֱ���
Mat Arithmetic_Src;

//Sobel
int alpha;
int beta;
int gamma;

//Laplacian
int _size = 0;
int color = 0;
int kernel_size = 3;
int ddepth = CV_16S;

//Canny
int lowThreshold;

//HoughLines



Arithmetic::Arithmetic(Mat _src)
{
	Arithmetic_Src = _src;
}

//Sobel
void Sobel_callback(int, void *) {
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat grad;
	/// �� X�����ݶ�
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(Arithmetic_Src, grad_x, ddepth, 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);

	namedWindow("x", 0);
	imshow("x",abs_grad_x);

	/// ��Y�����ݶ�
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(Arithmetic_Src, grad_y, ddepth, 0, 1, 3); 

	convertScaleAbs(grad_y, abs_grad_y);
	namedWindow("y", 0);
	imshow("y",abs_grad_y);

	/// �ϲ��ݶ�(����)
	//addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	double a = alpha / 100.0;
	double b = beta / 100.0;
	double c = gamma / 100.0;
	addWeighted(abs_grad_x, a, abs_grad_y,b, gamma, grad);
	imshow("Sobel����ͼ", grad);
}
void Arithmetic::Arithmetic_Sobel() {
	namedWindow("Sobel�㷨����������", 0);
	namedWindow("Sobel����ͼ", 0);
	createTrackbar("alpha", "Sobel�㷨����������", &alpha, 100, Sobel_callback);
	createTrackbar("beta", "Sobel�㷨����������", &beta, 100, Sobel_callback);
	createTrackbar("gamma", "Sobel�㷨����������", &gamma, 100, Sobel_callback);
}


//Laplacian
void Laplacian_callback(int, void *)
{
	if (_size % 2 == 0)
	{
		_size++;
	}
	if (ddepth == 0 || ddepth == 1)
	{
		return;
	}
	Mat dst;
	Mat dst2;
	Mat dst3;
	GaussianBlur(Arithmetic_Src, dst, Size(_size, _size), 0, 0);
	cvtColor(dst, dst2, 7);
	Laplacian(dst2, dst3, ddepth, kernel_size);
	convertScaleAbs(dst3, dst3);
	imshow("Laplacian����ͼ", dst3);
}
void Arithmetic::Arithmetic_Laplacian() {
	
	namedWindow("Laplacian�㷨����������",0);
	namedWindow("Laplacian����ͼ", 0);
	createTrackbar("��˹size", "Laplacian�㷨����������", &_size, 50, Laplacian_callback);
	//createTrackbar("�Ҷ�Ϊ7", "Sobel�㷨����������", &color, 143, Sobel_callback);	
	createTrackbar("�ں����", "Laplacian�㷨����������", &ddepth, 3, Laplacian_callback);
	//createTrackbar("�����ں˴�С", "Sobel�㷨����������", &kernel_size, 50, Sobel_callback);
}


//Canny
void Canny_callback(int, void *) {
	Mat src_gray;
	Mat detected_edges;
	Mat dst;
	cvtColor(Arithmetic_Src, src_gray, CV_BGR2GRAY);
	blur(src_gray, detected_edges, Size(3, 3));

	/// ����Canny����
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*3, kernel_size);

	/// ʹ�� Canny���������Ե��Ϊ������ʾԭͼ��
	dst = Scalar::all(0);

	Arithmetic_Src.copyTo(dst, detected_edges);
	imshow("Canny��Ե", dst);
}
void Arithmetic::Arithmetic_Canny()
{
	/// ������ʾ����
	namedWindow("Canny��Ե", 0);

	/// ����trackbar
	createTrackbar("Min Threshold:", "Canny��Ե", &lowThreshold, 200, Canny_callback);

}


//HoughLines
void Arithmetic::Arithmetic_HoughLines(){
	Mat dst;
	Mat dst2;
	Mat ccdst;

	Canny(Arithmetic_Src, dst, 50, 200, 3);

	cvtColor(dst, ccdst, CV_GRAY2BGR);

	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 150, 0,0);
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(ccdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
		//line(ccdst, pt1, pt2, Scalar(55, 100, 195), 1, CV_AA);
	}
	namedWindow("houghlines ", 0);
	imshow("houghlines", ccdst);
}

//HoughLines
void Arithmetic::Arithmetic_HoughLinesP() {
	Mat dst;
	Mat dst2;
	Mat ccdst;

	Canny(Arithmetic_Src, dst, 50, 200, 3);

	cvtColor(dst, ccdst, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 150, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(ccdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
		//line(ccdst, pt1, pt2, Scalar(55, 100, 195), 1, CV_AA);
	}
	imshow("houghlinesP", ccdst);

}





Arithmetic::~Arithmetic()
{
}
