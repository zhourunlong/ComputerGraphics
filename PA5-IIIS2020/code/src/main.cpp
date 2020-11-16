#include<iostream>
#include<string>
#include "image.hpp"
#include "Solver.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 5) {
		cout << "Usage: ./bin/PA1 <input bmp file> <output bmp file> <changeWidth> <shrink rate>" << endl;
	}
	// ͷ��������Ϊ�����ļ���������ļ���
	// ����������=1��ʾ��С��ȣ�=0��ʾ��С����
	// ���ĸ�������ʾ��Ҫ��С�ı�����0~1֮���С����
	int changeWidth = atoi(argv[3]); 
	double rate = atof(argv[4]);
	assert(rate > 0 && rate < 1);
	Solver solver;
	solver.process(argv[1], argv[2], changeWidth, rate);
	return 0;
}