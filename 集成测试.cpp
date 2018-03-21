#include "stdafx.h"
#include "集成测试.h"


集成测试::集成测试()
{
	/*第一种方法*/
	//Mat dst;
	//Mat dst2;
	///*灰度*/
	//Mat src = imread("piaohao.jpg", 0);
	////imshow("图1", src);
	//threshold(src, dst, 100, 255, THRESH_BINARY);
	//imshow("1.图", dst);
	//medianBlur(dst, dst2, 5);
	////GaussianBlur(dst, dst, Size(43, 43), 0, 0);
	//imshow("1.2图", dst2);


	/*第二种方法*/
	Mat test_src2 = imread("piaohao.jpg", 1);
	//imshow("2.原图", test_src2);
	Mat test_dst;
	
	//算法		
	//1.降噪获取轮廓
	GaussianBlur(test_src2, test_dst, Size(9, 9), 0, 0);
	Mat test_dst2, test_dst3;
	cvtColor(test_dst, test_dst2, 7);
	Laplacian(test_dst2, test_dst3, 2, 3);
	convertScaleAbs(test_dst3, test_dst);
	imshow("2.1高斯图", test_dst);

	//二值，黑白
	//2.二值黑白图像
	threshold(test_dst, test_src2, 10, 255, THRESH_BINARY);
	//threshold(test_dst, test_src2, 10, 255, THRESH_BINARY_INV);
	imshow("2.2二值图", test_src2);
	
	////降噪
	//blur(test_src2, test_dst, Size(49, 49), Point(-1, -1));
	//GaussianBlur(src, dst, Size(3, 3), 0, 0);
	//imshow("2.3降噪图", test_dst);

	//区域分化更明显
	//3.凸出所需区域
	Mat kernel = getStructuringElement(0, Size(13, 13));
	dilate(test_src2, test_src2, kernel);
	imshow("2.4膨胀", test_src2);

	//contourArea();
	RotatedRect a;
	//a = minAreaRect(test_src2);

	//查找轮廓
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// 用Canny算子检测边缘
	Canny(test_src2, test_dst, thresh, thresh * 2, 3);
	imshow("Canny算子", test_dst);
	/// 寻找轮廓
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


集成测试::~集成测试()
{
}
