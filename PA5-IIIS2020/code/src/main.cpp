#include<iostream>
#include<string>
#include "image.hpp"
#include "Solver.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 5) {
		cout << "Usage: ./bin/PA1 <input bmp file> <output bmp file> <changeWidth> <shrink rate>" << endl;
	}
	// 头两个参数为输入文件名和输出文件名
	// 第三个参数=1表示缩小宽度，=0表示缩小长度
	// 第四个参数表示需要减小的比例（0~1之间的小数）
	int changeWidth = atoi(argv[3]); 
	double rate = atof(argv[4]);
	assert(rate > 0 && rate < 1);
	Solver solver;
	solver.process(argv[1], argv[2], changeWidth, rate);
	return 0;
}