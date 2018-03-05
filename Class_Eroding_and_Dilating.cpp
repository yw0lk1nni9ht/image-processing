#include "stdafx.h"
#include "Class_Eroding_and_Dilating.h"
#include <math.h>

#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"highgui.h"
using namespace cv;

/*��ʴ����*/
Mat src2;
Mat dst_Dilate;
Mat dst_Erode;
Mat kernel;
int para1;
int para2;
void E_and_D2(int ,void *);

/*������̬*/
Mat dst_Other;
int op;
void Morphology2(int, void *);

/*�Ŵ���С*/
Mat dst_Up;
Mat dst_Down;
int num_Up;
int num_Down;
void Up(int ,void *);
void Down(int, void *);
Mat temp;

/*��ֵ����*/
Mat dst_Threshold;
int threshold_value;
int threshold_type;
void Threshold2(int, void *);

/*�����˲���*/
Mat dst_filter;
int Self_Filter_size;
void Fileter2(int, void *);


Class_Eroding_and_Dilating::Class_Eroding_and_Dilating(Mat _src)
{
	src2 = _src;
}

/*��ʴ����*/
void Class_Eroding_and_Dilating::E_and_D()
{
	E_and_D2(0,0);
}

void E_and_D2(int ,void *) {
	namedWindow("������", 0);
	namedWindow("ԭͼ��ʾ", 0);
	namedWindow("������ʾ", 0);
	namedWindow("��ʴ��ʾ", 0);
	imshow("ԭͼ��ʾ", src2);
	createTrackbar("�����С����", "������", &para1, 50, E_and_D2);
	createTrackbar("��������", "������", &para2, 2, E_and_D2);
	if (para1 == 0)
		return;
	if (para2 == 0) {
		/*kernel = getStructuringElement(0, Size(para1*2+1, para1 * 2 + 1));*/
		kernel = getStructuringElement(0, Size(para1 , para1 ));
	}
	if (para2 == 1) {
		//kernel = getStructuringElement(1, Size(para1 * 2 + 1, para1 * 2 + 1));
		kernel = getStructuringElement(1, Size(para1 * 2 + 1, para1 * 2 + 1));
	}
	if (para2 == 2) {
		//kernel = getStructuringElement(2, Size(para1 * 2 + 1, para1 * 2 + 1));
		kernel = getStructuringElement(2, Size(para1 * 2 + 1, para1 * 2 + 1));
	}
	dilate(src2, dst_Dilate, kernel);
	erode(src2, dst_Erode, kernel);
	imshow("������ʾ", dst_Dilate);
	imshow("��ʴ��ʾ", dst_Erode);
}


/*������̬*/
void Class_Eroding_and_Dilating::Morphology()
{
	Morphology2(0, 0);
}

void Morphology2(int, void *)
{
	namedWindow("��̬�任",0);
	namedWindow("������̬",0);
	createTrackbar("������", "��̬�任", &op, 4, Morphology2);
	createTrackbar("��ֵ", "��̬�任", &para1, 100, Morphology2);
	int operation = op + 2;
	if (para1 == 0)
		para1 = 1;
	kernel = getStructuringElement(0, Size(para1, para1));
	morphologyEx(src2, dst_Other, operation, kernel);
	imshow("������̬", dst_Other);
}


/*�Ŵ���С*/
void Class_Eroding_and_Dilating::Up_and_Down() {
	Mat tmp_Up;
	Mat tmp_Down;
	namedWindow("����̨", 0);
	temp = src2;
	
	createTrackbar("�Ŵ���", "����̨", &num_Up, 20,Up);
	createTrackbar("��С����", "����̨", &num_Down, 20, Down);

	namedWindow("�Ŵ�", 0);
	namedWindow("��С", 0);
}

void Up(int ,void *)
{
	pyrUp(temp, dst_Up, Size(temp.cols * 2, temp.rows * 2));
	temp = dst_Up;
	imshow("�Ŵ�", dst_Up);
}

void Down(int, void *)
{
	Mat temp;
	temp = src2;
	
	for (int i = 0; i<num_Down; i++)
	{
		pyrDown(temp, dst_Up, Size(temp.cols / 2, temp.rows / 2));
		temp = dst_Up;
	}
	imshow("��С", dst_Up);
}


/*��ֵ����*/
void Class_Eroding_and_Dilating::Threshold() {
	namedWindow("����", 0);
	namedWindow("��ʾ", 0);
	createTrackbar("��ֵ��С", "����", &threshold_value, 255, Threshold2);
	createTrackbar("��ֵ����", "����", &threshold_type, 4, Threshold2);
}

void Threshold2(int, void *) {
	threshold(src2, dst_Threshold,threshold_value,255,threshold_type);
	imshow("��ʾ", dst_Threshold);
}


/*�����˲���*/
void Class_Eroding_and_Dilating::Self_Filter()
{
	namedWindow("�˲���", 0);
	namedWindow("������");
	createTrackbar("�ں�����", "������", &Self_Filter_size, 2000, Fileter2);
}

void Fileter2(int ,void *)
{
	int kernel_size = 3 + 2 * (Self_Filter_size % 5);
	kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);
	filter2D(src2, dst_filter, -1, kernel);
	imshow("�˲���", dst_filter);
}

Class_Eroding_and_Dilating::~Class_Eroding_and_Dilating()
{
}
