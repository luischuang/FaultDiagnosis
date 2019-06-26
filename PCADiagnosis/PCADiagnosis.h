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
	void standardizing(MatrixXd data, RowVectorXd  mean, RowVectorXd sigma, MatrixXd& standardData);
	/*
	����Э����
	*/
	void getCov(MatrixXd data , MatrixXd& cov);
	/*
	�����������������ֵ
	*/
	void getCharacterMatrix(MatrixXd  data, MatrixXd& matrix, MatrixXd& value);

	/*
	�������
	*/
	void calculateLoad(MatrixXd  characterMatrix, MatrixXd characterValue, MatrixXd& load);
	/*
	����ָ��T
	*/
	void calculateT2(MatrixXd data , MatrixXd cov, RowVectorXd& T2);
	/*
	����T2ָ���������
	*/
	void calculateLimit(RowVectorXd T2, double& max, double&min);
	/*
	������Ϲ��׶�
	*/
	void calculateC(MatrixXd data , MatrixXd load, MatrixXd cov, MatrixXd& C);
};

