#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QLabel>
#include <QDebug>
#include <QVector>
#include <qmath.h>
#include <QGraphicsBlurEffect>
#include "FFT.h"

class FastFourierTransform;

using namespace std;

typedef struct ColorStruct
{
    short side;
    QColor color;
    ColorStruct *next;
}GradientColorStruct;


class Spectrum : public QLabel
{
	Q_OBJECT
    friend class BackgroundWidget;

public:
    Spectrum(QWidget *parent = nullptr);
    ~Spectrum();

    /**
     * @brief 频谱显示样式枚举
     */
    enum DisplayStyle
    {
        RectangleStyle,
        BlendWaveStyle,
        RoundWaveStyle
    };

    /**
     * @brief 频谱填充纹理样式枚举
     */
    enum TextureStyle
    {
        SolidStyle,
        GradientStyle,
        PatternStyle
    };

    static const int FFTPoint = 1024;   // FFT点数
    int rectNum = 64;                   // 矩形个数
    double ampGrade = 1.0;              // 幅度比例
    double opacityValue = 1.0;          // 透明度
    int spectrumStyle = DisplayStyle::BlendWaveStyle;   // 显示样式
    int brushStyle = TextureStyle::GradientStyle;          // 纹理样式
    QColor PureColor;
	
private:
	Spectrum(const Spectrum &);
	Spectrum& operator=(const Spectrum &);
    bool GetPoints(int n, QVector<QPoint> &points, QVector<QPoint> &bzPoints);
    long combinationCalc(int n, int m);
    long factorialCalc(int n);
    long getCombination(int n, int m);
    long getFactorial(int n);
    bool YhTriangle(int n, QVector<int> &coms);
    void SetBlurGrade(const int grade);
    void SetTextureStyle(int style = TextureStyle::GradientStyle);
    void SetGradientColor(const short side, const QColor color);

    void AddColor(ColorStruct *gradientColorStruct);

	void paintEvent(QPaintEvent *event);
    QBrush brush;
    FastFourierTransform *FFT;	// 快速傅里叶变换
    QGraphicsBlurEffect *blurEffect;
    ColorStruct colorStruct;
    short *sample;				// 待变换的数据
    double *FFT_result;			// 变换后的结果
    bool CalculatePowerSpectrum(short *sampleData, int totalSamples,
                       int channels, double *power); // 进行功率谱分析

};
