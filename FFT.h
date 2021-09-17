#pragma once
#include <complex>
using namespace std;

class FastFourierTransform {
	friend class Spectrum;
private:
	int count; // 待处理的数据数
public:
	FastFourierTransform(int c) { count = c; }
	// 快速傅里叶变换核心部分
	void process(complex<float>*Data, int Log2N, int sign);
};
