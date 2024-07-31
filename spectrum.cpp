#include "spectrum.h"

#define TEST_TEXTURE_PATH   ":/assets/bbzl.png"


// 初始化对象和数据
Spectrum::Spectrum(QWidget *parent) : QLabel(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    FFT = new FastFourierTransform(FFTPoint);
    // FFT->PreCalc(this->FFTPoint);
    FFT_result = new double[FFTPoint];
    sample = new short[FFTPoint];

    // 设置窗体模糊效果
    blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint); // 性能优先
    this->setGraphicsEffect(blurEffect);
}

Spectrum::~Spectrum() 
{
    delete[] sample;
    delete[] FFT_result;
    delete FFT;
}

/**
 * @brief 设置频谱模糊半径
 * @param grade 模糊半径
 */
void Spectrum::SetBlurGrade(const int grade)
{
    this->blurEffect->setBlurRadius(grade);
}

/**
 * @brief 设置频谱纹理样式
 * @param style 纹理样式枚举值
 */
void Spectrum::SetTextureStyle(int style)
{
    switch (style)
    {
        case TextureStyle::SolidStyle:  // 纯色填充
        {
            this->brush = QBrush(PureColor);
            break;
        }
        case TextureStyle::GradientStyle: // 线性渐变
        {
            // 线性渐变画刷
            QLinearGradient linearGradient(0, this->height(), this->width(), this->height());
            linearGradient.setColorAt(0.0, Qt::white);
            linearGradient.setColorAt(0.5, Qt::green);
            linearGradient.setColorAt(1.0, Qt::red);

            brush = QBrush(linearGradient);
            break;
        }
        case TextureStyle::StableRGBStyle:  // 固定RGB填充
        {
            // 线性渐变画刷
            QLinearGradient linearGradient(0, this->height(), this->width(), this->height());

            linearGradient.setColorAt(0.0 / 6, Qt::green);
            linearGradient.setColorAt(1.0 / 6, Qt::cyan);
            linearGradient.setColorAt(2.0 / 6, Qt::blue);
            linearGradient.setColorAt(3.0 / 6, QColor(255, 0, 255));
            linearGradient.setColorAt(4.0 / 6, QColor(255, 255, 0));
            linearGradient.setColorAt(5.0 / 6, QColor(255, 97, 0));
            linearGradient.setColorAt(1, Qt::red);

            brush = QBrush(linearGradient); // 线性渐变
            break;
        }
        case TextureStyle::SlideRGBStyle:   // 滑动RGB填充
        {
            break;
        }
        case TextureStyle::PatternStyle:    // 图案纹理填充
        {
            brush = QBrush(QPixmap(TEST_TEXTURE_PATH));
            brush.setStyle(Qt::TexturePattern);
            QTransform tf;
            tf.scale(0.2, 0.2);
            brush.setTransform(tf);
            break;
        }
    }
    this->brushStyle = style;
}

void Spectrum::DealRGBArray(qreal *colorArray)
{
    for(int index = 0; index < 3; index++)
    {
        colorArray[index] += 0.01;
        if (colorArray[index] > 2.0)
        {
            colorArray[index] -= 2.0;
        }
    }
}

/**
 * @brief 频谱绘制更新事件，更新显示频谱图
 * @param event 更新事件
 */
void Spectrum::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    // 判断是否记录了开始时间 用于计算 FPS
    if (m_StartTime == 0)
    {
        m_StartTime = GetTickCount();
    }

    QPainter painter(this);

    painter.setOpacity(opacityValue);
    
    // 计算频谱图的位置信息
    int singleWidth = this->width() / rectNum;              // 单个频谱显示宽度
    int barWidth = static_cast<int>(0.99 * singleWidth);    // 频内图形宽度
    int gapWidth = singleWidth - barWidth;                  // 频内留空宽度
    int barHeight = this->height() - 2 * gapWidth;

    int temp = 0;
    QPointF zeroPoint = QPointF(0, this->height());
    QPointF startPoint;
    QPointF lastPoint;
    QPointF currentPoint;
    QPainterPath drawPath;
    drawPath.moveTo(zeroPoint);

    painter.setPen(QPen(Qt::transparent, 1)); // 设置画笔样式 透明 宽度 1px
    // painter.setRenderHint(QPainter::Antialiasing, true);    // 线条渲染抗锯齿

    static qreal colorArray[7] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    if (this->brushStyle == TextureStyle::SlideRGBStyle)     // 线性渐变画刷
    {
        QLinearGradient linearGradient(0, this->height(), this->width(), this->height());

        linearGradient.setColorAt(colorArray[0] / 2.0, Qt::red);
        linearGradient.setColorAt(colorArray[1] / 2.0, Qt::green);
        linearGradient.setColorAt(colorArray[2] / 2.0, Qt::blue);
        // linearGradient.setColorAt(colorArray[0] / 2, Qt::red);
        // linearGradient.setColorAt(colorArray[1] / 2, QColor(255, 97, 0));
        // linearGradient.setColorAt(colorArray[2] / 2, QColor(255, 255, 0));
        // linearGradient.setColorAt(colorArray[3] / 6, Qt::green);
        // linearGradient.setColorAt(colorArray[4] / 6, Qt::cyan);
        // linearGradient.setColorAt(colorArray[5] / 6, Qt::blue);
        // linearGradient.setColorAt(colorArray[6] / 6, QColor(255, 0, 255));

        DealRGBArray(colorArray);

        brush = QBrush(linearGradient);          // 线性渐变
    }

    // 对快速傅里叶变换的结果进行绘制 每个结果对应一个矩形
    for (int i = 0; i < rectNum; i++)
    {
        // 读取FFT结果
        double value = FFT_result[i];

        // 判断频谱显示样式
        switch(spectrumStyle)
        {
            case DisplayStyle::RectangleStyle:  // 若为矩形样式
            {
                QRect bar = rect();
                bar.setLeft(i * singleWidth);
                bar.setWidth(barWidth);
                bar.setTop(static_cast<int>(rect().top() + 2*gapWidth +
                            (1.0 - value) * barHeight));

                painter.fillRect(bar, this->brush);
                break;
            }
            case DisplayStyle::SmoothWaveStyle: // 若为圆滑波形样式
            {
                if (i == 0)
                {
                    lastPoint = QPointF(0, this->height());
                }
                currentPoint = QPointF(i*singleWidth+singleWidth/2,
                                       static_cast<int>((1.0 - value) * this->height()));

                QPointF ctrlPoint1 = QPointF((lastPoint.x() + currentPoint.x()) / 2,
                                     lastPoint.y());
                QPointF ctrlPoint2 = QPointF((lastPoint.x() + currentPoint.x()) / 2,
                                     currentPoint.y());
                // 连接曲线点
                drawPath.cubicTo(ctrlPoint1,
                                 ctrlPoint2,
                                 currentPoint);

                lastPoint = currentPoint;

                if (i == rectNum-1)
                {
                    // 曲线连接到右下角
                    currentPoint = QPointF(this->width(), this->height());
                    QPointF ctrlPoint1 = QPointF((lastPoint.x() + currentPoint.x()) / 2,
                                         lastPoint.y());
                    QPointF ctrlPoint2 = QPointF((lastPoint.x() + currentPoint.x()) / 2,
                                         currentPoint.y());
                    drawPath.cubicTo(ctrlPoint1,
                                     ctrlPoint2,
                                     currentPoint);

                    painter.drawPath(drawPath);
                    painter.fillPath(drawPath, this->brush);
                }

                break;
            }
            case DisplayStyle::BlendWaveStyle:  // 若为波形样式
            {
                // 计算当前数据点
                currentPoint = QPointF(i*singleWidth+singleWidth/2,
                                       static_cast<int>((1.0 - value) * this->height()));
                if (i == 1)
                {
                    drawPath.cubicTo(zeroPoint, lastPoint, currentPoint);
                    temp = 0;
                }
                if (i == rectNum-1)
                {
                    drawPath.cubicTo(lastPoint, currentPoint, QPointF(this->width(), this->height()));
                    painter.drawPath(drawPath);
                    painter.fillPath(drawPath, this->brush);
                    temp = 0;
                }
                if (temp == 3)
                {
                    drawPath.cubicTo(startPoint, lastPoint, currentPoint);
                    temp = 0;
                }
                startPoint = lastPoint;
                lastPoint = currentPoint;
                temp++;
                break;
            }
            case DisplayStyle::RoundWaveStyle:  // 若为圆角波形样式
            {
                currentPoint = QPointF(i*singleWidth+singleWidth/2,
                                       static_cast<int>((1.0 - value) * barHeight));
                // drawPath.quadTo(QPointF(i*singleWidth, this->height()), currentPoint);
                // drawPath.quadTo(currentPoint, QPointF(i*singleWidth+singleWidth, this->height()));
                drawPath.cubicTo(QPointF(i*singleWidth, this->height()), currentPoint, QPointF(i*singleWidth+singleWidth, this->height()));

                if (i == rectNum-1)
                {
                    painter.drawPath(drawPath);
                    painter.fillPath(drawPath, this->brush);
                }
                break;
            }
            default:break;
        }
    }
    event->accept();

    // 计算 FPS
    // m_FPSCnt++;
    // if ((GetTickCount() - m_StartTime) >= 1000)
    // {
    //     qDebug() << "FPS:" << m_FPSCnt;
    //     m_StartTime = 0;
    //     m_FPSCnt = 0;
    // }
}

#define FFTWAY 0
// 根据采样数据计算功率谱
bool Spectrum::CalculatePowerSpectrum(short *sampleData, int totalSamples,
                                      int channels, double *power)
{
    // 进行快速傅里叶变换之前必要的参数计算
    int log2N = static_cast<int>(log2(totalSamples/channels - 1)) + 1;

#if FFTWAY == 0
    complex<double> *sample = new complex<double>[FFTPoint];
    complex<double> *out = new complex<double>[FFTPoint];

    for (int i = 0; i < totalSamples; i += 2)
        sample[i/2] = complex<double>((sampleData[i] + sampleData[i+1]) / 65536.0, 0);

    // 对采样数据进行快速傅里叶变换
    FFT->Calculate(sample, out, log2N);
#endif
#if FFTWAY == 1
    complex<float> *sample = new complex<float>[FFTPoint];
    complex<float> *out = new complex<float>[FFTPoint];
    for (int i = 0; i < totalSamples; i += 2)
        sample[i/2] = complex<float>(((sampleData[i] + sampleData[i+1]) / 65536.0f), 0);
    // 对采样数据进行快速傅里叶变换
    FFT->FFTW(sample, out, log2N);
#endif
#if FFTWAY == 2
    float *sample = new float[FFTPoint];
    float *out = new float[FFTPoint];
    for (int i = 0; i < totalSamples; i += 2)
        sample[i/2] = ((sampleData[i] + sampleData[i+1]) / 65536.0f);
    // 对采样数据进行快速傅里叶变换
    FFT->FFTWF(sample, out, log2N);
#endif

    // 首先根据声道数将采样数据分组 随后全部转成复数 其中虚部为0 最后进行归一化。
    // if (channels == 1)
    // {
    //     sample = new complex<double>[totalSamples];
    //     for (int i = 0; i < totalSamples; i++)
    //         sample[i] = complex<double>(sampleData[i] / 32768.0, 0);
    // }
    // else
    // {
    //     sample = new complex<double>[totalSamples / 2];
    //     for (int i = 0; i < totalSamples; i += 2)
    //         sample[i/2] = complex<double>((sampleData[i] + sampleData[i+1]) / 65536.0, 0);
    // }

    int a = 11;
    int count = 0;
    // double rms_sum = 0;
    for(int i = 0; i < rectNum; i++)
    {
        if (rectNum == 128)
        {
            if (i<rectNum/4)
            {
                power[i] = abs(out[i]) * ampGrade;
            }
            else if (i<rectNum/2)
            {
                power[i] = 0;
                for(int j=0;j<2;j++)
                {
                    power[i] += abs(out[(rectNum/4)+((i-(rectNum/4))*2+j)]);
                }
                power[i] /= 2;
                power[i] *= ampGrade*3;
            }
            else if (i < rectNum/2+rectNum/4)
            {
                power[i] = 0;
                for(int j=0;j<3;j++)
                {
                    power[i] += abs(out[(rectNum/2)+((i-(rectNum/2))*3+j)]);
                }
                power[i] /= 3;
                power[i] *= ampGrade*3;
            }
            else
            {
                power[i] = 0;
                for(int j=0;j<a;j++)
                {
                    power[i] += abs(out[(rectNum/2+rectNum/4)+((i-(rectNum/2+rectNum/4))*a+j)]);
                }
                power[i] /= a;
                power[i] *= ampGrade*3;
            }
        }
        else if (rectNum == 64)
        {
            a = (FFTPoint/2 - LOG_SHOW_POINT) / (rectNum - LOG_SHOW_POINT);
            if (i<LOG_SHOW_POINT)
            {
                power[i] = abs(out[i]) * ampGrade;  // 0-16
                count++;
            }
            else
            {
                power[i] = 0;
                double max = abs(out[count]);
                for(int j=0;j<a;j++)
                {
                    max = MAX(max, abs(out[count]));
                    // rms_sum += abs(out[count]);
                    count++;
                }
                // rms_sum = sqrtf(rms_sum / a);
                
                power[i]  = max;
                // power[i] = rms_sum;
                power[i] *= ampGrade; // *8
            }
#if 0
            a = 20;
            if (i<rectNum/2)
            {
                power[i] = abs(out[i]) * ampGrade;  // 0-16
                count++;
            }
            else if (i < rectNum * 3/4)
            {
                power[i] = 0;
                for(int j=0; j<8; j++)
                {
                    power[i] += abs(out[count]);
                    count++;
                }
                power[i] /= 8;
                power[i] *= ampGrade; // *4
            }
            else
            {
                power[i] = 0;
                for(int j=0;j<a;j++)
                {
                    power[i] += abs(out[count]);
                    count++;
                }
                power[i] /= a;
                power[i] *= ampGrade; // *8
            }
#endif
        }
        // 最低幅度处理
        if (power[i] <= POWER_THRESHOLD_MIN)
            power[i] = POWER_VALUE_MIN;
        // 限幅处理
        if (power[i] > POWER_THRESHOLD_MAX)
            power[i] = POWER_VALUE_MAX;
    }
    delete [] sample;
    delete [] out;
    return true;
}
