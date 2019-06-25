#pragma once
#include <stdio.h>
#include <vector>
#include "vector.h"

#include"Eigen/Dense"
using namespace std;
using namespace splab;
using namespace Eigen;
class PCADiagnosis
{
	
public:
	PCADiagnosis();
	~PCADiagnosis();
	/*
	������������ֵ
	*/
	void getMean(MatrixXd data, RowVectorXd& mean);
	/*
	������������׼��
	*/
	void getSigma(MatrixXd data, RowVectorXd& sigma);
	/*
	��׼��
	*/
	void standardizing(RowVectorXd  mean, RowVectorXd sigma, MatrixXd& data);
	/*
	����Э����
	*/
	void getCov(MatrixXd data , RowVectorXd mean, MatrixXd& cov);
	/*
	�����������������ֵ
	*/
	void getCharacterMatrix(MatrixXd  data, MatrixXd& matrix, MatrixXd& value);
	///*
	//��������ֵ
	//*/
	//void getCharacterValue(MatrixXd  data, MatrixXd& value);
	/*
	�������
	*/
	void calculateLoad(MatrixXd  characterMatrix, MatrixXd characterValue, MatrixXd& load);
	/*
	����ָ��T
	*/
	void calculateT(MatrixXd data , MatrixXd cov, MatrixXd& T);
	/*
	������Ϲ��׶�
	*/
	void calculateC(MatrixXd data , MatrixXd load, MatrixXd cov, MatrixXd C);
};

