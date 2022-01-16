#pragma once
#include <complex>
using namespace std;

class FastFourierTransform {
	friend class Spectrum;

private:
    int FFTPoint;      // 待处理的数据数
    complex<double> *W;

public:
    FastFourierTransform(int c) { FFTPoint = c; }
    ~FastFourierTransform();
    void PreCalc(int fftPoint);
	// 快速傅里叶变换核心部分
    void Calculate(complex<double> *TD, complex<double> *FD, int log2N);
    void FFTW(complex<float> *samples, complex<float> *FD, int log2N);
    void FFTWF(float *samples, float *FD, int log2N);
};
