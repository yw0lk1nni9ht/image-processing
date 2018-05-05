#include "stdafx.h"
#include "集成测试.h"

//Mat test_src2;


//获取票号的区域框
集成测试::集成测试(Mat _src, int type)
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
	//test_src2 = imread("piaohao2.jpg", 1);
	Mat test_src2 = _src;
	Mat test_dst;

	//算法		
	//1.降噪获取轮廓
	GaussianBlur(test_src2, test_dst, Size(9, 9), 0, 0);
	Mat test_dst2, test_dst3;
	cvtColor(test_dst, test_dst2, 7);
	Laplacian(test_dst2, test_dst3, 2, 3);
	convertScaleAbs(test_dst3, test_dst);
	//imshow("2.1高斯图", test_dst);

	//Mat test_dst2, test_dst3;
	//test_dst2 = test_src2;
	//cvtColor(test_dst2, test_dst, 7);

	//二值，黑白
	//2.二值黑白图像
	threshold(test_dst, test_src2, 10, 255, THRESH_BINARY);
	//threshold(test_dst, test_src2, 10, 255, THRESH_BINARY_INV);
	//imshow("2.2二值图", test_src2);

	////降噪
	//blur(test_src2, test_dst, Size(49, 49), Point(-1, -1));
	//GaussianBlur(src, dst, Size(3, 3), 0, 0);
	//imshow("2.3降噪图", test_dst);

	//区域分化更明显
	//3.凸出所需区域
	/************************广东通用机打发票**********************************/
	if (type == 0)
	{
		Mat kernel = getStructuringElement(0, Size(10, 10));		//1000张样本
		dilate(test_src2, test_src2, kernel);
	}
	//篮字白底
	else if (type == 2 || type ==3)
	{
		Mat kernel = getStructuringElement(0, Size(50, 50));		//other_invoke
		dilate(test_src2, test_src2, kernel);
	}
	//imshow("2.4膨胀", test_src2);

	//查找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// 用Canny算子检测边缘
	Canny(test_src2, test_dst, thresh, thresh * 2, 3);
	//imshow("Canny算子", test_dst);
	/// 寻找轮廓
	findContours(test_dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	int Max  = 0;
	//Rect Need;
	for (int i= 0 ; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		if (type == 3)
		{
			if (a.width > 500)
				continue;
			if (a.y < test_dst.rows*0.25 || a.y>test_dst.rows*0.75)
				continue;
		}
		/********************第二种票据的筛选*****************************/
		if (type == 0 || type == 2)
		{
			if (a.width > 450)
			{
				continue;
			}
		}
		/***************************************************************/
		if ((a.width * a.height) > Max)
		{
			Max = a.width * a.height;
			this->Need = a;
		}
	}
	rectangle(test_dst, this->Need, Scalar(148, 0, 211));
	//imshow("drawing", test_dst);
}

//对裁剪后图片的一列处理，获取到数字框
void 集成测试::pichandle(Mat handle_src) {
	Mat save = handle_src.clone();

	

#pragma region 1.初始化处理

	cvtColor(handle_src, handle_src, 7);	//转成灰度
	threshold(handle_src, handle_src, 130, 255, THRESH_BINARY);		//二值化
	//imshow("1二值", handle_src);

	#pragma endregion


#pragma region 2.去背景干扰 ，膨胀腐蚀使票号更明显

	Mat kernel = getStructuringElement(0, Size(3, 3));	//膨胀
	dilate(handle_src, handle_src, kernel);
	//imshow("2膨胀", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(4, 4));	//腐蚀
	erode(handle_src, handle_src, kernel2);
	//imshow("2腐蚀", handle_src);
	Mat kernel3 = getStructuringElement(0, Size(5, 5));	//膨胀
	dilate(handle_src, handle_src, kernel3);
	//imshow("3膨胀", handle_src);
	Mat kernel4 = getStructuringElement(0, Size(3, 3));	//腐蚀
	erode(handle_src, handle_src, kernel4);
	////imshow("3腐蚀", handle_src);

	#pragma endregion


#pragma region 3.检测边缘

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	 //用Canny算子检测边缘
	Canny(handle_src, canny_output, 10, 10 * 2, 3);
	//imshow("Canny算子", canny_output);

	#pragma endregion

	 
#pragma region 4.寻找轮廓
	 
	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	#pragma	endregion


#pragma region 5.筛选轮廓

	for (int i = 0; i < contours.size() ; i++) {
		Rect a = boundingRect(contours[i]);
		if (a.area() > 80)
		{
			rectangle(save, a, Scalar(255, 64, 64));
			saved_contours.push_back(a);
		}
	}

	//for (int i = 0; i < contours.size() - 1; i++) {
	//	Rect a = boundingRect(contours[i]);
	//	if ((a.width * a.height) > 1 && (a.width * a.height)<1800)
	//	{
	//		a.x = a.x - 5;
	//		a.y = a.y - 5;
	//		a.width = a.width + 10;
	//		a.height = a.height + 10;
	//		rectangle(save, a, Scalar(255, 64, 64));
	//		saved_contours.push_back(a);
	//	}
	//}
	//imshow("Contours", save);
	/// 绘出轮廓
	#pragma endregion 




}

//号码框过滤并排序
void 集成测试::clean() {

	//根据x坐标，排序
	int _contours = saved_contours.size();
	for (int i = 0; i < _contours - 1 ; i++)
	{
		int saveindex = i;
		int min = saved_contours[i].x;
		for (int j = i + 1; j < _contours; j++)
		{
			if (min > saved_contours[j].x)
			{
				min = saved_contours[j].x;
				saveindex = j;
			}
		}
		if (saveindex != i) {
			Rect temp = saved_contours[i];
			saved_contours[i] = saved_contours[saveindex];
			saved_contours[saveindex] = temp;
		}
	}


	//清除同一数字下多个轮廓的不精确轮廓；只保留最最上角的
	vector<Rect>::iterator element_to_delete = saved_contours.begin(); 
	for (int i = 0; i < _contours - 1; i++) {
		//下一个轮廓的x处于这个轮廓的x范围里，证明不是所需要的下一个轮廓，所以就删除
		//如果是下半轮廓，把y值设置成上半轮廓的y值
		if ((saved_contours[i + 1].x <= saved_contours[i].x + saved_contours[i].width)
			&& (saved_contours[i + 1].x >= saved_contours[i].x))
		{
			if (saved_contours[i].y > saved_contours[i + 1].y)
			{
				saved_contours[i].y = saved_contours[i + 1].y;
			}
			element_to_delete + 1 = saved_contours.erase(element_to_delete + 1);
			_contours--;
			i--;
			continue;
		}
		element_to_delete++;
	}

	//获取轮廓左上角坐标，重绘轮廓大小
	for (int i = 0; i < _contours; i++) {
		saved_contours[i].x -= 5;
		saved_contours[i].y -= 5;
		saved_contours[i].width = 32;
		saved_contours[i].height = 50;


	}

	//票号8个
	if (saved_contours.size() == 8) {

	}
	else if (saved_contours.size() > 8) {
		while (saved_contours.size() > 8) {
			saved_contours.pop_back();
		}
	}

}



集成测试::~集成测试()
{
}
