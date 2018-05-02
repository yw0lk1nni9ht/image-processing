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
	//knn算法 属性初始化
	Ptr<KNearest> knn = KNearest::create();
	knn->setAlgorithmType(knn->BRUTE_FORCE);

	//训练矩阵
	int rows;	//训练的图片数量
	int cols;	//单张图片的像素，tmp.cols*tmp.rows
	Mat tmp = imread("单字\\第11张：0.jpg", CV_32FC1);
	rows = tmp.rows;
	cols = tmp.cols * tmp.rows;
	Mat train_data(rows, cols, CV_32FC1);
	Mat train_labels(rows, 1, CV_32FC1);
	
	for (int i = 0; i < 5; i++) {
		Mat input = imread("模板\\0\\" + to_string(i) + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
		input.reshape(0, 1);
		tmp = train_data.rowRange(i, i + 1);
		input.copyTo(tmp);
	}
	knn->train(train_data, ROW_SAMPLE, train_labels);

	Mat predict_data(rows, cols, CV_32FC1);
	Mat predict_labels(5, 1, CV_32FC1);
	for (int i = 0; i < 5; i++)
	{
		Mat input = imread("单字\\第11张：" + to_string(i) + ".jpg", CV_32FC1);
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
	Mat targetData,backData;	//正样本/负样本

	//Mat muban = imread("模板\\0\\0.jpg", 0);
	//int _rows = muban.rows;	//标准行
	//int _cols = muban.cols;	//标准列

	/*Mat targetData(plus_num, _rows*_cols, CV_32FC1);
	Mat backData(minus_num, _rows*_cols, CV_32FC1);*/

	//0样本
	for (int i = 1; i < 23; i++) {
		Mat tmp = imread("模板3票\\0\\0 (" + to_string(i) + ").jpg", 0);
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
		//换用这种方式，把样本的存储放大。适合多种不同大小的数字
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

	//1的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 5; i++) {
		Mat tmp = imread("模板3票\\1\\1 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	///*	float labels[10] = { 0,1,0,0,0,0,0,0,0,0 };
	//	Mat labelsMat(1, 10, CV_32FC1, labels);
	//	train_label.push_back(labelsMat);*/
		train_label.push_back(1);
	}
	//2的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 18; i++) {
		Mat tmp = imread("模板3票\\2\\2 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	/*	float labels[10] = { 0,0,1,0,0,0,0,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(2);
	}
	//3的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 16; i++) {
		Mat tmp = imread("模板3票\\3\\3 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	/////*	float labels[10] = { 0,0,0,1,0,0,0,0,0,0 };
	////	Mat labelsMat(1, 10, CV_32FC1, labels);
	////	train_label.push_back(labelsMat);*/
		train_label.push_back(3);
	}
	//4的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 18; i++) {
		Mat tmp = imread("模板3票\\4\\4 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	///*	float labels[10] = { 0,0,0,0,1,0,0,0,0,0 };
	//	Mat labelsMat(1, 10, CV_32FC1, labels);
	//	train_label.push_back(labelsMat);*/
		train_label.push_back(4);
	}
	//5的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 16; i++) {
		Mat tmp = imread("模板2票\\5\\5 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		/*float labels[10] = { 0,0,0,0,0,1,0,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(5);
	}
	//6的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 33; i++) {
		Mat tmp = imread("模板2票\\6\\6 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		/*float labels[10] = { 0,0,0,0,0,0,1,0,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(6);
	}
	//7的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 2; i++) {
		Mat tmp = imread("模板3票\\7\\7 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
	/*	float labels[10] = { 0,0,0,0,0,0,0,1,0,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(7);
	
	}
	//8的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 16; i++) {
		Mat tmp = imread("模板3票\\8\\8 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		/*float labels[10] = { 0,0,0,0,0,0,0,0,1,0 };
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(8);
	}

	//9的模板，识别率不高，单独增加训练样本
	for (int i = 1; i < 2; i++) {
		Mat tmp = imread("模板3票\\9\\9 (" + to_string(i) + ").jpg", 0);
		Mat _tmp = change_predict(tmp);
		targetData.push_back(_tmp);
		
		/*float labels[10] = { 0,0,0,0,0,0,0,0,0,1};
		Mat labelsMat(1, 10, CV_32FC1, labels);
		train_label.push_back(labelsMat);*/
		train_label.push_back(9);
	}

	////负样本
	//for (int i = 1; i < 10; i++) {
	//	if (i == 8 || i==7 || i==9)
	//	{
	//		continue;
	//	}
	//	for (int j = 0; j < 103; j++) {
	//		Mat tmp = imread("模板\\" + to_string(i) + "\\" + to_string(j) +".jpg", 0);
	//		Mat _tmp = change_predict(tmp);
	//		backData.push_back(_tmp);
	//		train_label.push_back(i);
	//	}
	//}

	//vconcat(targetData, backData, train_data); //合并所有的样本点，作为训练数据
	//train_label = Mat(plus_num + minus_num, 1, CV_32S, Scalar::all(1)); //初始化标注
	//train_label = Mat(plus_num + minus_num, 1, CV_32F, Scalar::all(1)); //初始化标注
	/*for (int i = plus_num; i < plus_num + minus_num; i++) {
		train_label.at<int>(i, 0) = 0;
	}*/

#pragma region SVM训练器
	//Ptr<SVM>svm = Algorithm::load<SVM>("svm.xml");
	Ptr<SVM> svm = SVM::create();
	//svm->setC(1000);
	svm->setType(100);	//100-104
	//svm->setKernel(SVM::RBF);
	svm->setKernel(SVM::LINEAR);
	//svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	// 训练分类器
	Ptr<TrainData> traindata = TrainData::create(targetData, ROW_SAMPLE, train_label);
	//svm->train(traindata);
	cout << "开始训练" << endl;
	svm->trainAuto(traindata);
	svm->save("svm_3.xml");
#pragma endregion

//#pragma region ann神经网络
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

	#pragma region 10张图
	Mat sampleMat_6 = imread("单字\\第11张：0.jpg", 0);
	/*sampleMat_6.rows = _rows;
	sampleMat_6.cols = _cols;*/
	sampleMat_6 = change_predict(sampleMat_6);
	float response = svm->predict(sampleMat_6);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_7 = imread("单字\\第11张：1.jpg", 0);
	/*sampleMat_7.rows = _rows;
	sampleMat_7.cols = _cols;*/
	sampleMat_7 = change_predict(sampleMat_7);
	response = svm->predict(sampleMat_7);  //进行预测，返回1或-1,返回类型为float
	
	Mat sampleMat_8 = imread("单字\\第11张：2.jpg", 0);
	//sampleMat_8.rows = _rows;
	//sampleMat_8.cols = _cols;
	sampleMat_8 = change_predict(sampleMat_8);
	response = svm->predict(sampleMat_8);  //进行预测，返回1或-1,返回类型为float
	
	Mat sampleMat_9 = imread("单字\\第11张：3.jpg", 0);
	/*sampleMat_9.rows = _rows;
	sampleMat_9.cols = _cols;*/
	sampleMat_9 = change_predict(sampleMat_9);
	response = svm->predict(sampleMat_9);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_1 = imread("单字\\第11张：4.jpg", 0);
	//sampleMat_1.rows = _rows;
	//sampleMat_1.cols = _cols;
	sampleMat_1 = change_predict(sampleMat_1);
	response = svm->predict(sampleMat_1);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_0 = imread("单字\\第11张：5.jpg", 0);
	/*sampleMat_0.rows = _rows;
	sampleMat_0.cols = _cols;*/
	sampleMat_0 = change_predict(sampleMat_0);
	response = svm->predict(sampleMat_0);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_2 = imread("单字\\第12张：7.jpg", 0);
	//sampleMat_2.rows = _rows;
	//sampleMat_2.cols = _cols;
	sampleMat_2 = change_predict(sampleMat_2);
	response = svm->predict(sampleMat_2);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_3 = imread("单字\\第13张：7.jpg", 0);
	/*sampleMat_3.rows = _rows;
	sampleMat_3.cols = _cols;*/
	sampleMat_3 = change_predict(sampleMat_3);
	response = svm->predict(sampleMat_3);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_4 = imread("单字\\第14张：7.jpg", 0);
	//sampleMat_4.rows = _rows;
	//sampleMat_4.cols = _cols;
	sampleMat_4 = change_predict(sampleMat_4);
	response = svm->predict(sampleMat_4);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_5 = imread("单字\\第15张：7.jpg", 0);
	//sampleMat_5.rows = _rows;
	//sampleMat_5.cols = _cols;
	sampleMat_5 = change_predict(sampleMat_5);
	response = svm->predict(sampleMat_5);  //进行预测，返回1或-1,返回类型为float

#pragma endregion


	//Mat sampleMat2 = imread("单字\\第17张：5.jpg", 0);
	//sampleMat2.rows = _rows;
	//sampleMat2.cols = _cols;
	//sampleMat2.convertTo(sampleMat2, CV_32FC1);
	//sampleMat2 = sampleMat2.reshape(1, 1);
	//float response2 = svm->predict(sampleMat2);  //进行预测，返回1或-1,返回类型为float
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