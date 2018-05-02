#include "stdafx.h"
#include "MachineLearning.h"
#include "opencv2/features2d/features2d.hpp"

Mat change_predict(Mat src);
Mat getFeature(Mat src);

MachineLearning::MachineLearning()
{
}

void MachineLearning::KNN_test()
{
	int K = 3;
	//knn�㷨 ���Գ�ʼ��
	Ptr<KNearest> knn = KNearest::create();
	knn->setAlgorithmType(knn->BRUTE_FORCE);

	//ѵ������
	int rows;	//ѵ����ͼƬ����
	int cols;	//����ͼƬ�����أ�tmp.cols*tmp.rows
	Mat tmp = imread("����\\��11�ţ�0.jpg", CV_32FC1);
	rows = tmp.rows;
	cols = tmp.cols * tmp.rows;
	Mat train_data(rows, cols, CV_32FC1);
	Mat train_labels(rows, 1, CV_32FC1);
	
	for (int i = 0; i < 5; i++) {
		Mat input = imread("ģ��\\0\\" + to_string(i) + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
		input.reshape(0, 1);
		tmp = train_data.rowRange(i, i + 1);
		input.copyTo(tmp);
	}
	knn->train(train_data, ROW_SAMPLE, train_labels);

	Mat predict_data(rows, cols, CV_32FC1);
	Mat predict_labels(5, 1, CV_32FC1);
	for (int i = 0; i < 5; i++)
	{
		Mat input = imread("����\\��11�ţ�" + to_string(i) + ".jpg", CV_32FC1);
		input.reshape(0, 1);
		tmp = predict_data.rowRange(i, i + 1);
		input.copyTo(tmp);
	}
	
	Mat result;
	knn->findNearest(predict_data, K, result);

	int count = 0;
	for (int i = 0; i < 5; ++i) {
		float value1 = ((float*)predict_labels.data)[i];
		float value2 = ((float*)result.data)[i];
		fprintf(stdout, "expected value: %f, actual value: %f\n", value1, value2);

		if (int(value1) == int(value2)) ++count;
	}
	fprintf(stdout, "when K = %d, accuracy: %f\n", K, count * 1.f / 5);

}


void MachineLearning::SVM_test() {

	Mat train_data, train_label;
	Mat targetData,backData;	//������/������

	//Mat muban = imread("ģ��\\0\\0.jpg", 0);
	//int _rows = muban.rows;	//��׼��
	//int _cols = muban.cols;	//��׼��

	/*Mat targetData(plus_num, _rows*_cols, CV_32FC1);
	Mat backData(minus_num, _rows*_cols, CV_32FC1);*/

	//0����
	for (int i = 1; i < 23; i++) {
		Mat tmp = imread("ģ��3Ʊ\\0\\0 (" + to_string(i) + ").jpg", 0);
		/*tmp.rows = _rows;
		tmp.cols = _cols;*/
		//tmp.reshape(1, 1);
		/*Mat _tmp = targetData.rowRange(i, i + 1);
		_tmp = tmp.reshape(1, 1);*/
		//tmp.copyTo(_tmp);
		/*
		Mat _tmp;
		tmp.convertTo(_tmp, CV_32FC1);
		_tmp = _tmp.reshape(1, 1);
		targetData.push_back(_tmp);
		train_label.push_back(0);
		*/
		//�������ַ�ʽ���������Ĵ洢�Ŵ��ʺ϶��ֲ�ͬ��С������
		Mat _tmp = Mat(1, 2500, CV_32FC1,Scalar(255,255,255));
		tmp.convertTo(tmp, CV_32FC1);
		tmp = tmp.reshape(1, 1);
		for (int i = 0; i < tmp.rows; i++)
		{
			float* mubiao = _tmp.ptr<float>(i);
			float* yangben = tmp.ptr<float>(i);
			for (int j = 0; j < tmp.cols; j++)
			{
				mubiao[j] = yangben[j];
			}
		}
		targetData.push_back(_tmp);
	/*	float labels[10] = { 1,0,0,0,0,0,0,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(0);
	}

	//1��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 5; i++) {
		Mat tmp = imread("ģ��3Ʊ\\1\\1 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	///*	float labels[10] = { 0,1,0,0,0,0,0,0,0,0 };
	//	Mat labelsMat(1, 10, CV_32FC1, labels);
	//	train_label.push_back(labelsMat);*/
		train_label.push_back(1);
	}
	//2��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 18; i++) {
		Mat tmp = imread("ģ��3Ʊ\\2\\2 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	/*	float labels[10] = { 0,0,1,0,0,0,0,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(2);
	}
	//3��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 16; i++) {
		Mat tmp = imread("ģ��3Ʊ\\3\\3 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	/////*	float labels[10] = { 0,0,0,1,0,0,0,0,0,0 };
	////	Mat labelsMat(1, 10, CV_32FC1, labels);
	////	train_label.push_back(labelsMat);*/
		train_label.push_back(3);
	}
	//4��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 18; i++) {
		Mat tmp = imread("ģ��3Ʊ\\4\\4 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	///*	float labels[10] = { 0,0,0,0,1,0,0,0,0,0 };
	//	Mat labelsMat(1, 10, CV_32FC1, labels);
	//	train_label.push_back(labelsMat);*/
		train_label.push_back(4);
	}
	//5��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 16; i++) {
		Mat tmp = imread("ģ��2Ʊ\\5\\5 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		/*float labels[10] = { 0,0,0,0,0,1,0,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(5);
	}
	//6��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 33; i++) {
		Mat tmp = imread("ģ��2Ʊ\\6\\6 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		/*float labels[10] = { 0,0,0,0,0,0,1,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(6);
	}
	//7��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 2; i++) {
		Mat tmp = imread("ģ��3Ʊ\\7\\7 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	/*	float labels[10] = { 0,0,0,0,0,0,0,1,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(7);
	
	}
	//8��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 16; i++) {
		Mat tmp = imread("ģ��3Ʊ\\8\\8 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		/*float labels[10] = { 0,0,0,0,0,0,0,0,1,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(8);
	}

	//9��ģ�壬ʶ���ʲ��ߣ���������ѵ������
	for (int i = 1; i < 2; i++) {
		Mat tmp = imread("ģ��3Ʊ\\9\\9 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		
		/*float labels[10] = { 0,0,0,0,0,0,0,0,0,1};
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(9);
	}

	////������
	//for (int i = 1; i < 10; i++) {
	//	if (i == 8 || i==7 || i==9)
	//	{
	//		continue;
	//	}
	//	for (int j = 0; j < 103; j++) {
	//		Mat tmp = imread("ģ��\\" + to_string(i) + "\\" + to_string(j) +".jpg", 0);
	//		Mat _tmp = change_predict(tmp);
	//		backData.push_back(_tmp);
	//		train_label.push_back(i);
	//	}
	//}

	//vconcat(targetData, backData, train_data); //�ϲ����е������㣬��Ϊѵ������
	//train_label = Mat(plus_num + minus_num, 1, CV_32S, Scalar::all(1)); //��ʼ����ע
	//train_label = Mat(plus_num + minus_num, 1, CV_32F, Scalar::all(1)); //��ʼ����ע
	/*for (int i = plus_num; i < plus_num + minus_num; i++) {
		train_label.at<int>(i, 0) = 0;
	}*/

#pragma region SVMѵ����
	//Ptr<SVM>svm = Algorithm::load<SVM>("svm.xml");
	Ptr<SVM> svm = SVM::create();
	//svm->setC(1000);
	svm->setType(100);	//100-104
	//svm->setKernel(SVM::RBF);
	svm->setKernel(SVM::LINEAR);
	//svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	// ѵ��������
	Ptr<TrainData> traindata = TrainData::create(targetData, ROW_SAMPLE, train_label);
	//svm->train(traindata);
	cout << "��ʼѵ��" << endl;
	svm->trainAuto(traindata);
	svm->save("svm_3.xml");
#pragma endregion

//#pragma region ann������
//	Ptr<ANN_MLP>svm = ANN_MLP::create();
//	svm->setTrainMethod(ANN_MLP::BACKPROP,0.001,0.1);
//	Mat layersizes = (Mat_<int>(1, 5) << 2500, 10, 10, 10, 10);
//	svm->setLayerSizes(layersizes);
//	svm->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1);
//	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 10000, 0.0001));
//	Ptr<TrainData> traindata = TrainData::create(targetData, ROW_SAMPLE, train_label);
//	svm->train(traindata);
//	svm->save("bpcharModel2.xml");
//#pragma endregion

	#pragma region 10��ͼ
	Mat sampleMat_6 = imread("����\\��11�ţ�0.jpg", 0);
	/*sampleMat_6.rows = _rows;
	sampleMat_6.cols = _cols;*/
	sampleMat_6 = change_predict(sampleMat_6);
	float response = svm->predict(sampleMat_6);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_7 = imread("����\\��11�ţ�1.jpg", 0);
	/*sampleMat_7.rows = _rows;
	sampleMat_7.cols = _cols;*/
	sampleMat_7 = change_predict(sampleMat_7);
	response = svm->predict(sampleMat_7);  //����Ԥ�⣬����1��-1,��������Ϊfloat
	
	Mat sampleMat_8 = imread("����\\��11�ţ�2.jpg", 0);
	//sampleMat_8.rows = _rows;
	//sampleMat_8.cols = _cols;
	sampleMat_8 = change_predict(sampleMat_8);
	response = svm->predict(sampleMat_8);  //����Ԥ�⣬����1��-1,��������Ϊfloat
	
	Mat sampleMat_9 = imread("����\\��11�ţ�3.jpg", 0);
	/*sampleMat_9.rows = _rows;
	sampleMat_9.cols = _cols;*/
	sampleMat_9 = change_predict(sampleMat_9);
	response = svm->predict(sampleMat_9);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_1 = imread("����\\��11�ţ�4.jpg", 0);
	//sampleMat_1.rows = _rows;
	//sampleMat_1.cols = _cols;
	sampleMat_1 = change_predict(sampleMat_1);
	response = svm->predict(sampleMat_1);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_0 = imread("����\\��11�ţ�5.jpg", 0);
	/*sampleMat_0.rows = _rows;
	sampleMat_0.cols = _cols;*/
	sampleMat_0 = change_predict(sampleMat_0);
	response = svm->predict(sampleMat_0);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_2 = imread("����\\��12�ţ�7.jpg", 0);
	//sampleMat_2.rows = _rows;
	//sampleMat_2.cols = _cols;
	sampleMat_2 = change_predict(sampleMat_2);
	response = svm->predict(sampleMat_2);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_3 = imread("����\\��13�ţ�7.jpg", 0);
	/*sampleMat_3.rows = _rows;
	sampleMat_3.cols = _cols;*/
	sampleMat_3 = change_predict(sampleMat_3);
	response = svm->predict(sampleMat_3);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_4 = imread("����\\��14�ţ�7.jpg", 0);
	//sampleMat_4.rows = _rows;
	//sampleMat_4.cols = _cols;
	sampleMat_4 = change_predict(sampleMat_4);
	response = svm->predict(sampleMat_4);  //����Ԥ�⣬����1��-1,��������Ϊfloat

	Mat sampleMat_5 = imread("����\\��15�ţ�7.jpg", 0);
	//sampleMat_5.rows = _rows;
	//sampleMat_5.cols = _cols;
	sampleMat_5 = change_predict(sampleMat_5);
	response = svm->predict(sampleMat_5);  //����Ԥ�⣬����1��-1,��������Ϊfloat

#pragma endregion


	//Mat sampleMat2 = imread("����\\��17�ţ�5.jpg", 0);
	//sampleMat2.rows = _rows;
	//sampleMat2.cols = _cols;
	//sampleMat2.convertTo(sampleMat2, CV_32FC1);
	//sampleMat2 = sampleMat2.reshape(1, 1);
	//float response2 = svm->predict(sampleMat2);  //����Ԥ�⣬����1��-1,��������Ϊfloat
}

Mat change_predict(Mat src) {
	src.convertTo(src, CV_32FC1);
	src = src.reshape(1, 1);
	Mat _tmp = Mat(1, 2500, CV_32FC1, Scalar(255, 255, 255));
	for (int i = 0; i < src.rows; i++)
	{
		float* mubiao = _tmp.ptr<float>(i);
		float* yangben = src.ptr<float>(i);
		for (int j = 0; j < src.cols; j++)
		{
			mubiao[j] = yangben[j];
		}
	}
	return _tmp;
}

MachineLearning::~MachineLearning()
{
}

Mat getFeature(Mat src22) {
	Mat out_src;
	vector<KeyPoint> keypoints;
	Ptr<AgastFeatureDetector> testdetector = AgastFeatureDetector::create();
	testdetector->detect(src22, keypoints);

	drawKeypoints(src22, keypoints, out_src);
	return out_src;
}