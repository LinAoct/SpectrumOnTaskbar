#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "mainwindow.h"
#include "ui_mainwindow.h"


// static const int timeConst = 125; // 采样时间间隔(ms)
// static const int barConst = 32;	  // 频谱条个数

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("实时频谱显示"));    // 主窗口标题设置
    std::cout << "hello" << std::endl;

    connect(ui->btn_show, SIGNAL(clicked(bool)), this, SLOT(Slot_On_Show_Btn_Clicked()));
    connect(ui->btn_close, SIGNAL(clicked(bool)), this, SLOT(Slot_On_Close_Btn_Clicked()));
    ui->btn_show->setEnabled(true);
    ui->btn_close->setEnabled(false);
    ui->console->setReadOnly(true); // 设置信息输出框为只读

    audioList = new AudioDeviceList(ui->comboBox_inputDevice, ui->comboBox_outputDevice);

    AudioRecordThread *recordThread = new AudioRecordThread();
    connect(recordThread, &AudioRecordThread::DataReady, this, &MainWindow::CopyData); // 连接槽函数
    connect(recordThread, &AudioRecordThread::ConsoleDataReady, this, &MainWindow::WriteConsoleData);
    recordThread->start();  // 音频检测线程启动

    connect(ui->slider_AmpSetting, SIGNAL(valueChanged(int)), this, SLOT(Slot_OnAmpSliderChanged(int)));
    ui->btn_show->click();
}

MainWindow::~MainWindow()
{
    // 若窗口已经创建 则关闭窗口再退出
    if(this->bBackgroundWidgetCreated)
        delete backgroundWidget;
    this->Reset_Wallpaper();    // 壁纸恢复
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    *bAudioThreadDone = true;
    // 若窗口已经创建 则关闭窗口再退出
    if(this->bBackgroundWidgetCreated)
    {
        this->bBackgroundWidgetCreated = false;
        this->Reset_Wallpaper();    // 壁纸恢复
        delete backgroundWidget;
    }
    event->accept();
}

void MainWindow::CopyData(BYTE *data, unsigned long numFrames, BOOL *bDone)
{
    static byte bPrintNum = false;
    bAudioThreadDone = bDone;
    if(data && this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetAudioData(data, static_cast<int>(numFrames));
    }
    if(!bPrintNum)
    {
        QString str = QString::asprintf("numFrames:%d", numFrames);
        WriteConsoleData(&str);
        bPrintNum = true;
    }
}

void MainWindow::Slot_On_Show_Btn_Clicked()
{
    backgroundWidget = new BackgroundWidget(nullptr);
    connect(backgroundWidget, &BackgroundWidget::ConsoleDataReady, this, &MainWindow::WriteConsoleData);
    if(ui->radioButton_modeOnBackground->isChecked())
        backgroundWidget->SetDisplayArea(1);    // 设置频谱集成到桌面背景
    if(ui->radioButton_modeOnTaskbar->isChecked())
        backgroundWidget->SetDisplayArea(2);    // 设置频谱集成到任务栏
    ui->radioButton_modeOnTaskbar->setEnabled(false);
    ui->radioButton_modeOnTaskbar->setEnabled(false);
    backgroundWidget->show();
    ui->btn_show->setEnabled(false);
    ui->btn_close->setEnabled(true);
    this->bBackgroundWidgetCreated = true;
}

void MainWindow::Slot_On_Close_Btn_Clicked()
{
    this->bBackgroundWidgetCreated = false;
    delete backgroundWidget;
    this->Reset_Wallpaper();    // 恢复默认壁纸
    ui->btn_show->setEnabled(true);
    ui->btn_close->setEnabled(false);
    ui->radioButton_modeOnTaskbar->setEnabled(true);
    ui->radioButton_modeOnTaskbar->setEnabled(true);
}

void MainWindow::WriteConsoleData(const QString *str)
{
    qDebug().noquote() << *str;
    ui->console->append(*str);
}

void MainWindow::Slot_OnAmpSliderChanged(int value)
{
    if(this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetAmpGrade(value/10.0);
    }
}

void MainWindow::Reset_Wallpaper()
{
    // 调用该函数刷新壁纸，使得壁纸恢复
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0,
                          nullptr, SPIF_SENDWININICHANGE);
}
