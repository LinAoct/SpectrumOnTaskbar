#pragma once
#include <complex>
using namespace std;


class FastFourierTransform
{
    friend class Spectrum;

private:
    int FFTPoint;      // 待处理的数据数
    complex<double> *W;
    // complex<double> X1, X2, X;

public:
    FastFourierTransform(const int point);
    ~FastFourierTransform();
    void PreCalc(int fftPoint);
    // 快速傅里叶变换核心部分
    void Calculate(complex<double> *timeDomain, complex<double> *freqDomain, int log2N);
    void FFTW(complex<float> *samples, complex<float> *FD, int log2N);
    void FFTWF(float *samples, float *FD, int log2N);
};
