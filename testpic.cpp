 // testpic.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include"Class_Fileter.h"
#include"Class_Eroding_and_Dilating.h"
#include "Blur.h"
#include "Arithmetic.h"
#include "Histogram.h"
#include "Contour.h"
#include "Feature.h"
#include "MachineLearning.h"

#include "集成测试.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

//声明区域
Mat cut(Mat src, Rect test);
//int转string,固定位数
String changestring(int i);
//处理另一种票号
void other_invoke(Mat src, int *i, float *fail_num, float *predict_num, Ptr<SVM>svm);
//广东通用机打发票
void GDTY_invoke(Mat src, int *i, float *fail_num, float *predict_num, Ptr<SVM>svm);
//通用机打发票
void TY_invoke(Mat src, int *i, float *fail_num, float *predict_num, Ptr<SVM>svm);
/*通用函数*/
Mat pic_init(Mat src, Size blur_size, int _thresh, Size kernel_size);	
vector<Rect> Sort_by_X(Mat src,vector<vector<Point>> contours,int area, int reduce_x, int reduce_y, int width, int height);		//根据x坐标排序轮廓

int main()
{
	Ptr<SVM>svm = Algorithm::load<SVM>("svm.xml");

	float predict_num = 0;	//识别次数，用于成功率计算
	float fail_num = 0;		//失败次数
	for (int i = 92; i < 103; i++)
	{
		// 载入原图像
		//Mat src = imread("C:\\Users\\ywin\\Desktop\\2017-12-25 15.38.49 No.13110657.jpg", 1);
		//Mat src = imread("100张样本\\" + std::to_string(i) + ".jpg", 1);
		//Mat src = imread("1000张样本\\test ("  + to_string(i) + ")" + ".jpg", 1);
		Mat src = imread("其他样本\\test_img_1 (" + to_string(i) + ")" + ".jpg", 1);
		//Mat src = imread("其他样本\\002 (1).jpg", 1);
		//Mat src = imread("pic2.jpg", 1);
		

	

		//平滑处理
		/*Class_Fileter a;
		a._src = src;
		a._i = 1;
		a._max_value = 71;
		a.first();*/

		//膨胀腐蚀等形态变化
		/*Class_Eroding_and_Dilating b(src);
		//膨胀
		b.E_and_D();
		//更多形态变化
		b.Morphology();
		//放大和缩小
		b.Up_and_Down();
		//阈值操作
		b.Threshold();
		//自制滤波
		b.Self_Filter();
		*/

		//降噪处理
		/*Blur b(src);
		b.View();
		b.BilateralFilter();
		b.Blur_junzhi();
		b.GaussianBlur();
		b.MedianBlur();
		*/

		//获取轮廓算法 
		/*
		Arithmetic c(src);
		c.Arithmetic_Sobel();
		c.Arithmetic_Laplacian();
		c.Arithmetic_Canny();
		c.Arithmetic_HoughLines();
		c.Arithmetic_HoughLinesP();
		*/

		//直方图
		/*
		Histogram d(src);
		d.Histogram_EqualizeHist();
		d.Histogram_Calc();
		Mat src2 = imread("piaohao2.jpg", 1);
		d.Histogram_Compare(src,src2 );
		d.Histogram_BackProject();
		*/

		//匹配相关
		/*src = imread("单字\\第11张：0.jpg");
		Contour e(src);
		Mat temp = imread("模板\\0\\0.jpg");
		namedWindow("样本", 0);
		imshow("样本", temp);
		e.Match(temp);
		//e.FindContours();
		//e.ConvexHull();*/

		//特征寻找，匹配
		/*
		//Feature a;
		//a.FindFeature();
		//a.MatchFeature();
		*/

		//机器学习

		//MachineLearning ML;
		//ML.KNN_test();
		//ML.SVM_test();
		
		//GDTY_invoke(src, &i, &fail_num, &predict_num,svm);		//1000张样本
		
		//other_invoke(src, &i, &fail_num, &predict_num,svm);

		TY_invoke(src, &i, &fail_num, &predict_num, svm);
		//waitKey();
	}

	waitKey();
	return 0;
}

//裁剪图片
Mat cut(Mat src, Rect test){
	Mat src2 = src.clone();
	int y = test.y + test.height - 90;
	int height = test.y + test.height - y;
	test.y = y;
	test.height = height;
	//rectangle(src2, test, Scalar(148, 0, 211), 2);
	//imshow("最终", src2);

	Mat newpic = src(Range(test.y, test.y + height), Range(test.x, test.x + test.width));
	//imshow("裁剪", newpic);


	return newpic;
}

String changestring(int i) {
	if (i < 1000)
	{
		if (i < 100)
		{
			if (i < 10)
			{
				return "000" + to_string(i);
			}
			return "00" + to_string(i);
		}
		return "0" + to_string(i);
	}
}

void other_invoke(Mat src, int *i, float *fail_num, float *predict_num, Ptr<SVM>svm) {
	集成测试 a(src);
	Mat newpic = src(Range(a.Need.y, a.Need.y + a.Need.height), Range(a.Need.x, a.Need.x + a.Need.width));	//裁剪到剩下票号得矩阵图
	//a.pichandle(newpic);

	//pichandle		/***	截取数字区域		***/
	Mat save = newpic.clone();

	#pragma region 1.初始化处理

	cvtColor(newpic, newpic, 7);	//转成灰度
	threshold(newpic, newpic, 150, 255, THRESH_BINARY);		//二值化

#pragma endregion

	#pragma region 2.无需去背景干扰 ，腐蚀使票号更明显
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	//dilate(newpic, newpic, kernel);
	////imshow("2膨胀", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(3, 3));	//腐蚀
	erode(newpic, newpic, kernel2);
	////imshow("2腐蚀", handle_src);
	//Mat kernel3 = getStructuringElement(0, Size(5, 5));	//膨胀
	//dilate(newpic, newpic, kernel3);
	////imshow("3膨胀", handle_src);
	//Mat kernel4 = getStructuringElement(0, Size(3, 3));	//腐蚀
	//erode(newpic, newpic, kernel4);
	//////imshow("3腐蚀", handle_src);
#pragma endregion

	#pragma region 3.检测边缘

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	//用Canny算子检测边缘
	Canny(newpic, canny_output, 10, 10 * 2, 3);
	//imshow("Canny算子", canny_output);

#pragma endregion

	#pragma region 4.寻找轮廓

	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

#pragma	endregion

	#pragma region 5.筛选轮廓

	for (int i = 0; i < contours.size(); i++) {
		Rect aa = boundingRect(contours[i]);
		if (aa.area() > 100)
		{
			//rectangle(save, aa, Scalar(255, 64, 64));
			a.saved_contours.push_back(aa);
		}
	}
	imshow("Contours", save);

#pragma endregion

	//clean			/*** 清理轮廓	***/
	//根据x坐标，排序
	int _contours = a.saved_contours.size();
	for (int i = 0; i < _contours - 1; i++)
	{
		int saveindex = i;
		int min = a.saved_contours[i].x;
		for (int j = i + 1; j < _contours; j++)
		{
			if (min > a.saved_contours[j].x)
			{
				min = a.saved_contours[j].x;
				saveindex = j;
			}
		}
		if (saveindex != i) {
			Rect temp = a.saved_contours[i];
			a.saved_contours[i] = a.saved_contours[saveindex];
			a.saved_contours[saveindex] = temp;
		}
	}

	//清除同一数字下多个轮廓的不精确轮廓；只保留最最上角的
	vector<Rect>::iterator element_to_delete = a.saved_contours.begin();
	for (int i = 0; i < _contours - 1; i++) {
		//下一个轮廓的x处于这个轮廓的x范围里，证明不是所需要的下一个轮廓，所以就删除
		//如果是下半轮廓，把y值设置成上半轮廓的y值
		if ((a.saved_contours[i + 1].x <= a.saved_contours[i].x + a.saved_contours[i].width)
			&& (a.saved_contours[i + 1].x >= a.saved_contours[i].x))
		{
			if (a.saved_contours[i].y > a.saved_contours[i + 1].y)
			{
				a.saved_contours[i].y = a.saved_contours[i + 1].y;
			}
			element_to_delete + 1 = a.saved_contours.erase(element_to_delete + 1);
			_contours--;
			i--;
			continue;
		}
		element_to_delete++;
	}

	

	//获取轮廓左上角坐标，重绘轮廓大小
	for (int i = 0; i < _contours; i++) {
		a.saved_contours[i].x -= 6;
		a.saved_contours[i].y -= 6;
		a.saved_contours[i].width = 35;
		a.saved_contours[i].height = 65;
	}

	
	//获取最后的8个矩形(清理)
	if (a.saved_contours.size() > 8)
	{
		int delete_num = a.saved_contours.size() - 8;
		for (int i = 0; i < delete_num ; i++) {
			a.saved_contours.begin()  = a.saved_contours.erase(a.saved_contours.begin());
		}
	}
	for (int i = 0; i < a.saved_contours.size(); i++) {
		Rect aa = a.saved_contours[i];
		rectangle(save, aa, Scalar(255, 64, 64));
	}
	imshow("Contours",save);

	/*显示每个数字*/
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("单字_test_cs\\第"+to_string(*i)+"张："+ to_string(j) + ".jpg", test);
		test.rows = 65;
		test.cols = 32;
		test.convertTo(test, CV_32FC1);
		test = test.reshape(1, 1);
		Mat _tmp = Mat(1, 2500, CV_32FC1, Scalar(255, 255, 255));
		for (int i = 0; i < test.rows; i++)
		{
			float* mubiao = _tmp.ptr<float>(i);
			float* yangben = test.ptr<float>(i);
			for (int j = 0; j < test.cols; j++)
			{
				mubiao[j] = yangben[j];
			}
		}
		float response2 = svm->predict(_tmp);  //进行预测，返回1或-1,返回类型为float
		cout << response2;
	}
	cout << endl;
}

void GDTY_invoke(Mat src,int *i,float *fail_num,float *predict_num,Ptr<SVM>svm) {
	集成测试 a(src);
	Mat newpic = cut(src, a.Need);		//裁剪到剩下票号得矩阵图
										//newpic = imread("14110657.jpg", 1);
	a.pichandle(newpic);				//获取并筛选大0部分轮廓
	a.clean();							//对轮廓清洗和放大，留下最精确的8个票号轮廓

										//显示处理过后的轮廓
	for (int i = 0; i < a.saved_contours.size(); i++) {
		rectangle(newpic, a.saved_contours[i], Scalar(255, 64, 64));
	}
	imshow("new", newpic);

	/*显示每个数字*/
	cvtColor(newpic, newpic, 7);	//转成灰度
	threshold(newpic, newpic, 130, 255, THRESH_BINARY);		//二值化
															//2.去背景干扰
	Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	dilate(newpic, newpic, kernel);
	cout << "第" << *i << "张票号：";

	String predict = "";		//识别的结果
	String real = "6789";		//实际结果

	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("单字\\第"+to_string(i)+"张："+ to_string(j) + ".jpg", test);

		//需要识别的数字，要和训练库里的训练元素大小一直，即1x2000x1
		test.rows = 47;
		test.cols = 32;
		test.convertTo(test, CV_32FC1);
		test = test.reshape(1, 1);
		Mat _tmp = Mat(1, 2500, CV_32FC1, Scalar(255, 255, 255));
		for (int i = 0; i < test.rows; i++)
		{
			float* mubiao = _tmp.ptr<float>(i);
			float* yangben = test.ptr<float>(i);
			for (int j = 0; j < test.cols; j++)
			{
				mubiao[j] = yangben[j];
			}
		}
		float response2 = svm->predict(_tmp);  //进行预测，返回1或-1,返回类型为float
		cout << response2;
		predict += to_string((int)response2);	//预测的票号

#pragma region 注释的模板匹配雏形
												//Mat _test = test.clone();
												//cvtColor(test, _test, 8);
												//for (int method_num = 0 ; method_num < 5 ; method_num++) 
												//{
												//	vector<Point> percent;		
												//	Mat result;
												//	for (int moban = 0; moban < 10; moban++) 
												//	{		
												//		try
												//		{
												//			Mat temp = imread("模板\\" + to_string(moban) + "\\0.jpg");
												//			matchTemplate(_test, temp, result, method_num); //0-5
												//														   ///归一化
												//			normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
												//			/// 通过函数 minMaxLoc 定位最匹配的位置
												//			double minVal; double maxVal; Point minLoc; Point maxLoc;
												//			Point matchLoc;
												//			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
												//			if (method_num == CV_TM_SQDIFF || method_num == CV_TM_SQDIFF_NORMED)
												//			{
												//				matchLoc = minLoc;
												//			}
												//			else
												//			{
												//				matchLoc = maxLoc;
												//			}
												//			percent.push_back(matchLoc);
												//		}
												//		catch (exception e)
												//		{
												//			continue;
												//			// 处理 ExceptionName 异常的代码
												//		}
												//		
												//	}
												//}
#pragma endregion
	}
	//cout << "\t正确号码" << "6789" << setfill('0') << setw(4) << i;
	real = real + changestring(*i);
	cout << "\t正确号码" << real;
	*predict_num = *predict_num + 1;
	if (real.compare(predict) != 0)
		*fail_num = *fail_num + 1;
	cout << "\t成功率" << ((*predict_num - *fail_num) / *predict_num) * 100 << "%";		//0正确，其他不正确
	cout << endl << endl;
	//waitKey();
}

void TY_invoke(Mat src, int *i, float *fail_num, float *predict_num, Ptr<SVM>svm) {
	//原图的复制
	Mat src_clone = src.clone();

#pragma region 初始化图片并查找轮廓,画出最大矩形
	src = pic_init(src, Size(1, 1), 80, Size(20, 9));
	//查找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// 用Canny算子检测边缘
	Canny(src, src, thresh, thresh * 2, 3);
	/// 寻找轮廓
	findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//先排除其他明显不符合的过大矩形，找出第一行数字的矩形。
	int Max = 0;	//最大的矩形面积
	Rect rect;
	for (int i = 0; i < contours.size() - 1; i++) {
		Rect a = boundingRect(contours[i]);
		if (a.width > src.cols*0.7 || a.height > 80) {	//去除一些大的矩形
			continue;
		}
		if ((a.width * a.height) > Max)
		{
			Max = a.width * a.height;
			rect = a;
		}
	}
	rectangle(src, rect, Scalar(148, 0, 211));
#pragma endregion

	//裁剪一部分容易干扰区域
	Mat temp_1 = src_clone(Range(rect.y-10, src.rows),Range(0, src.cols));
	Mat temp_2 = temp_1;		//裁剪后的原图
#pragma region 裁剪原图，再次处理图像找出轮廓,找到2行数字票号,重绘所需号码区域
	temp_1 = pic_init(temp_1, Size(1, 1), 80, Size(20, 9));
	//查找轮廓
	int _thresh = 10;
	/// 用Canny算子检测边缘
	Canny(temp_1, temp_1, _thresh, _thresh * 2, 3);
	/// 寻找轮廓
	findContours(temp_1, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


	int Max_1 = 1;
	int Max_2 = 2;
	Rect rect_1 = boundingRect(contours[0]);
	Rect rect_2 = boundingRect(contours[1]);

	for (int i = 0; i < contours.size() - 1; i++) {
		Rect a = boundingRect(contours[i]);
		rectangle(temp_1, a, Scalar(148, 0, 211));
	}

	//先排除其他明显不符合的过大矩形，然后找出2行数字的矩形,重绘定位区域
	for (int i = 0; i < contours.size() - 1; i++) {
		Rect a = boundingRect(contours[i]);
		if (a.width > temp_1.cols*0.7 || a.height > 80) {	//去除一些大的矩形
			continue;
		}
		if ((a.width * a.height) > Max_2)
		{
			if ((a.width * a.height) > Max_1)
			{
				rect_2 = rect_1;
				Max_2 = Max_1;
				rect_1 = a;
				Max_1 = (a.width * a.height);
				continue;
			}
			Max_2 = a.width * a.height;
			rect_2 = a;
		}
	}
	/*rectangle(cut, rect_1, Scalar(148, 0, 211));
	rectangle(cut, rect_2, Scalar(148, 0, 211));*/
	if (rect_2.x >= rect_1.x) {
		rect_2.width = rect_2.width + rect_2.x - rect_1.x;
		rect_2.x = rect_1.x;	
	}
	rectangle(temp_1, rect_2, Scalar(148, 0, 211));
#pragma endregion

	Mat number_pic = temp_2(Range(rect_2.y, rect_2.y + rect_2.height), Range(rect_2.x-20, rect_2.x + rect_2.width+20));
	Mat temp3 = number_pic;			//号码行的原图
#pragma region 获取数字区域,并分离出单个数字并做保存或识别
	number_pic = pic_init(number_pic, Size(9, 9), 30, Size(3, 3));
	findContours(number_pic, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Rect> Need = Sort_by_X(number_pic,contours,80,5,5,34,50);
	//画矩形
	for (int i = 0; i < Need.size(); i++) {
		rectangle(number_pic, Need[i], Scalar(148, 0, 211));
	}
	//分离成单个数字
	cvtColor(temp3, temp3, 7);	//转成灰度
	threshold(temp3, temp3, 130, 255, THRESH_BINARY);		//二值化	
	for (int i = 0; i < Need.size(); i++)
	{
		Mat num_temp;
		num_temp = temp3(Range(Need[i].y, Need[i].y + Need[i].height), Range(Need[i].x, Need[i].x + Need[i].width));
	}
}


//通用初始化操作 降噪/转成灰度/获取轮廓/二值黑白/膨胀
Mat pic_init(Mat src,Size blur_size,int _thresh,Size kernel_size) {
	Mat _test1;
	//GaussianBlur(src, src, blur_size, 0, 0);
	cvtColor(src, src, 7);
	//equalizeHist(src, src);
	Laplacian(src, _test1, 2, 3);
	convertScaleAbs(_test1, src);
	//二值黑白,阈值设置
	threshold(src, src, _thresh, 255, THRESH_BINARY);
	//膨胀
	Mat kernel3 = getStructuringElement(0, kernel_size);
	dilate(src, src, kernel3);
	return src;
}

//轮廓转换成矩形轮廓，根据矩形轮廓面积判断是否清除，排序,重绘矩形轮廓大小
vector<Rect> Sort_by_X(Mat src,vector<vector<Point>> contours,int area,int reduce_x,int reduce_y,int width,int height) {
	//轮廓-->矩形轮廓
	vector<Rect> contours_rect;
	for (int i = 0; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		if (a.area() > area)
		{
			contours_rect.push_back(a);
		}
	}

	//矩形轮廓按x坐标排序
	int _contours = contours_rect.size();
	for (int i = 0; i < _contours - 1; i++)
	{
		int saveindex = i;
		int min = contours_rect[i].x;
		for (int j = i + 1; j < _contours; j++)
		{
			if (min > contours_rect[j].x)
			{
				min = contours_rect[j].x;
				saveindex = j;
			}
		}
		if (saveindex != i) {
			Rect temp = contours_rect[i];
			contours_rect[i] = contours_rect[saveindex];
			contours_rect[saveindex] = temp;
		}
	}
	//清除同一数字下多个轮廓的不精确轮廓；只保留最最上角的 (去除重叠轮廓)
	vector<Rect>::iterator element_to_delete = contours_rect.begin();
	for (int i = 0; i < _contours - 1; i++) {
		//下一个轮廓的x处于这个轮廓的x范围里，证明不是所需要的下一个轮廓，所以就删除
		//如果是下半轮廓，把y值设置成上半轮廓的y值
		if ((contours_rect[i + 1].x <= contours_rect[i].x + contours_rect[i].width)
			&& (contours_rect[i + 1].x >= contours_rect[i].x))
		{
			if (contours_rect[i].y > contours_rect[i + 1].y)
			{
				contours_rect[i].y = contours_rect[i + 1].y;
			}
			element_to_delete + 1 = contours_rect.erase(element_to_delete + 1);
			_contours--;
			i--;
			continue;
		}
		element_to_delete++;
	}

	//获取轮廓左上角坐标，重绘轮廓大小
	for (int i = 0; i < _contours; i++) {
		contours_rect[i].x -= reduce_x;
		contours_rect[i].y -= reduce_x;
		contours_rect[i].width = width;
		contours_rect[i].height = height;
		if (contours_rect[i].x < 0)
			contours_rect[i].x = 0;
		if (contours_rect[i].x + contours_rect[i].width > src.cols)
			contours_rect[i].width = src.cols - contours_rect[i].x;
		if (contours_rect[i].y < 0)
			contours_rect[i].y = 0;
		if (contours_rect[i].y + contours_rect[i].height > src.rows)
			contours_rect[i].height = src.rows - contours_rect[i].y;
	}
	return contours_rect;
}