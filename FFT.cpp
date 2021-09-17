#include "FFT.h"
#include <cmath>
#include <qdebug.h>

static const double pi = 3.1415926535898;

// 快速傅里叶变换核心计算部分，进行原位计算，避免值传递浪费时间
void FastFourierTransform::process(complex<float>*Data, int Log2N, int sign)
{
    int i, j, step, length;
	complex<float> wn, temp, deltawn;
	length = 1 << Log2N;
//    qDebug() << length << Log2N;
	for (i = 0; i < length; i += 2)
	{
		temp = Data[i];
		Data[i] = Data[i] + Data[i + 1];
		Data[i + 1] = temp - Data[i + 1];
	}
	for (i = 2; i <= Log2N; i++)
	{
        wn = 1;
        step = 1 << i;
        deltawn = complex<float>(cos(2.0*pi / step), sin(sign*2.0*pi / step));
		for (j = 0; j < step / 2; j++)
		{
			for (i = 0; i < length / step; i++)
			{
				temp = Data[i*step + step / 2 + j] * wn;
				Data[i*step + step / 2 + j] = Data[i*step + j] - temp;
				Data[i*step + j] = Data[i*step + j] + temp;
			}
			wn = wn*deltawn;
		}
	}
	if (sign == 1)
		for (i = 0; i < length; i++)
			Data[i] /= length;
}
