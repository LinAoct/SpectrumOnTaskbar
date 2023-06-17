#pragma once

#include <iostream>
#include <time.h>
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
#include "SmoothCurveGenerator.h"
#include "commonhelp.h"


//static const QStringList DisplayStyleNameList(QStringList() <<
//                                              "矩形"  <<
//                                              "圆滑波形" <<
//                                              "混合波形"  <<
//                                              "圆角波形");

#define POWER_THRESHOLD_MIN 0.03
#define POWER_THRESHOLD_MAX 1.0
#define POWER_VALUE_MAX     1.0
#define POWER_VALUE_MIN     0.001

#define LOG_SHOW_POINT      48      // rectNum * 3/4

static const QStringList DisplayStyleNameList = {"矩形",
                                                 "圆滑波形",
                                                 "混合波形",
                                                 "圆角波形"};

class FastFourierTransform;

using namespace std;


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
        RectangleStyle, // 矩形样式
        SmoothWaveStyle,// 圆滑波形样式
        BlendWaveStyle, // 混合波形样式
        RoundWaveStyle  // 圆角波形样式
    };

    /**
     * @brief 频谱填充纹理样式枚举
     */
    enum TextureStyle
    {
        SolidStyle,     // 纯色样式
        GradientStyle,  // 渐变填充样式
        StableRGBStyle, // 固定RGB样式
        SlideRGBStyle,  // 变幻RGB样式
        PatternStyle    // 图案填充样式
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
    void DealRGBArray(qreal *colorArray);

//    void AddColor(ColorStruct *gradientColorStruct);

	void paintEvent(QPaintEvent *event);
    QBrush brush;
    FastFourierTransform *FFT;	// 快速傅里叶变换
    QGraphicsBlurEffect *blurEffect;
//    ColorStruct colorStruct;
    short *sample;				// 待变换的数据
    double *FFT_result;			// 变换后的结果
    bool CalculatePowerSpectrum(short *sampleData, int totalSamples,
                       int channels, double *power); // 进行功率谱分析
    // FPS 计算相关变量
    DWORD   m_StartTime = 0;
    short   m_FPSCnt    = 0;


};
