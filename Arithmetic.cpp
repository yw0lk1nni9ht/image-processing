#include "stdafx.h"
#include "Arithmetic.h"

#include <opencv2\opencv.hpp>
using namespace cv;


Mat Arithmetic_Src;
int alpha;
int beta;
int gamma;

int _size = 0;
int color = 0;
int kernel_size = 3;
int ddepth = CV_16S;

int lowThreshold;


Arithmetic::Arithmetic(Mat _src)
{
	Arithmetic_Src = _src;
}

void Sobel_callback(int, void *) {
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat grad;
	/// �� X�����ݶ�
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(Arithmetic_Src, grad_x, ddepth, 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);

	/// ��Y�����ݶ�
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(Arithmetic_Src, grad_y, ddepth, 0, 1, 3);
	convertScaleAbs(grad_y, abs_grad_y);

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


void Laplacian_callback(int, void *)
{
	if (_size % 2 == 0)
	{
		_size++;
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
	//createTrackbar("�ں����", "Sobel�㷨����������", &ddepth, 50, Sobel_callback);
	//createTrackbar("�����ں˴�С", "Sobel�㷨����������", &kernel_size, 50, Sobel_callback);
}


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

Arithmetic::~Arithmetic()
{
}
