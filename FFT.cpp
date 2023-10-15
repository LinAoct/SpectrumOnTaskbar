#include "FFT.h"
#include <cmath>
#include <qdebug.h>
#include "fftw3.h"


static const double PI = 3.1415926535898;

FastFourierTransform::FastFourierTransform(const int point) : FFTPoint(point)
{
    PreCalc(point);
}

FastFourierTransform::~FastFourierTransform()
{
    delete this->W;
}

/**
 * @brief FFT预计算加权系数
 * @param fftPoint FFT点数
 */
void FastFourierTransform::PreCalc(int fftPoint)
{
    this->FFTPoint = fftPoint;
    double angle;   // 角度
    // 分配运算所需存储器
    W = new complex<double>[this->FFTPoint / 2];
    // 计算加权系数
    for(int i = 0; i < this->FFTPoint / 2; i++)
    {
        angle = -i * PI * 2 / this->FFTPoint;
        W[i] = complex<double>(cos(angle), sin(angle));
    }
}

/**
 * @brief 快速傅里叶变换核心算法
 * @param timeDomain 时域输入
 * @param freqDomain 频域输出
 * @param log2N 迭代次数
 */
void FastFourierTransform::Calculate(complex<double> *timeDomain, complex<double> *freqDomain, int log2N)
{
    int i, j, k, bfsize, p;				// 循环变量
    complex<double> *X1,*X2,*X;

    FFTPoint = 1 << log2N;				// 计算傅里叶变换点数

    // 分配运算所需存储器
    X1 = new complex<double>[FFTPoint];
    X2 = new complex<double>[FFTPoint];

    // 将时域点写入X1
    memcpy(X1, timeDomain, (sizeof(complex<double>) * FFTPoint));

    // 采用蝶形算法进行快速傅里叶变换
    for(k = 0; k < log2N; k++)  // k为蝶形运算的级数
    {
        for(j = 0; j < 1 << k; j++)
        {
            bfsize = 1 << (log2N-k);    // 做蝶形运算两点间距离
            for(i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
                X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2])
                                         * W[i * (1<<k)];
            }
        }
        X  = X1;
        X1 = X2;
        X2 = X;
    }
    // 重新排序
    for(j = 0; j < this->FFTPoint; j++)
    {
        p = 0;
        for(i = 0; i < log2N; i++)
        {
            if (j&(1<<i))
            {
                p+=1<<(log2N-i-1);
            }
        }
        freqDomain[j]=X1[p];
    }

    delete X1;
    delete X2;
}

void FastFourierTransform::FFTW(complex<float> *samples, complex<float> *FD, int log2N)
{
    int fftPoint = 1 << log2N;				// 计算傅里叶变换点数
//    fftwf_complex *in = reinterpret_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * fftPoint));
//    fftwf_complex *out = reinterpret_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * fftPoint));
//    fftwf_complex *in = reinterpret_cast<fftwf_complex*>(samples);
//    fftwf_complex *out = reinterpret_cast<fftwf_complex*>(FD);

//    for (int i = 0; i < fftPoint; i++)
//    {
//        in[i][0] = samples[i];  // real
//        in[i][1] = 0;           // image
//    }

    // 输入数据in赋值
    fftwf_plan p = fftwf_plan_dft_1d(fftPoint, reinterpret_cast<fftwf_complex*>(samples), reinterpret_cast<fftwf_complex*>(FD), FFTW_FORWARD, FFTW_ESTIMATE);
    fftwf_execute(p); // 执行变换

//    for (int i = 0; i < fftPoint; i++)
//    {
//        FD[i] = out[i][0];
//    }
    fftwf_destroy_plan(p);
    //    fftwf_free(in);
//        fftwf_free(out);
}

void FastFourierTransform::FFTWF(float *samples, float *FD, int log2N)
{
    int fftPoint = 1 << log2N;				// 计算傅里叶变换点数
    fftwf_complex *in = reinterpret_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * fftPoint));
    fftwf_complex *out = reinterpret_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * fftPoint));

    for (int i = 0; i < fftPoint; i++)
    {
        in[i][0] = samples[i];  // real
        in[i][1] = 0;           // image
    }

    // 输入数据in赋值
    fftwf_plan p = fftwf_plan_dft_1d(fftPoint, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftwf_execute(p); // 执行变换

    for (int i = 0; i < fftPoint; i++)
    {
        FD[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    }
    fftwf_destroy_plan(p);
    //    fftwf_free(in);
//        fftwf_free(out);
}
