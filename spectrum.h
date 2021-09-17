#pragma once
#include <QLabel>
#include <QDebug>
#include <vector>

class FastFourierTransform;

using namespace std;

class Spectrum : public QLabel
{
	Q_OBJECT
    friend class BackgroundWidget;

public:
    Spectrum(QWidget *parent = nullptr);
    ~Spectrum();

    double ampGrade = 1.0;
	
private:
	Spectrum(const Spectrum &);
	Spectrum& operator=(const Spectrum &);
	void paintEvent(QPaintEvent *event);
    FastFourierTransform *FFT;	// 快速傅里叶变换
    short *sample;				// 待变换的数据
    double *FFT_result;			// 变换后的结果
    bool powerSpectrum(short * sampleData, int totalSamples,
                       int channels, double *power); // 进行功率谱分析

};


