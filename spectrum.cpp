#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <cmath>
#include "spectrum.h"
#include "FFT.h"

static const int num = 32; // 矩形的个数，禁止修改
// static const double pi = 3.1415926535898;

// 初始化对象和数据
Spectrum::Spectrum(QWidget *parent):QLabel(parent)
{
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint); 
	FFT = new FastFourierTransform(num);
	FFT_result = new double[num];
	sample = new short[num];
	for (int i = 0; i < num; i++)
		FFT_result[i] = 0.0;
}

Spectrum::~Spectrum() 
{
	delete[] sample;
	delete[] FFT_result;
	delete FFT;
}

// 频谱图绘制部分
void Spectrum::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter painter(this);
	
	int barNum = num;
    if (barNum != 0)
    {
		// 计算频谱图的位置信息
		int singleWidth = this->width() / barNum;
        int barWidth = static_cast<int>(0.98 * singleWidth);
		int gapWidth = singleWidth - barWidth;
		int barHeight = this->height() - 2 * gapWidth;

		// 对快速傅里叶变换的结果进行绘制，每个结果对应一个矩形
		for (int i = 0; i < barNum; i++) {
            double value = FFT_result[i];
			QRect bar = rect();
			bar.setLeft(i * singleWidth);
			bar.setWidth(barWidth);
//            if (value > 0)
//            {
                bar.setTop(static_cast<int>(rect().top() + 2*gapWidth +
                            (1.0 - value) * barHeight));
//            }
//            else
//            {
//                bar.setTop(rect().top() + this->height());
//            }

//            bar.setBottom(rect().bottom() - gapWidth);
//            bar.setBottom(rect().bottom());
			// 这里设置了渐变色
            QColor barColor(7*i, static_cast<int>(0.9 * i * (num - i)), 7*(num - i));
			painter.fillRect(bar, barColor);
		}
	}
	event->accept();
}

// 根据采样数据计算功率谱
bool Spectrum::powerSpectrum(short * sampleData, int totalSamples,
    int channels, double *power)
{
    complex<float> *sample;

//	for (int i = 0; i < totalSamples; i++)
//		qDebug() << sampleData[i];

	// 首先根据声道数将采样数据分组，随后一一转成复数，其中虚部为0。最后进行归一化。
    if (channels == 1)
    {
		sample = new complex<float>[totalSamples];
		for (int i = 0; i < totalSamples; i++)
			sample[i] = complex<float>(sampleData[i] / 32768.0, 0);
	}
    else
    {
		sample = new complex<float>[totalSamples / 2];
		for (int i = 0; i < totalSamples; i += 2)
            sample[i/2] = complex<float>((sampleData[i] + sampleData[i+1]) / 65536.0, 0);
	}

	// 进行快速傅里叶变换之前必要的参数计算
    int log2N = log2(totalSamples/channels - 1) + 1;
    int sign = -1;

	// 对采样数据进行快速傅里叶变换
	FFT->process(sample, log2N, sign);

	// 将结果转成实数并进行适当的调整以满足屏幕显示的要求
    for (int i = 0; i < totalSamples/channels; i++)
    {
//        power[i] = abs(sample[i]) / sqrt(2) / 2;
        power[i] = abs(sample[i]) * ampGrade;
//        if (power[i] < 0 || power[i] > 1)
//            power[i] = 0;
    }
	delete [] sample;
	return true;
}
