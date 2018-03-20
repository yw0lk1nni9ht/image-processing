#include "stdafx.h"
#include "Blur.h"

#include<opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"

using namespace cv;


//����
Mat Blur_Src;
int Blur_i;
void callback(int, void *);
Mat dst_blur;
Mat dst_GaussianBlur;
Mat dst_medianBlur;
Mat dst_bilateralFileter;

Blur::Blur(Mat _src)
{
	Blur_Src = _src;
}


void Blur::View()
{
	namedWindow("���ƻ���");
	createTrackbar("ֵ", "���ƻ���", &Blur_i, 100, callback);

	namedWindow("��ֵƽ����ʾ", 0);
	namedWindow("��˹ƽ����ʾ", 0);
	namedWindow("��ֵƽ����ʾ", 0);
	namedWindow("˫��ƽ����ʾ", 0);
}


void callback(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	///ʹ�þ�ֵƽ��
	blur(Blur_Src, dst_blur, Size(Blur_i, Blur_i), Point(-1, -1));
	imshow("��ֵƽ����ʾ", dst_blur);

	/// ʹ�ø�˹ƽ��
	GaussianBlur(Blur_Src, dst_GaussianBlur, Size(Blur_i, Blur_i), 0, 0);
	imshow("��˹ƽ����ʾ", dst_GaussianBlur);

	/// ʹ����ֵƽ��
	medianBlur(Blur_Src, dst_medianBlur, Blur_i);
	imshow("��ֵƽ����ʾ", dst_medianBlur);

	/// ʹ��˫��ƽ��
	bilateralFilter(Blur_Src, dst_bilateralFileter, Blur_i, Blur_i * 2, Blur_i / 2);
	imshow("˫��ƽ����ʾ", dst_bilateralFileter);
}

//��ֵ����
void blur_change(int ,void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	blur(Blur_Src, dst_blur, Size(Blur_i, Blur_i), Point(-1, -1));
	imshow("��ֵƽ������",dst_blur);
}
void Blur::Blur_junzhi() {
	namedWindow("��ֵƽ������", 0);
	createTrackbar("ֵ", "��ֵƽ������", &Blur_i, 100, blur_change);
}

//��˹����
void Gaussian_change(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	GaussianBlur(Blur_Src, dst_GaussianBlur, Size(Blur_i, Blur_i), 0, 0);
	imshow("��˹ƽ������", dst_GaussianBlur);
}
void Blur::GaussianBlur() {
	namedWindow("��˹ƽ������", 0);
	createTrackbar("ֵ", "��˹ƽ������", &Blur_i, 100, Gaussian_change);
}


//��ֵ����
void MedianBlur_change(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	medianBlur(Blur_Src, dst_medianBlur, Blur_i);
	imshow("��ֵƽ������", dst_medianBlur);
}
void Blur::MedianBlur() {
	namedWindow("��ֵƽ������", 0);
	createTrackbar("ֵ", "��ֵƽ������", &Blur_i, 100, MedianBlur_change);
}


//˫�ߴ���
void BilateralFilter_change(int, void *) {
	if (Blur_i % 2 == 0)
	{
		Blur_i++;
	}
	bilateralFilter(Blur_Src, dst_bilateralFileter, Blur_i, Blur_i * 2, Blur_i / 2);
	imshow("˫��ƽ������", dst_bilateralFileter);
}
void Blur::BilateralFilter() {
	namedWindow("˫��ƽ������", 0);
	createTrackbar("ֵ", "˫��ƽ������", &Blur_i, 100, BilateralFilter_change);
}


Blur::~Blur()
{
}
