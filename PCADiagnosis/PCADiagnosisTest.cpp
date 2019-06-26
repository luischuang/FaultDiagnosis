#include "pch.h"
#include <iostream>
#include "PCADiagnosis.h"
#include "myUtil.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace splab;
//��ȡ��¼����
int readDatas(char* fileName, int length, vector<vector<double>> &datas) {
	int count = 0;
	ifstream file;
	file.open(fileName, ios::in);

	if (!file.is_open())

		return 0;

	std::string strLine;
	while (getline(file, strLine) && count < length)
	{
		if (strLine.empty())
			continue;
		replace_all(strLine, " ", "");
		vector<std::string> strings = split(strLine, ",");
		vector<double> temp = vector<double>();
		for (int i = 0; i < strings.size(); i++) {
			temp.push_back(atof(strings[i].c_str()));
		}
		datas.push_back(temp);
		count++;
		//cout << strLine << endl;
		strings.~vector();
		temp.~vector();
	}
	file.close();
}
int main()
{
	//��ȡ��׼���ݣ���Ϊ�����֣�100ѵ����40����
	vector<vector<double>> datas = vector<vector<double>>();
	readDatas((char*)"normal.csv", 140, datas);
	MatrixXd data(100, 4);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 4; j++) {
			data(i,j)=datas[i][j];
		}
	}
	MatrixXd normalTestData(40, 4);
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 4; j++) {
			normalTestData(i, j) = datas[i+100][j];
		}
	}

	//��ʼѵ��
	PCADiagnosis diagnosis = PCADiagnosis();

	//����������ֵ
	RowVectorXd mean(data.cols());
	diagnosis.getMean(data,mean);
	//cout << mean<<endl;

	//����������׼��
	//cout << "*************��׼��*****" << endl;
	RowVectorXd sigma(data.cols());
	diagnosis.getSigma(data,sigma);
	//cout << sigma <<endl;

	//��ѵ�����ݽ��б�׼��
	//cout << "**************************��׼��*********************" << endl;
	MatrixXd standardData(100, 4);
	diagnosis.standardizing(data,mean,sigma, standardData);
	//cout << standardData << endl;

	//��Э�������
	//cout << "Э����" << endl;
	MatrixXd cov1(data.cols(),data.cols());
	diagnosis.getCov(standardData,cov1);
	//cout << cov1<<endl;

	//������ֵ����������
	//cout << "****************����ֵ   ��������" << endl;
	MatrixXd vec;
	MatrixXd val;
	diagnosis.getCharacterMatrix(cov1,vec,val);
	//cout << "��������" << endl;
	//cout << vec << endl;
	//cout << "����ֵ��" << endl;
	//cout << val<<endl;

	//����ؾ���
	MatrixXd load;
	diagnosis.calculateLoad(vec,val,load);
	//cout << endl << "���ؾ���" <<endl;
	//cout << load << endl;

	//����ѵ�����ݵ�T2ֵ
	RowVectorXd T2(standardData.rows());
	diagnosis.calculateT2(standardData, cov1, T2);

	//����T2�����¿�����
	double max = 0, min = 0;
	diagnosis.calculateLimit(T2, max, min);
	cout << "���ޣ�" << max << " ���ޣ�" << min << endl;
	//���������Ĳ������ݽ��в���
	//���������Խ��б�׼��
	MatrixXd normalTestStandardData(40, 4);
	diagnosis.standardizing(normalTestData,mean, sigma, normalTestStandardData);
	//����������ݵ�T2ֵ
	RowVectorXd normalTestT2(normalTestData.rows());
	diagnosis.calculateT2(normalTestStandardData,cov1, normalTestT2);
	cout << normalTestT2 << endl;
	//ͳ�������������ݵ���ȷ��
	int normalCurrect = 0;
	for (int i = 0; i < normalTestT2.size(); i++) {
		if (normalTestT2(i) <= max && normalTestT2(i) >= min) {
			normalCurrect++;
		}
	}
	cout << "����������ȷ��Ϊ��" << normalCurrect << "//" << normalTestT2.size()
		<< "(" << (double)normalCurrect / normalTestT2.size() << ")" << endl;


	//��ȡ���ϲ�������
	vector<vector<double>> faultDatas = vector<vector<double>>();
	readDatas((char*)"IRFault.csv", 217, faultDatas);
	MatrixXd faultTestData(217, 4);
	for (int i = 0; i < 217; i++) {
		for (int j = 0; j < 4; j++) {
			faultTestData(i, j) = faultDatas[i][j];
		}
	}
	//���ݹ��ϵĲ������ݽ��в���
	//�Թ��ϲ��Խ��б�׼��
	MatrixXd faultTestStandardData(faultTestData.rows(), 4);
	diagnosis.standardizing(faultTestData, mean, sigma, faultTestStandardData);
	//����������ݵ�T2ֵ
	RowVectorXd faultTestT2(faultTestData.rows());
	diagnosis.calculateT2(faultTestStandardData, cov1, faultTestT2);
	cout << faultTestT2 << endl;
	//ͳ�������������ݵ���ȷ��
	int faultCurrect = 0;
	for (int i = 0; i < faultTestT2.size(); i++) {
		if (faultTestT2(i) > max || faultTestT2(i) < min) {
			faultCurrect++;
		}
	}
	cout << "�������ݲ�����ȷ��Ϊ��" << faultCurrect << "//" << faultTestT2.size()
		<< "(" << (double)faultCurrect / faultTestT2.size() << ")" << endl;

	//�����������Ƶ�ʶԹ��ϵĹ��׶�
	MatrixXd C(faultTestData.rows(), faultTestData.cols());
	diagnosis.calculateC(faultTestData,load,cov1,C);
	//cout << "���׶�C" << endl;
	//cout << C << endl;
}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ������ʾ: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
