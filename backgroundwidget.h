#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "windows.h"
#include <winuser.h>
#include <QWidget>
#include <QTimer>
#include <QString>
#include "spectrum.h"

using namespace std;

class BackgroundWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget *parent = nullptr);
    ~BackgroundWidget();
    void SetAudioData(BYTE *data, const int interval);
    void SetDisplayArea(const char area);
    void SetAmpGrade(const double grade);

private:
    QTimer *timer;
    Spectrum *SpecGraph;			// 用于处理数据并且绘制频谱图形

    short *AudioData;
    int cxScreen;
    int cyScreen;
    int taskbarHeight;

    void SetBackgroundWMChild(QWidget* widget);
    void SetTaskbarWMChild(QWidget* widget);

signals:
    void ConsoleDataReady(const QString *str);

private slots:
    void Slot_Time_Out();
};

#endif // BACKGROUNDWIDGET_H
