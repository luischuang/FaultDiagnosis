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
	vector<vector<double>> datas = vector<vector<double>>();
	readDatas((char*)"normal.csv", 140, datas);
	std::cout << "Hello World!\n";
	cout << endl;
	MatrixXd data(100, 4);
	for (int i = 0; i < 100; i++) {
		//data << datas[i][0], datas[i][1], datas[i][2], datas[i][3];
		for (int j = 0; j < 4; j++) {
			data(i,j)=datas[i][j];
		}
	}
	//data << 1, 2,
	//	3, 6,
	//	4, 2,
	//	5, 2;
	PCADiagnosis diagnosis = PCADiagnosis();
	RowVectorXd mean;
	diagnosis.getMean(data,mean);
	cout << mean<<endl;
	cout << "*************��׼��*****" << endl;
	RowVectorXd sigma;
	diagnosis.getSigma(data,sigma);
	cout << sigma;
	cout << endl;
	cout << "**************************��׼��*********************" << endl;
	diagnosis.standardizing(mean,sigma,data);
	cout << data << endl;
	cout << "Э����" << endl;
	MatrixXd cov1;
	diagnosis.getCov(data,mean,cov1);
	cout << cov1<<endl;
	cout << "****************����ֵ   ��������" << endl;
	MatrixXd vec;
	MatrixXd val;

	
	diagnosis.getCharacterMatrix(cov1,vec,val);
	cout << "��������" << endl;
	cout << vec << endl;
	cout << "����ֵ��" << endl;
	cout << val<<endl;


	MatrixXd load;
	diagnosis.calculateLoad(vec,val,load);
	cout << endl << "���ؾ���" <<endl;
	cout << load << endl;

	cout << "Tƽ��" << endl;
	MatrixXd T;
	diagnosis.calculateT(data,cov1,T);
	cout << T << endl;


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
