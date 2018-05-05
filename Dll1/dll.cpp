#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "集成测试.h"
#include "dll.h"
#include <sstream> 
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;
using namespace ml;



//声明区域
Mat cut(Mat src, Rect test);
//int转string,固定位数
String changestring(int i);
//蓝字扁
void other_invoke2(Mat src, Ptr<SVM>svm);
//蓝字
void other_invoke(Mat src, Ptr<SVM>svm);
//广东通用机打发票
void GDTY_invoke(Mat src, Ptr<SVM>svm);
//通用机打发票
void TY_invoke(Mat src, Ptr<SVM>svm);
/*通用函数*/
Mat pic_init(Mat src, int if_equal, Size blur_size, int _thresh, Size kernel_size);
vector<Rect> Sort_by_X(Mat src, vector<vector<Point>> contours, int area, int reduce_x, int reduce_y, int width, int height);		//根据x坐标排序轮廓


ofstream logg;
char invoke_num[13];
char *r;	//返回

void test(char* buffer) {
	logg.open("E:\\log.txt", ofstream::app);
	logg << "测试"<< endl;
	char result[] = "abc";
	int len = sizeof(result) / sizeof(*result);
	logg <<"长度"<< len<<endl;
	for (int i = 0; i < len; i++)
	{
		buffer[i] = result[i];
		logg << buffer[i] << endl;
	}
	logg.close();
}

int test_int() {
	return 10;
}

void Reconized(char* path, int type,char* buffer)
{
	logg.open("E:\\log.txt", ofstream::app);
	logg << "图片路径：" << path << endl;
	char *result;
	char error[] = "识别出错";
	result = error;
	Mat src = imread(path, 1); 
	logg << "图像高"<< src.rows << endl;
	logg << "图像宽"<< src.cols << endl;
	logg << "返回值初始化:" << result << endl;

	if (type == 0) {
		//广东通用机打发票测试票	红字有底
		logg << "开始识别type=0票种" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_0.xml");
		logg << "成功载入特征文件svm)0" << endl;
		GDTY_invoke(src, svm);
		//result = GDTY_invoke(src, svm);		//1000张样本
		//logg << "result:" << result << endl << endl;
	}
	else if (type == 1) {
		//通用机打发票，难定位   红字没底
		logg << "开始识别type=1票种" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_1.xml");
		logg << "成功载入特征文件svm_1" << endl;
		TY_invoke(src, svm);
	}
	else if (type == 2) {
		//蓝字白底1
		logg << "开始识别type=2票种" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_2.xml");
		logg << "成功载入特征文件svm_2" << endl;
		other_invoke(src, svm);
	}
	else if (type == 3) {
		//蓝字白底扁
		logg << "开始识别type=3票种" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_3.xml");
		logg << "成功载入特征文件svm_3" << endl;
		other_invoke2(src, svm);
	}

	int len = sizeof(invoke_num) / sizeof(*invoke_num);
	for (int i = 0; i < len; i++)
	{
		buffer[i] = invoke_num[i];
		logg << "invloke_num:" << buffer[i] << endl;
	}
	logg << "结束" << endl;
	logg.close();
}


//裁剪图片
Mat cut(Mat src, Rect test) {
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

void other_invoke2(Mat src, Ptr<SVM>svm) {
	集成测试 a(src, 3);
	Mat newpic = src(Range(a.Need.y, a.Need.y + a.Need.height), Range(a.Need.x, a.Need.x + a.Need.width));	//裁剪到剩下票号得矩阵图
																											//a.pichandle(newpic);
																											//pichandle		/***	截取数字区域		***/
	Mat save = newpic.clone();

#pragma region 1.初始化处理

	cvtColor(newpic, newpic, 7);	//转成灰度
									//equalizeHist(newpic, newpic);
									//GaussianBlur(newpic, newpic, Size(3,3), 0, 0);

									//threshold(newpic, newpic, 180, 255, THRESH_BINARY);		//二值化
									//adaptiveThreshold
	adaptiveThreshold(newpic, newpic, 255, ADAPTIVE_THRESH_MEAN_C,//自适应阀值，blocksize为奇数
		THRESH_BINARY, 41, 23);
	imshow("a", newpic);
#pragma endregion

#pragma region 2.无需去背景干扰 ，腐蚀使票号更明显
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	//dilate(newpic, newpic, kernel);
	////imshow("2膨胀", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(3, 3));	//腐蚀
														//erode(newpic, newpic, kernel2);
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
		a.saved_contours[i].width = 39;
		a.saved_contours[i].height = 60;
	}


	//获取最后的8个矩形(清理)
	if (a.saved_contours.size() > 8)
	{
		int delete_num = a.saved_contours.size() - 8;
		for (int i = 0; i < delete_num; i++) {
			a.saved_contours.begin() = a.saved_contours.erase(a.saved_contours.begin());
		}
	}
	for (int i = 0; i < a.saved_contours.size(); i++) {
		Rect aa = a.saved_contours[i];
		rectangle(save, aa, Scalar(255, 64, 64));
	}
	imshow("Contours", save);
	String predict = "";
	/*显示每个数字*/
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("单字_type3蓝字白底扁\\第"+to_string(i)+"张："+ to_string(j) + ".jpg", test);
		//test.rows = 65;
		//test.cols = 32;
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
		predict += to_string((int)response2);	//预测的票号
	}
	for (int i = 0; i < predict.length(); i++)
	{
		invoke_num[i] = predict[i];
	}
}

void other_invoke(Mat src, Ptr<SVM>svm) {
	集成测试 a(src, 2);
	Mat newpic = src(Range(a.Need.y, a.Need.y + a.Need.height), Range(a.Need.x, a.Need.x + a.Need.width));	//裁剪到剩下票号得矩阵图
																											//a.pichandle(newpic);

																											//pichandle		/***	截取数字区域		***/
	Mat save = newpic.clone();

#pragma region 1.初始化处理

	cvtColor(newpic, newpic, 7);	//转成灰度
									//equalizeHist(newpic, newpic);
									//GaussianBlur(newpic, newpic, Size(3,3), 0, 0);

									//threshold(newpic, newpic, 180, 255, THRESH_BINARY);		//二值化
									//adaptiveThreshold
	adaptiveThreshold(newpic, newpic, 255, ADAPTIVE_THRESH_MEAN_C,//自适应阀值，blocksize为奇数
		THRESH_BINARY, 41, 23);
	//imshow("a", newpic);
#pragma endregion

#pragma region 2.无需去背景干扰 ，腐蚀使票号更明显
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	//dilate(newpic, newpic, kernel);
	////imshow("2膨胀", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(3, 3));	//腐蚀
														//erode(newpic, newpic, kernel2);
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
	//imshow("Contours", save);

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
		for (int i = 0; i < delete_num; i++) {
			a.saved_contours.begin() = a.saved_contours.erase(a.saved_contours.begin());
		}
	}
	for (int i = 0; i < a.saved_contours.size(); i++) {
		Rect aa = a.saved_contours[i];
		rectangle(save, aa, Scalar(255, 64, 64));
	}
	imshow("Contours", save);
	String predict = "";
	/*显示每个数字*/
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("单字_test_cs\\第"+to_string(*i)+"张："+ to_string(j) + ".jpg", test);
		//test.rows = 65;
		//test.cols = 32;
		//test.rows = 16;
		//test.cols = 8;
		test.convertTo(test, CV_32FC1);
		test = test.reshape(1, 1);
		Mat _tmp = Mat(1, 2500, CV_32FC1, Scalar(255, 255, 255));
		for (int i = 0; i < test.rows; i++)
		{
			float* mubiao = _tmp.ptr<float>(i);
			float* yangben = test.ptr<float>(i);
			for (int j2 = 0; j2 < test.cols; j2++)
			{
				mubiao[j2] = yangben[j2];
			}
		}
		float response2 = svm->predict(_tmp);  //进行预测，返回1或-1,返回类型为float
		predict += to_string((int)response2);
		/*	Mat resultq;
		Point maxLoc;
		double maxVal = 0;
		svm->predict(_tmp, resultq);
		minMaxLoc(resultq, NULL, &maxVal, NULL, &maxLoc);
		cout << maxLoc.x << endl;*/
	}
	for (int i = 0; i < predict.length(); i++)
	{
		invoke_num[i] = predict[i];
	}
	invoke_num[8] = '\0';
}

void GDTY_invoke(Mat src, Ptr<SVM>svm) {
	集成测试 a(src, 0);
	Mat newpic = cut(src, a.Need);		//裁剪到剩下票号得矩阵图
	logg << "1.裁剪票号区域(高，宽):" << newpic.rows << "，" << newpic.cols << endl;
										//newpic = imread("14110657.jpg", 1);
	a.pichandle(newpic);				//获取并筛选大0部分轮廓
	a.clean();							//对轮廓清洗和放大，留下最精确的8个票号轮廓

										//显示处理过后的轮廓
	for (int i = 0; i < a.saved_contours.size(); i++) {
		rectangle(newpic, a.saved_contours[i], Scalar(255, 64, 64));
	}
	logg << "2.轮廓已经勾选" << endl;
	//imshow("new", newpic);

	/*显示每个数字*/
	cvtColor(newpic, newpic, 7);	//转成灰度
	threshold(newpic, newpic, 130, 255, THRESH_BINARY);		//二值化
	logg << "3.二值化" << endl;														//2.去背景干扰
	Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	dilate(newpic, newpic, kernel);
	//imshow("2膨胀", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(4, 4));	//腐蚀
	erode(newpic, newpic, kernel2);
	//imshow("2腐蚀", handle_src);
	Mat kernel3 = getStructuringElement(0, Size(5, 5));	//膨胀
	dilate(newpic, newpic, kernel3);
	//imshow("3膨胀", handle_src);
	Mat kernel4 = getStructuringElement(0, Size(3, 3));	//腐蚀
	erode(newpic, newpic, kernel4);
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	//dilate(newpic, newpic, kernel);
	//cout << "第" << *i << "张票号：";
	imshow("票号", newpic);
	logg << "4.去背景" << endl;
	String predict = "";		//识别的结果
	String real = "6789";		//实际结果
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("单字\\第"+to_string(i)+"张："+ to_string(j) + ".jpg", test);
		logg << "5. 单字高/宽" << test.rows << "," << test.cols << endl;
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
		logg << response2 << endl;
		//cout << response2;
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
	logg << "预测票号" << predict << endl;
	//cout << endl << endl;
	//waitKey();
	a.~集成测试();
	for (int i = 0; i < predict.length(); i++)
	{
		invoke_num[i] = predict[i];
	}
	invoke_num[8] = '\0';
	r = invoke_num;
	logg << invoke_num << endl;
}

void TY_invoke(Mat src, Ptr<SVM>svm) {
	//原图的复制
	Mat src_clone;
	src.copyTo(src_clone);
	//imshow("原图", src_clone);
	Rect src_MaxRect;
	int Max = 0;
#pragma region 初始化图片并查找轮廓,画出最大矩形
	src = pic_init(src, 1, Size(7, 7), 40, Size(70, 5));
	//imshow("原图初始化", src);
	//查找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// 用Canny算子检测边缘
	Canny(src, src, thresh, thresh * 2, 3);
	/// 寻找轮廓
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Rect> find_middle_Rect;
	for (int i = 0; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		if (a.height > 60)	//去除过宽的明显不符合的矩阵
			continue;
		if (a.width > src.cols*0.7)	//最宽较长的那些矩阵
		{
			find_middle_Rect.push_back(a);
		}
		//rectangle(src, a, Scalar(148, 0, 211));	
	}
	Max = find_middle_Rect[0].y;	//此处的Max用作保存y的最小值
	src_MaxRect = find_middle_Rect[0];
	for (int i = 1; i < find_middle_Rect.size(); i++) {
		if (find_middle_Rect[i].y < Max)
		{
			Max = find_middle_Rect[i].y;
			src_MaxRect = find_middle_Rect[i];
		}
	}
	rectangle(src, src_MaxRect, Scalar(148, 0, 211));
	//imshow("原图里的最大矩形", src);
#pragma endregion

	//裁剪一部分容易干扰区域
	Mat temp_1 = src_clone(Range(src_MaxRect.y - 20, src.rows), Range(0, src.cols));
	Mat temp_2;
	temp_1.copyTo(temp_2);		//裁剪后的原图
								//imshow("第一次裁剪", temp_1);
#pragma region 裁剪原图,再次处理图像找出轮廓,找到发票代码,根据x坐标偏移找出所有附近的下方轮廓,然后做一次清理.定位出票号号码区域
	temp_1 = pic_init(temp_1, 0, Size(9, 9), 30, Size(30, 3));
	//imshow("一次裁剪初始化后", temp_1);
	//寻找最大的矩形，第一行数字
	int _thresh = 10;
	Canny(temp_1, temp_1, _thresh, _thresh * 2, 3);
	findContours(temp_1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		//rectangle(temp_1, a, Scalar(148, 0, 211));
		if (a.y > 160)
		{//筛选最下的矩形
			continue;
		}
		if ((a.width * a.height) > Max)
		{
			Max = a.width * a.height;
			src_MaxRect = a;
		}
	}
	Max = 0;
	rectangle(temp_1, src_MaxRect, Scalar(148, 0, 211));
	//imshow("显示最大的矩形", temp_1);

	//找出第二行数字
	vector<Rect> array_rect;
	Rect rect_2;//我们需要的票号矩形
	for (int i = 0; i < contours.size() - 1; i++) {
		Rect a = boundingRect(contours[i]);
		//rectangle(temp_1, a, Scalar(148, 0, 211));
		if (a != src_MaxRect)
		{
			if (a.x < src_MaxRect.x + 50 && a.x > src_MaxRect.x - 50 && a.y > src_MaxRect.y + src_MaxRect.height)
			{
				array_rect.push_back(a);
			}
		}
	}
	if (array_rect.size() == 1)
	{
		rect_2 = array_rect[0];
	}
	else {
		int x_min = array_rect[0].x;
		int x_max = array_rect[0].x + array_rect[0].width;
		int y_max = array_rect[0].y + array_rect[0].height;
		int y_min = array_rect[0].y;
		for (int i = 1; i < array_rect.size(); i++) {
			if (array_rect[i].x < x_min)
				x_min = array_rect[i].x;
			if (array_rect[i].y < y_min)
				y_min = array_rect[i].y;
			if (array_rect[i].x + array_rect[i].width > x_max)
				x_max = array_rect[i].x + array_rect[i].width;
			if (array_rect[i].y + array_rect[i].height > y_max)
				y_max = array_rect[i].y + array_rect[i].height;

		}
		rect_2.x = x_min;
		rect_2.y = y_min;
		rect_2.width = x_max - x_min;
		rect_2.height = y_max - y_min;
	}
	//.height = 60;
	if (rect_2.width < 267)
		rect_2.width = 267;
	if (rect_2.height < 60 || rect_2.height > 100)
		rect_2.height = 60;
	rectangle(temp_1, rect_2, Scalar(148, 0, 211));
	//imshow("显示所需的票号矩形", temp_1);

#pragma endregion

	Mat number_pic = temp_2(Range(rect_2.y - 20, rect_2.y + rect_2.height), Range(rect_2.x - 20, rect_2.x + rect_2.width + 20));
	Mat temp3;
	number_pic.copyTo(temp3);		//号码行的原图
#pragma region 获取数字区域,并分离出单个数字并做保存或识别
	number_pic = pic_init(number_pic, 0, Size(9, 9), 10, Size(3, 3));
	Mat kernel3 = getStructuringElement(0, Size(1, 1));
	erode(src, src, kernel3);
	findContours(number_pic, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Rect> Need = Sort_by_X(number_pic, contours, 80, 5, 5, 34, 50);
	//画矩形
	/*for (int i = 0; i < Need.size(); i++) {
	rectangle(number_pic, Need[i], Scalar(148, 0, 211));
	}*/
	if (Need.size() > 8)
	{//筛选不是票号的轮廓
		int num = Need.size() - 8;
		vector<Rect>::iterator element_to_delete = Need.begin();
		for (int i = 0; i <num; i++)
		{
			Need.begin() = Need.erase(Need.begin());
		}
	}
	//分离成单个数字
	cvtColor(temp3, temp3, 7);	//转成灰度
	GaussianBlur(temp3, temp3, Size(9, 9), 0, 0);
	Mat _temp3;
	Laplacian(temp3, _temp3, 2, 3);
	convertScaleAbs(_temp3, temp3);
	//二值黑白,阈值设置
	threshold(temp3, temp3, 10, 255, THRESH_BINARY_INV);
	//腐蚀
	Mat kernel4 = getStructuringElement(0, Size(3, 3));
	//erode(temp3, temp3, kernel4);
	imshow("temp3", temp3);
	String predict = "";
	for (int j = 0; j < Need.size(); j++)
	{
		Mat num_temp;
		num_temp = temp3(Range(Need[j].y, Need[j].y + Need[j].height), Range(Need[j].x, Need[j].x + Need[j].width));
		//imshow(to_string(*i), num_temp);
		//imwrite("单字_test_3\\第"+to_string(*i)+"张："+ to_string(j) + ".jpg", num_temp);
		num_temp.convertTo(num_temp, CV_32FC1);
		num_temp = num_temp.reshape(1, 1);
		Mat _tmp = Mat(1, 2500, CV_32FC1, Scalar(255, 255, 255));
		for (int i = 0; i < num_temp.rows; i++)
		{
			float* mubiao = _tmp.ptr<float>(i);
			float* yangben = num_temp.ptr<float>(i);
			for (int j = 0; j < num_temp.cols; j++)
			{
				mubiao[j] = yangben[j];
			}
		}
		float result = svm->predict(_tmp);
		predict += to_string((int)result);
	}
	for (int i = 0; i < predict.length(); i++)
	{
		invoke_num[i] = predict[i];
	}
	invoke_num[8] = '\0';
#pragma endregion
}

//通用初始化操作 降噪/转成灰度/获取轮廓/二值黑白/膨胀
Mat pic_init(Mat src, int if_equal, Size blur_size, int _thresh, Size kernel_size) {
	Mat _test1;
	cvtColor(src, src, 7);
	if (if_equal)
		equalizeHist(src, src);
	GaussianBlur(src, src, blur_size, 0, 0);
	Laplacian(src, _test1, 2, 3);
	convertScaleAbs(_test1, src);
	//Canny(src, src, 30, 90);
	//二值黑白,阈值设置
	threshold(src, src, _thresh, 255, THRESH_BINARY);
	//adaptiveThreshold(src,src,255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,59,5);

	//膨胀
	Mat kernel3 = getStructuringElement(0, kernel_size);
	dilate(src, src, kernel3);
	return src;
}

//轮廓转换成矩形轮廓，根据矩形轮廓面积判断是否清除，排序,重绘矩形轮廓大小
vector<Rect> Sort_by_X(Mat src, vector<vector<Point>> contours, int area, int reduce_x, int reduce_y, int width, int height)
{
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
