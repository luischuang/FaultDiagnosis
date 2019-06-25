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
	//cout << "��ֵ" << meanval << endl;
	 mean = meanval;
}

//��������׼��
void PCADiagnosis::getSigma(MatrixXd data, RowVectorXd& sigma)
{
	MatrixXd meanval = data.colwise().mean();
	RowVectorXd meanvecRow = meanval;	//��ֵ
	data.rowwise() -= meanvecRow;
	RowVectorXd tem=data.colwise().squaredNorm()/data.rows();
	sigma = tem.cwiseSqrt();
}

	/*
	��׼��
	*/
void PCADiagnosis::standardizing(RowVectorXd mean, RowVectorXd sigma, MatrixXd& data)
{
	data.rowwise() -= mean;
	for (int i = 0; i < data.rows();i++) {
		for (int j = 0; j < data.cols(); j++) {
			data(i, j) = data(i, j) / sigma(j);
		}
	}
}

/*
	����Э����
*/
void PCADiagnosis::getCov(MatrixXd data, RowVectorXd mean, MatrixXd& cov)
{
	data.rowwise() -= mean;
	cov=data.adjoint()* data;
	cov = cov.array() / (data.rows()-1);
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
void PCADiagnosis::calculateT(MatrixXd data, MatrixXd cov, MatrixXd& T)
{
	MatrixXd invcov(cov.rows(),cov.cols());
	invcov = cov.inverse();
	T=data* invcov* data.transpose();
	

}
/*
������Ϲ��׶�
*/
void PCADiagnosis::calculateC(MatrixXd data, MatrixXd load, MatrixXd cov, MatrixXd C)
{

}


