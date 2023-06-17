#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <QMainWindow>
#include <QTabBar>
#include <QString>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QColorDialog>
#include <QButtonGroup>
#include "commonhelp.h"
#include "backgroundwidget.h"
#include "audiorecordthread.h"
#include "colorselectwidget.h"
#include "config.h"

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
    QSystemTrayIcon *systemTray;
    QMenu *systemTrayMenu;
//    AudioDeviceList *audioList;
    QColor currentColor;
    QButtonGroup *textureButtonGroup;
    BOOL *bAudioThreadDone;
    bool bBackgroundWidgetCreated = false;

    void Quit();
    void ResetWallpaper();
    void Refresh();

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void OnSystemTrayActived(QSystemTrayIcon::ActivationReason reason);
    void CopyData(BYTE *data, unsigned long numFrames, BOOL *bDone);
    void Slot_OnShowBtnClicked();
    void Slot_OnCloseBtnClicked();
    void Slot_OnAmpSliderChanged(const int value);
    void Slot_OnBlurSliderChanged(const int value);
    void Slot_OnOpacityChanged(const int index);
    void Slot_OnStyleChanged(const int index);
    void Slot_OnUpdateSpeedChanged(const int index);
    void Slot_OnAutoBootStateChange(const int state);
    void Slot_OnPureColorSelectButtonClicked();
    void Slot_CurrentColorChanged(QColor color);
    void Slot_OnTextureRadioButtonClicked();
};

#endif // MAINWINDOW_H
