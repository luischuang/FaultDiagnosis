#include "pch.h"
#include "PCADiagnosis.h"


PCADiagnosis::PCADiagnosis()
{
}


PCADiagnosis::~PCADiagnosis()
{
}

//��������ֵ
void PCADiagnosis::getMean(MatrixXd data, RowVectorXd& mean)
{
	MatrixXd meanval = data.colwise().mean();
	 mean = meanval;
}

//��������׼��
void PCADiagnosis::getSigma(MatrixXd data, RowVectorXd& sigma)
{
	RowVectorXd meanval = data.colwise().mean(); //��ֵ
	data.rowwise() -= meanval;
	RowVectorXd tem=data.colwise().squaredNorm()/data.rows();
	sigma = tem.cwiseSqrt();
}

	/*
	��׼��
	*/
void PCADiagnosis::standardizing(MatrixXd data, RowVectorXd  mean, RowVectorXd sigma, MatrixXd& standardData)
{
	for (int i = 0; i < data.rows();i++) {
		for (int j = 0; j < data.cols(); j++) {
			standardData(i, j) = (data(i, j)-mean(j)) / sigma(j);
		}
	}
}

/*
	����Э����
*/
void PCADiagnosis::getCov(MatrixXd data, MatrixXd& cov)
{
	//��ȡ��������ֵ
	MatrixXd meanVec = data.colwise().mean();
	//��ȡ���������ֵ����������
	MatrixXd  zeroMeanMat = data;
	//����������ֵ��MatrixXf ת��Ϊ������ RowVectorXf
	RowVectorXd  meanVecRow(RowVectorXd::Map(meanVec.data(), 4));
	zeroMeanMat.rowwise() -= meanVecRow;
	//����Э����
	cov = (zeroMeanMat.adjoint()*zeroMeanMat) / double(data.rows() - 1);
}

/*
	�����������������ֵ
*/
void PCADiagnosis::getCharacterMatrix(MatrixXd cov, MatrixXd& vec, MatrixXd& val)
{
	SelfAdjointEigenSolver<MatrixXd> eig(cov);

	vec = eig.eigenvectors();//��������
	val = eig.eigenvalues();//����ֵ

}



/*
	�������
*/
void PCADiagnosis::calculateLoad(MatrixXd characterMatrix, MatrixXd characterValue, MatrixXd& load)
{
	MatrixXd tem(characterValue.rows(),characterValue.rows());
	for (int i = 0; i < characterValue.rows(); i++) {
		for (int j = 0; j < characterValue.rows(); j++) {
			tem(j, i) = sqrt(characterValue(i, 0)) * characterMatrix(j, i);
		}
	}
	load = tem;
}
/*
	����ָ��T
*/
void PCADiagnosis::calculateT2(MatrixXd data, MatrixXd cov, RowVectorXd& T2)
{
	MatrixXd invcov(cov.rows(),cov.cols());
	invcov = cov.inverse();
	for (int i = 0; i < data.rows(); i++) {
		T2(i) = data.row(i)*cov.inverse()*data.row(i).transpose();
	}
}
void PCADiagnosis::calculateLimit(RowVectorXd T2, double & max, double & min)
{
	double meanval = T2.mean(); //��ֵ
	double sum = 0;
	for (int i = 0; i < T2.size(); i++) {
		sum += (T2(i) - meanval)*(T2(i) - meanval);
	}
	double sigma = sqrt(sum/ T2.size());
	max = meanval + 3 * sigma;
	min = meanval - 3 * sigma;
}
/*
������Ϲ��׶�
*/
void PCADiagnosis::calculateC(MatrixXd data, MatrixXd load, MatrixXd cov, MatrixXd& C)
{
	for (int i = 0; i < data.rows(); i++) {
		double sum = 0;
		for (int j = 0; j < data.cols(); j++) {
			C(i, j) = (data.row(i)*load)*cov.inverse()*(data(i, j)*load.col(j));
			C(i, j) = abs(C(i, j));
			sum += C(i, j);
		}
		for (int j = 0; j < data.cols(); j++) {
			C(i, j) = C(i, j)/sum;
		}
	}
}


