#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "backgroundwidget.h"
#include "audiodevicelist.h"
#include "audiorecordthread.h"

using namespace std;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BackgroundWidget *backgroundWidget;
    AudioDeviceList *audioList;
    BOOL *bAudioThreadDone;

    void Reset_Wallpaper();

    bool bBackgroundWidgetCreated = false;

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void CopyData(BYTE *data, unsigned long numFrames, BOOL *bDone);
    void Slot_On_Show_Btn_Clicked();
    void Slot_On_Close_Btn_Clicked();
    void WriteConsoleData(const QString *str);
    void Slot_OnAmpSliderChanged(int value);

};

#endif // MAINWINDOW_H
