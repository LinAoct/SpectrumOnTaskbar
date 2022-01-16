#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <winuser.h>
#include <uxtheme.h>
#include <QWidget>
//#include <QTimer>
#include <QColor>
#include <QString>
#include "spectrum.h"

using namespace std;

class BackgroundWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget *parent = nullptr);
    ~BackgroundWidget();
    void SetAudioData(BYTE *data, const int size);
    void SetDisplayArea(const char area);
    void SetAmpGrade(const double grade);
    void SetBlurGrade(const int grade);
    void SetPainterOpacity(const double value);
    void SetSpectrumStyle(const int value);
    void SetTextureStyle(const Spectrum::TextureStyle style);
    void SetUpdateSpeed(const int value);
    void SetPureColor(const QColor *color);

    LRESULT  WindowProc(UINT msg, WPARAM wp, LPARAM lp);

private:
    QTimer *timer;
    Spectrum *SpecGraph;			// 用于处理数据并且绘制频谱图形

    short *AudioData;
    int cxScreen;
    int cyScreen;
    int taskbarHeight;
    int taskbarX;
    int taskbarY;
    int updateSpeed;
    bool bPause=0;

    short count=0;

    void SetBackgroundWMChild(QWidget* widget);
    void SetTaskbarWMChild(QWidget* widget);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void RegAppBarData();
    void UnRegAppBarData();
    void SetLower();

signals:
    void ConsoleDataReady(const QString *str);

private slots:

};

#endif // BACKGROUNDWIDGET_H
