#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "���ɲ���.h"
#include "dll.h"
#include <sstream> 
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;
using namespace ml;



//��������
Mat cut(Mat src, Rect test);
//intתstring,�̶�λ��
String changestring(int i);
//���ֱ�
void other_invoke2(Mat src, Ptr<SVM>svm);
//����
void other_invoke(Mat src, Ptr<SVM>svm);
//�㶫ͨ�û���Ʊ
void GDTY_invoke(Mat src, Ptr<SVM>svm);
//ͨ�û���Ʊ
void TY_invoke(Mat src, Ptr<SVM>svm);
/*ͨ�ú���*/
Mat pic_init(Mat src, int if_equal, Size blur_size, int _thresh, Size kernel_size);
vector<Rect> Sort_by_X(Mat src, vector<vector<Point>> contours, int area, int reduce_x, int reduce_y, int width, int height);		//����x������������


ofstream logg;
char invoke_num[13];
char *r;	//����

void test(char* buffer) {
	logg.open("E:\\log.txt", ofstream::app);
	logg << "����"<< endl;
	char result[] = "abc";
	int len = sizeof(result) / sizeof(*result);
	logg <<"����"<< len<<endl;
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
	logg << "ͼƬ·����" << path << endl;
	char *result;
	char error[] = "ʶ�����";
	result = error;
	Mat src = imread(path, 1); 
	logg << "ͼ���"<< src.rows << endl;
	logg << "ͼ���"<< src.cols << endl;
	logg << "����ֵ��ʼ��:" << result << endl;

	if (type == 0) {
		//�㶫ͨ�û���Ʊ����Ʊ	�����е�
		logg << "��ʼʶ��type=0Ʊ��" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_0.xml");
		logg << "�ɹ����������ļ�svm)0" << endl;
		GDTY_invoke(src, svm);
		//result = GDTY_invoke(src, svm);		//1000������
		//logg << "result:" << result << endl << endl;
	}
	else if (type == 1) {
		//ͨ�û���Ʊ���Ѷ�λ   ����û��
		logg << "��ʼʶ��type=1Ʊ��" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_1.xml");
		logg << "�ɹ����������ļ�svm_1" << endl;
		TY_invoke(src, svm);
	}
	else if (type == 2) {
		//���ְ׵�1
		logg << "��ʼʶ��type=2Ʊ��" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_2.xml");
		logg << "�ɹ����������ļ�svm_2" << endl;
		other_invoke(src, svm);
	}
	else if (type == 3) {
		//���ְ׵ױ�
		logg << "��ʼʶ��type=3Ʊ��" << endl;
		Ptr<SVM>svm = Algorithm::load<SVM>("svm_3.xml");
		logg << "�ɹ����������ļ�svm_3" << endl;
		other_invoke2(src, svm);
	}

	int len = sizeof(invoke_num) / sizeof(*invoke_num);
	for (int i = 0; i < len; i++)
	{
		buffer[i] = invoke_num[i];
		logg << "invloke_num:" << buffer[i] << endl;
	}
	logg << "����" << endl;
	logg.close();
}


//�ü�ͼƬ
Mat cut(Mat src, Rect test) {
	Mat src2 = src.clone();
	int y = test.y + test.height - 90;
	int height = test.y + test.height - y;
	test.y = y;
	test.height = height;
	//rectangle(src2, test, Scalar(148, 0, 211), 2);
	//imshow("����", src2);

	Mat newpic = src(Range(test.y, test.y + height), Range(test.x, test.x + test.width));
	//imshow("�ü�", newpic);


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
	���ɲ��� a(src, 3);
	Mat newpic = src(Range(a.Need.y, a.Need.y + a.Need.height), Range(a.Need.x, a.Need.x + a.Need.width));	//�ü���ʣ��Ʊ�ŵþ���ͼ
																											//a.pichandle(newpic);
																											//pichandle		/***	��ȡ��������		***/
	Mat save = newpic.clone();

#pragma region 1.��ʼ������

	cvtColor(newpic, newpic, 7);	//ת�ɻҶ�
									//equalizeHist(newpic, newpic);
									//GaussianBlur(newpic, newpic, Size(3,3), 0, 0);

									//threshold(newpic, newpic, 180, 255, THRESH_BINARY);		//��ֵ��
									//adaptiveThreshold
	adaptiveThreshold(newpic, newpic, 255, ADAPTIVE_THRESH_MEAN_C,//����Ӧ��ֵ��blocksizeΪ����
		THRESH_BINARY, 41, 23);
	imshow("a", newpic);
#pragma endregion

#pragma region 2.����ȥ�������� ����ʴʹƱ�Ÿ�����
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//����
	//dilate(newpic, newpic, kernel);
	////imshow("2����", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(3, 3));	//��ʴ
														//erode(newpic, newpic, kernel2);
														////imshow("2��ʴ", handle_src);
														//Mat kernel3 = getStructuringElement(0, Size(5, 5));	//����
														//dilate(newpic, newpic, kernel3);
														////imshow("3����", handle_src);
														//Mat kernel4 = getStructuringElement(0, Size(3, 3));	//��ʴ
														//erode(newpic, newpic, kernel4);
														//////imshow("3��ʴ", handle_src);
#pragma endregion

#pragma region 3.����Ե

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	//��Canny���Ӽ���Ե
	Canny(newpic, canny_output, 10, 10 * 2, 3);
	//imshow("Canny����", canny_output);

#pragma endregion

#pragma region 4.Ѱ������

	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

#pragma	endregion

#pragma region 5.ɸѡ����

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

	//clean			/*** ��������	***/
	//����x���꣬����
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

	//���ͬһ�����¶�������Ĳ���ȷ������ֻ���������Ͻǵ�
	vector<Rect>::iterator element_to_delete = a.saved_contours.begin();
	for (int i = 0; i < _contours - 1; i++) {
		//��һ��������x�������������x��Χ�֤����������Ҫ����һ�����������Ծ�ɾ��
		//������°���������yֵ���ó��ϰ�������yֵ
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



	//��ȡ�������Ͻ����꣬�ػ�������С
	for (int i = 0; i < _contours; i++) {
		a.saved_contours[i].x -= 6;
		a.saved_contours[i].y -= 6;
		a.saved_contours[i].width = 39;
		a.saved_contours[i].height = 60;
	}


	//��ȡ����8������(����)
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
	/*��ʾÿ������*/
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("����_type3���ְ׵ױ�\\��"+to_string(i)+"�ţ�"+ to_string(j) + ".jpg", test);
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
		float response2 = svm->predict(_tmp);  //����Ԥ�⣬����1��-1,��������Ϊfloat
		predict += to_string((int)response2);	//Ԥ���Ʊ��
	}
	for (int i = 0; i < predict.length(); i++)
	{
		invoke_num[i] = predict[i];
	}
}

void other_invoke(Mat src, Ptr<SVM>svm) {
	���ɲ��� a(src, 2);
	Mat newpic = src(Range(a.Need.y, a.Need.y + a.Need.height), Range(a.Need.x, a.Need.x + a.Need.width));	//�ü���ʣ��Ʊ�ŵþ���ͼ
																											//a.pichandle(newpic);

																											//pichandle		/***	��ȡ��������		***/
	Mat save = newpic.clone();

#pragma region 1.��ʼ������

	cvtColor(newpic, newpic, 7);	//ת�ɻҶ�
									//equalizeHist(newpic, newpic);
									//GaussianBlur(newpic, newpic, Size(3,3), 0, 0);

									//threshold(newpic, newpic, 180, 255, THRESH_BINARY);		//��ֵ��
									//adaptiveThreshold
	adaptiveThreshold(newpic, newpic, 255, ADAPTIVE_THRESH_MEAN_C,//����Ӧ��ֵ��blocksizeΪ����
		THRESH_BINARY, 41, 23);
	//imshow("a", newpic);
#pragma endregion

#pragma region 2.����ȥ�������� ����ʴʹƱ�Ÿ�����
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//����
	//dilate(newpic, newpic, kernel);
	////imshow("2����", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(3, 3));	//��ʴ
														//erode(newpic, newpic, kernel2);
														////imshow("2��ʴ", handle_src);
														//Mat kernel3 = getStructuringElement(0, Size(5, 5));	//����
														//dilate(newpic, newpic, kernel3);
														////imshow("3����", handle_src);
														//Mat kernel4 = getStructuringElement(0, Size(3, 3));	//��ʴ
														//erode(newpic, newpic, kernel4);
														//////imshow("3��ʴ", handle_src);
#pragma endregion

#pragma region 3.����Ե

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	//��Canny���Ӽ���Ե
	Canny(newpic, canny_output, 10, 10 * 2, 3);
	//imshow("Canny����", canny_output);

#pragma endregion

#pragma region 4.Ѱ������

	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

#pragma	endregion

#pragma region 5.ɸѡ����

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

	//clean			/*** ��������	***/
	//����x���꣬����
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

	//���ͬһ�����¶�������Ĳ���ȷ������ֻ���������Ͻǵ�
	vector<Rect>::iterator element_to_delete = a.saved_contours.begin();
	for (int i = 0; i < _contours - 1; i++) {
		//��һ��������x�������������x��Χ�֤����������Ҫ����һ�����������Ծ�ɾ��
		//������°���������yֵ���ó��ϰ�������yֵ
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



	//��ȡ�������Ͻ����꣬�ػ�������С
	for (int i = 0; i < _contours; i++) {
		a.saved_contours[i].x -= 6;
		a.saved_contours[i].y -= 6;
		a.saved_contours[i].width = 35;
		a.saved_contours[i].height = 65;
	}


	//��ȡ����8������(����)
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
	/*��ʾÿ������*/
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("����_test_cs\\��"+to_string(*i)+"�ţ�"+ to_string(j) + ".jpg", test);
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
		float response2 = svm->predict(_tmp);  //����Ԥ�⣬����1��-1,��������Ϊfloat
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
	���ɲ��� a(src, 0);
	Mat newpic = cut(src, a.Need);		//�ü���ʣ��Ʊ�ŵþ���ͼ
	logg << "1.�ü�Ʊ������(�ߣ���):" << newpic.rows << "��" << newpic.cols << endl;
										//newpic = imread("14110657.jpg", 1);
	a.pichandle(newpic);				//��ȡ��ɸѡ��0��������
	a.clean();							//��������ϴ�ͷŴ������ȷ��8��Ʊ������

										//��ʾ������������
	for (int i = 0; i < a.saved_contours.size(); i++) {
		rectangle(newpic, a.saved_contours[i], Scalar(255, 64, 64));
	}
	logg << "2.�����Ѿ���ѡ" << endl;
	//imshow("new", newpic);

	/*��ʾÿ������*/
	cvtColor(newpic, newpic, 7);	//ת�ɻҶ�
	threshold(newpic, newpic, 130, 255, THRESH_BINARY);		//��ֵ��
	logg << "3.��ֵ��" << endl;														//2.ȥ��������
	Mat kernel = getStructuringElement(0, Size(3, 3));	//����
	dilate(newpic, newpic, kernel);
	//imshow("2����", handle_src);
	Mat kernel2 = getStructuringElement(0, Size(4, 4));	//��ʴ
	erode(newpic, newpic, kernel2);
	//imshow("2��ʴ", handle_src);
	Mat kernel3 = getStructuringElement(0, Size(5, 5));	//����
	dilate(newpic, newpic, kernel3);
	//imshow("3����", handle_src);
	Mat kernel4 = getStructuringElement(0, Size(3, 3));	//��ʴ
	erode(newpic, newpic, kernel4);
	//Mat kernel = getStructuringElement(0, Size(3, 3));	//����
	//dilate(newpic, newpic, kernel);
	//cout << "��" << *i << "��Ʊ�ţ�";
	imshow("Ʊ��", newpic);
	logg << "4.ȥ����" << endl;
	String predict = "";		//ʶ��Ľ��
	String real = "6789";		//ʵ�ʽ��
	for (int j = 0; j < a.saved_contours.size(); j++) {
		Mat test = newpic.clone();
		test = newpic(Range(a.saved_contours[j].y, a.saved_contours[j].y + a.saved_contours[j].height), Range(a.saved_contours[j].x, a.saved_contours[j].x + a.saved_contours[j].width));
		//namedWindow(to_string(j), 0);
		//imshow(to_string(j), test);
		//imwrite("����\\��"+to_string(i)+"�ţ�"+ to_string(j) + ".jpg", test);
		logg << "5. ���ָ�/��" << test.rows << "," << test.cols << endl;
		//��Ҫʶ������֣�Ҫ��ѵ�������ѵ��Ԫ�ش�Сһֱ����1x2000x1
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
		float response2 = svm->predict(_tmp);  //����Ԥ�⣬����1��-1,��������Ϊfloat
		logg << response2 << endl;
		//cout << response2;
		predict += to_string((int)response2);	//Ԥ���Ʊ��
		
#pragma region ע�͵�ģ��ƥ�����
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
												//			Mat temp = imread("ģ��\\" + to_string(moban) + "\\0.jpg");
												//			matchTemplate(_test, temp, result, method_num); //0-5
												//														   ///��һ��
												//			normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
												//			/// ͨ������ minMaxLoc ��λ��ƥ���λ��
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
												//			// ���� ExceptionName �쳣�Ĵ���
												//		}
												//		
												//	}
												//}
#pragma endregion
	}
	logg << "Ԥ��Ʊ��" << predict << endl;
	//cout << endl << endl;
	//waitKey();
	a.~���ɲ���();
	for (int i = 0; i < predict.length(); i++)
	{
		invoke_num[i] = predict[i];
	}
	invoke_num[8] = '\0';
	r = invoke_num;
	logg << invoke_num << endl;
}

void TY_invoke(Mat src, Ptr<SVM>svm) {
	//ԭͼ�ĸ���
	Mat src_clone;
	src.copyTo(src_clone);
	//imshow("ԭͼ", src_clone);
	Rect src_MaxRect;
	int Max = 0;
#pragma region ��ʼ��ͼƬ����������,����������
	src = pic_init(src, 1, Size(7, 7), 40, Size(70, 5));
	//imshow("ԭͼ��ʼ��", src);
	//��������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int thresh = 10;
	/// ��Canny���Ӽ���Ե
	Canny(src, src, thresh, thresh * 2, 3);
	/// Ѱ������
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Rect> find_middle_Rect;
	for (int i = 0; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		if (a.height > 60)	//ȥ����������Բ����ϵľ���
			continue;
		if (a.width > src.cols*0.7)	//���ϳ�����Щ����
		{
			find_middle_Rect.push_back(a);
		}
		//rectangle(src, a, Scalar(148, 0, 211));	
	}
	Max = find_middle_Rect[0].y;	//�˴���Max��������y����Сֵ
	src_MaxRect = find_middle_Rect[0];
	for (int i = 1; i < find_middle_Rect.size(); i++) {
		if (find_middle_Rect[i].y < Max)
		{
			Max = find_middle_Rect[i].y;
			src_MaxRect = find_middle_Rect[i];
		}
	}
	rectangle(src, src_MaxRect, Scalar(148, 0, 211));
	//imshow("ԭͼ���������", src);
#pragma endregion

	//�ü�һ�������׸�������
	Mat temp_1 = src_clone(Range(src_MaxRect.y - 20, src.rows), Range(0, src.cols));
	Mat temp_2;
	temp_1.copyTo(temp_2);		//�ü����ԭͼ
								//imshow("��һ�βü�", temp_1);
#pragma region �ü�ԭͼ,�ٴδ���ͼ���ҳ�����,�ҵ���Ʊ����,����x����ƫ���ҳ����и������·�����,Ȼ����һ������.��λ��Ʊ�ź�������
	temp_1 = pic_init(temp_1, 0, Size(9, 9), 30, Size(30, 3));
	//imshow("һ�βü���ʼ����", temp_1);
	//Ѱ�����ľ��Σ���һ������
	int _thresh = 10;
	Canny(temp_1, temp_1, _thresh, _thresh * 2, 3);
	findContours(temp_1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		//rectangle(temp_1, a, Scalar(148, 0, 211));
		if (a.y > 160)
		{//ɸѡ���µľ���
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
	//imshow("��ʾ���ľ���", temp_1);

	//�ҳ��ڶ�������
	vector<Rect> array_rect;
	Rect rect_2;//������Ҫ��Ʊ�ž���
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
	//imshow("��ʾ�����Ʊ�ž���", temp_1);

#pragma endregion

	Mat number_pic = temp_2(Range(rect_2.y - 20, rect_2.y + rect_2.height), Range(rect_2.x - 20, rect_2.x + rect_2.width + 20));
	Mat temp3;
	number_pic.copyTo(temp3);		//�����е�ԭͼ
#pragma region ��ȡ��������,��������������ֲ��������ʶ��
	number_pic = pic_init(number_pic, 0, Size(9, 9), 10, Size(3, 3));
	Mat kernel3 = getStructuringElement(0, Size(1, 1));
	erode(src, src, kernel3);
	findContours(number_pic, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Rect> Need = Sort_by_X(number_pic, contours, 80, 5, 5, 34, 50);
	//������
	/*for (int i = 0; i < Need.size(); i++) {
	rectangle(number_pic, Need[i], Scalar(148, 0, 211));
	}*/
	if (Need.size() > 8)
	{//ɸѡ����Ʊ�ŵ�����
		int num = Need.size() - 8;
		vector<Rect>::iterator element_to_delete = Need.begin();
		for (int i = 0; i <num; i++)
		{
			Need.begin() = Need.erase(Need.begin());
		}
	}
	//����ɵ�������
	cvtColor(temp3, temp3, 7);	//ת�ɻҶ�
	GaussianBlur(temp3, temp3, Size(9, 9), 0, 0);
	Mat _temp3;
	Laplacian(temp3, _temp3, 2, 3);
	convertScaleAbs(_temp3, temp3);
	//��ֵ�ڰ�,��ֵ����
	threshold(temp3, temp3, 10, 255, THRESH_BINARY_INV);
	//��ʴ
	Mat kernel4 = getStructuringElement(0, Size(3, 3));
	//erode(temp3, temp3, kernel4);
	imshow("temp3", temp3);
	String predict = "";
	for (int j = 0; j < Need.size(); j++)
	{
		Mat num_temp;
		num_temp = temp3(Range(Need[j].y, Need[j].y + Need[j].height), Range(Need[j].x, Need[j].x + Need[j].width));
		//imshow(to_string(*i), num_temp);
		//imwrite("����_test_3\\��"+to_string(*i)+"�ţ�"+ to_string(j) + ".jpg", num_temp);
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

//ͨ�ó�ʼ������ ����/ת�ɻҶ�/��ȡ����/��ֵ�ڰ�/����
Mat pic_init(Mat src, int if_equal, Size blur_size, int _thresh, Size kernel_size) {
	Mat _test1;
	cvtColor(src, src, 7);
	if (if_equal)
		equalizeHist(src, src);
	GaussianBlur(src, src, blur_size, 0, 0);
	Laplacian(src, _test1, 2, 3);
	convertScaleAbs(_test1, src);
	//Canny(src, src, 30, 90);
	//��ֵ�ڰ�,��ֵ����
	threshold(src, src, _thresh, 255, THRESH_BINARY);
	//adaptiveThreshold(src,src,255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,59,5);

	//����
	Mat kernel3 = getStructuringElement(0, kernel_size);
	dilate(src, src, kernel3);
	return src;
}

//����ת���ɾ������������ݾ�����������ж��Ƿ����������,�ػ����������С
vector<Rect> Sort_by_X(Mat src, vector<vector<Point>> contours, int area, int reduce_x, int reduce_y, int width, int height)
{
	//����-->��������
	vector<Rect> contours_rect;
	for (int i = 0; i < contours.size(); i++) {
		Rect a = boundingRect(contours[i]);
		if (a.area() > area)
		{
			contours_rect.push_back(a);
		}
	}

	//����������x��������
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
	//���ͬһ�����¶�������Ĳ���ȷ������ֻ���������Ͻǵ� (ȥ���ص�����)
	vector<Rect>::iterator element_to_delete = contours_rect.begin();
	for (int i = 0; i < _contours - 1; i++) {
		//��һ��������x�������������x��Χ�֤����������Ҫ����һ�����������Ծ�ɾ��
		//������°���������yֵ���ó��ϰ�������yֵ
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

	//��ȡ�������Ͻ����꣬�ػ�������С
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
