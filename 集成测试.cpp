#include "stdafx.h"
#include "���ɲ���.h"


���ɲ���::���ɲ���()
{
	/*��һ�ַ���*/
	//Mat dst;
	//Mat dst2;
	///*�Ҷ�*/
	//Mat src = imread("piaohao.jpg", 0);
	////imshow("ͼ1", src);
	//threshold(src, dst, 100, 255, THRESH_BINARY);
	//imshow("1.ͼ", dst);
	//medianBlur(dst, dst2, 5);
	////GaussianBlur(dst, dst, Size(43, 43), 0, 0);
	//imshow("1.2ͼ", dst2);


	/*�ڶ��ַ���*/
	Mat test_src2 = imread("piaohao.jpg", 1);
	//imshow("2.ԭͼ", test_src2);
	Mat test_dst;
	
	//�㷨		
	//1.�����ȡ����
	GaussianBlur(test_src2, test_dst, Size(9, 9), 0, 0);
	Mat test_dst2, test_dst3;
	cvtColor(test_dst, test_dst2, 7);
	Laplacian(test_dst2, test_dst3, 2, 3);
	convertScaleAbs(test_dst3, test_dst);
	imshow("2.1��˹ͼ", test_dst);

	//��ֵ���ڰ�
	//2.��ֵ�ڰ�ͼ��
	threshold(test_dst, test_src2, 10, 255, THRESH_BINARY);
	//threshold(test_dst, test_src2, 10, 255, THRESH_BINARY_INV);
	imshow("2.2��ֵͼ", test_src2);
	
	////����
	//blur(test_src2, test_dst, Size(49, 49), Point(-1, -1));
	//GaussianBlur(src, dst, Size(3, 3), 0, 0);
	//imshow("2.3����ͼ", test_dst);

	//����ֻ�������
	//3.͹����������
	Mat kernel = getStructuringElement(0, Size(13, 13));
	dilate(test_src2, test_src2, kernel);
	imshow("2.4����", test_src2);

	//contourArea();
	RotatedRect a;
	//a = minAreaRect(test_src2);

	//��������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// ��Canny���Ӽ���Ե
	Canny(test_src2, test_dst, thresh, thresh * 2, 3);
	imshow("Canny����", test_dst);
	/// Ѱ������
	findContours(test_dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	int Max  = 0;
	//Rect Need;
	for (int i= 0 ; i < contours.size()-1; i++) {
		Rect a = boundingRect(contours[i]);
		if ((a.width * a.height) > Max)
		{
			Max = a.width * a.height;
			this->Need = a;
		}
	}
	rectangle(test_dst, this->Need, Scalar(148, 0, 211));
	imshow("drawing", test_dst);
}


���ɲ���::~���ɲ���()
{
}
