#include "stdafx.h"
#include "MachineLearning.h"


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

	Mat muban = imread("模板\\0\\0.jpg", 0);
	int _rows = muban.rows;	//标准行
	int _cols = muban.cols;	//标准列
	int plus_num = 90;	//正样本数量
	int minus_num = 36;	//负样本数量
	/*Mat targetData(plus_num, _rows*_cols, CV_32FC1);
	Mat backData(minus_num, _rows*_cols, CV_32FC1);*/

	//正样本
	for (int i = 0; i < plus_num; i++) {
		Mat tmp = imread("模板\\0\\" + to_string(i) + ".jpg", 0);
		tmp.rows = _rows;
		tmp.cols = _cols;
		//tmp.reshape(1, 1);
		/*Mat _tmp = targetData.rowRange(i, i + 1);
		_tmp = tmp.reshape(1, 1);*/
		//tmp.copyTo(_tmp);
		Mat _tmp;
		tmp.convertTo(_tmp, CV_32FC1);
		_tmp = _tmp.reshape(1, 1);
		targetData.push_back(_tmp);
		train_label.push_back(0);
	}

	//7的模板，识别率不高，单独增加训练样本
	for (int i = 0; i < 110; i++) {
		Mat tmp = imread("模板\\7\\" + to_string(i) + ".jpg", 0);
		tmp.rows = _rows;
		tmp.cols = _cols;
		//tmp.reshape(1, 1);
		/*Mat _tmp = targetData.rowRange(i, i + 1);
		_tmp = tmp.reshape(1, 1);*/
		//tmp.copyTo(_tmp);
		Mat _tmp;
		tmp.convertTo(_tmp, CV_32FC1);
		_tmp = _tmp.reshape(1, 1);
		targetData.push_back(_tmp);
		train_label.push_back(7);
	}

	//8的模板，识别率不高，单独增加训练样本
	for (int i = 0; i < 140; i++) {
		Mat tmp = imread("模板\\8\\" + to_string(i) + ".jpg", 0);
		tmp.rows = _rows;
		tmp.cols = _cols;
		//tmp.reshape(1, 1);
		/*Mat _tmp = targetData.rowRange(i, i + 1);
		_tmp = tmp.reshape(1, 1);*/
		//tmp.copyTo(_tmp);
		Mat _tmp;
		tmp.convertTo(_tmp, CV_32FC1);
		_tmp = _tmp.reshape(1, 1);
		targetData.push_back(_tmp);
		train_label.push_back(8);
	}

	//9的模板，识别率不高，单独增加训练样本
	for (int i = 0; i < 110; i++) {
		Mat tmp = imread("模板\\9\\" + to_string(i) + ".jpg", 0);
		tmp.rows = _rows;
		tmp.cols = _cols;
		//tmp.reshape(1, 1);
		/*Mat _tmp = targetData.rowRange(i, i + 1);
		_tmp = tmp.reshape(1, 1);*/
		//tmp.copyTo(_tmp);
		Mat _tmp;
		tmp.convertTo(_tmp, CV_32FC1);
		_tmp = _tmp.reshape(1, 1);
		targetData.push_back(_tmp);
		train_label.push_back(9);
	}

	//负样本
	for (int i = 1; i < 10; i++) {
		if (i == 8 || i==7 || i==9)
		{
			continue;
		}
		for (int j = 0; j < 103; j++) {
			Mat tmp = imread("模板\\" + to_string(i) + "\\" + to_string(j) +".jpg", 0);
			tmp.rows = _rows;
			tmp.cols = _cols;
			/*tmp.reshape(1, 1);
			Mat _tmp = backData.rowRange(i - 1, i);
			tmp.copyTo(_tmp);*/

			Mat _tmp;
			tmp.convertTo(_tmp, CV_32FC1); 
			_tmp = _tmp.reshape(1, 1);
			backData.push_back(_tmp);
			train_label.push_back(i);
		}
	}

	vconcat(targetData, backData, train_data); //合并所有的样本点，作为训练数据
	//train_label = Mat(plus_num + minus_num, 1, CV_32S, Scalar::all(1)); //初始化标注
	//train_label = Mat(plus_num + minus_num, 1, CV_32F, Scalar::all(1)); //初始化标注
	/*for (int i = plus_num; i < plus_num + minus_num; i++) {
		train_label.at<int>(i, 0) = 0;
	}*/

	//Ptr<SVM>svm = Algorithm::load<SVM>("svm.xml");
	Ptr<SVM> svm = SVM::create();
	svm->setType(100);	//100-104
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	// 训练分类器
	Ptr<TrainData> traindata = TrainData::create(train_data, ROW_SAMPLE, train_label);
	//svm->train(traindata);
	svm->trainAuto(traindata);
	svm->save("svm.xml");

	#pragma region 10张图
	Mat test;
	Mat sampleMat_6 = imread("单字\\第11张：0.jpg", 0);
	sampleMat_6.rows = _rows;
	sampleMat_6.cols = _cols;
	sampleMat_6.convertTo(sampleMat_6, CV_32FC1);
	sampleMat_6 = sampleMat_6.reshape(1, 1);
	float response = svm->predict(sampleMat_6);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_7 = imread("单字\\第11张：1.jpg", 0);
	sampleMat_7.rows = _rows;
	sampleMat_7.cols = _cols;
	sampleMat_7.convertTo(sampleMat_7, CV_32FC1);
	sampleMat_7 = sampleMat_7.reshape(1, 1);
	response = svm->predict(sampleMat_7);  //进行预测，返回1或-1,返回类型为float
	
	Mat sampleMat_8 = imread("单字\\第11张：2.jpg", 0);
	sampleMat_8.rows = _rows;
	sampleMat_8.cols = _cols;
	sampleMat_8.convertTo(sampleMat_8, CV_32FC1);
	sampleMat_8 = sampleMat_8.reshape(1, 1);
	response = svm->predict(sampleMat_8);  //进行预测，返回1或-1,返回类型为float
	
	Mat sampleMat_9 = imread("单字\\第11张：3.jpg", 0);
	sampleMat_9.rows = _rows;
	sampleMat_9.cols = _cols;
	sampleMat_9.convertTo(sampleMat_9, CV_32FC1);
	sampleMat_9 = sampleMat_9.reshape(1, 1);
	response = svm->predict(sampleMat_9);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_1 = imread("单字\\第11张：4.jpg", 0);
	sampleMat_1.rows = _rows;
	sampleMat_1.cols = _cols;
	sampleMat_1.convertTo(sampleMat_1, CV_32FC1);
	sampleMat_1 = sampleMat_1.reshape(1, 1);
	response = svm->predict(sampleMat_1);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_0 = imread("单字\\第11张：5.jpg", 0);
	sampleMat_0.rows = _rows;
	sampleMat_0.cols = _cols;
	sampleMat_0.convertTo(sampleMat_0, CV_32FC1);
	sampleMat_0 = sampleMat_0.reshape(1, 1);
	response = svm->predict(sampleMat_0);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_2 = imread("单字\\第12张：7.jpg", 0);
	sampleMat_2.rows = _rows;
	sampleMat_2.cols = _cols;
	sampleMat_2.convertTo(sampleMat_2, CV_32FC1);
	sampleMat_2 = sampleMat_2.reshape(1, 1);
	response = svm->predict(sampleMat_2);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_3 = imread("单字\\第13张：7.jpg", 0);
	sampleMat_3.rows = _rows;
	sampleMat_3.cols = _cols;
	sampleMat_3.convertTo(sampleMat_3, CV_32FC1);
	sampleMat_3 = sampleMat_3.reshape(1, 1);
	response = svm->predict(sampleMat_3);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_4 = imread("单字\\第14张：7.jpg", 0);
	sampleMat_4.rows = _rows;
	sampleMat_4.cols = _cols;
	sampleMat_4.convertTo(sampleMat_4, CV_32FC1);
	sampleMat_4 = sampleMat_4.reshape(1, 1);
	response = svm->predict(sampleMat_4);  //进行预测，返回1或-1,返回类型为float

	Mat sampleMat_5 = imread("单字\\第15张：7.jpg", 0);
	sampleMat_5.rows = _rows;
	sampleMat_5.cols = _cols;
	sampleMat_5.convertTo(sampleMat_5, CV_32FC1);
	sampleMat_5 = sampleMat_5.reshape(1, 1);
	response = svm->predict(sampleMat_5);  //进行预测，返回1或-1,返回类型为float

#pragma endregion


	Mat sampleMat2 = imread("单字\\第17张：5.jpg", 0);
	sampleMat2.rows = _rows;
	sampleMat2.cols = _cols;
	sampleMat2.convertTo(sampleMat2, CV_32FC1);
	sampleMat2 = sampleMat2.reshape(1, 1);
	float response2 = svm->predict(sampleMat2);  //进行预测，返回1或-1,返回类型为float
}


MachineLearning::~MachineLearning()
{
}
