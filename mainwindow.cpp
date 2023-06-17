#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>

#define WINDOW_ICON_PATH    ":/assets/ico/icon.ico"
#define TRAY_ICON_PATH      ":/assets/icon.png"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("实时频谱显示"));   // 主窗口标题设置
    this->setWindowIcon(QIcon(TRAY_ICON_PATH));           // 窗口图标

    // QTranslator SysTranslator;
    // AppTranslator.load(QString(":/Languages/qm/app_zh_CN.qm");
    // SysTranslator.load("qt_zh_CN.qm", "./");
    // qApp->installTranslator(&SysTranslator);

    connect(ui->btn_show, SIGNAL(clicked(bool)), this, SLOT(Slot_OnShowBtnClicked()));
    connect(ui->btn_close, SIGNAL(clicked(bool)), this, SLOT(Slot_OnCloseBtnClicked()));

    // UI控件初始化
    ui->btn_show->setEnabled(true);
    ui->btn_close->setEnabled(false);

    // 波形样式选项添加到控件
    ui->comboBox_StyleSeletec->addItems(DisplayStyleNameList);
    ui->comboBox_StyleSeletec->setCurrentIndex(Config::config_read_display_style());

    // 显示系统托盘
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(TRAY_ICON_PATH));
    systemTray->setToolTip("任务栏实时频谱显示");

    systemTrayMenu = new QMenu(reinterpret_cast<QWidget*>(QApplication::desktop()));
    QAction *trayActionQuit = new QAction(systemTrayMenu);
    trayActionQuit->setText("退出");

    QAction *trayActionRefresh = new QAction(systemTrayMenu);
    trayActionRefresh->setText("刷新");

    systemTrayMenu->addAction(trayActionRefresh);
    systemTrayMenu->addAction(trayActionQuit);
    systemTray->setContextMenu(systemTrayMenu);

    connect(trayActionRefresh, &QAction::triggered, this, &MainWindow::Refresh);
    connect(trayActionQuit, &QAction::triggered, this, &MainWindow::Quit);
    connect(systemTray, &QSystemTrayIcon::activated, this, &MainWindow::OnSystemTrayActived);   // 连接托盘动作执行函数
    systemTray->show();

//    audioList = new AudioDeviceList(ui->comboBox_inputDevice, ui->comboBox_outputDevice);

    AudioRecordThread *recordThread = new AudioRecordThread();
    connect(recordThread, &AudioRecordThread::DataReady, this, &MainWindow::CopyData); // 连接槽函数
    recordThread->start();  // 音频录制线程启动

    connect(ui->slider_AmpValue, SIGNAL(valueChanged(int)), this, SLOT(Slot_OnAmpSliderChanged(int)));
    connect(ui->slider_BlurValue, SIGNAL(valueChanged(int)), this, SLOT(Slot_OnBlurSliderChanged(int)));
    connect(ui->comboBox_StyleSeletec, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_OnStyleChanged(int)));
    connect(ui->comboBox_OpacityValue, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_OnOpacityChanged(int)));
    connect(ui->comboBox_UpdateSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_OnUpdateSpeedChanged(int)));

    ui->slider_AmpValue->setValue(Config::config_read_amp_value());

    connect(ui->btn_PureColorSelect, SIGNAL(clicked(bool)), this, SLOT(Slot_OnPureColorSelectButtonClicked()));

    // 判断是否开启开机自启功能
    const char *regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
    const char *key = "SpectrumDisplay";
    unsigned char *value = reinterpret_cast<unsigned char *>(malloc(1024));
    if (CommonHelp::ReadReg(regPath, key, value) == 0)
    {
        // 获取程序当前路径
        QString filePath = qApp->applicationFilePath();
        filePath.replace("/", "\\");
        filePath = QString::asprintf("\"%s\" -s", filePath.toStdString().c_str());
        // 比较当前路径与注册表信息
        if (strcmp(reinterpret_cast<char *>(value), filePath.toStdString().c_str()) == 0)
        {
            // 匹配一致 开机自启功能已启用
            this->ui->checkBox_BAutoBoot->setChecked(true);
        }
    }
    connect(ui->checkBox_BAutoBoot, SIGNAL(stateChanged(int)), this, SLOT(Slot_OnAutoBootStateChange(int))); // UI改变后再绑定信号槽 避免重复调用
    free(value);

    // 设置颜色选择器
    ColorSelectWidget *colorSelectorWidget = new ColorSelectWidget(this);
    ui->verticalLayout_ColorSelector->addWidget(colorSelectorWidget);

    textureButtonGroup = new QButtonGroup(this);
    textureButtonGroup->addButton(ui->radioButton_TextureStyle_Soild, Spectrum::SolidStyle);
    textureButtonGroup->addButton(ui->radioButton_TextureStyle_Gradient, Spectrum::GradientStyle);
    textureButtonGroup->addButton(ui->radioButton_TextureStyle_StableRGB, Spectrum::StableRGBStyle);
    textureButtonGroup->addButton(ui->radioButton_TextureStyle_SlideRGB, Spectrum::SlideRGBStyle);
    textureButtonGroup->addButton(ui->radioButton_TextureStyle_Pattern, Spectrum::PatternStyle);
    connect(ui->radioButton_TextureStyle_Soild, SIGNAL(clicked(bool)), this, SLOT(Slot_OnTextureRadioButtonClicked()));
    connect(ui->radioButton_TextureStyle_Gradient, SIGNAL(clicked(bool)), this, SLOT(Slot_OnTextureRadioButtonClicked()));
    connect(ui->radioButton_TextureStyle_StableRGB, SIGNAL(clicked(bool)), this, SLOT(Slot_OnTextureRadioButtonClicked()));
    connect(ui->radioButton_TextureStyle_SlideRGB, SIGNAL(clicked(bool)), this, SLOT(Slot_OnTextureRadioButtonClicked()));
    connect(ui->radioButton_TextureStyle_Pattern, SIGNAL(clicked(bool)), this, SLOT(Slot_OnTextureRadioButtonClicked()));

    ui->radioButton_TextureStyle_StableRGB->setChecked(true);    // 默认选中

    ui->btn_show->click();

}

MainWindow::~MainWindow()
{
    delete textureButtonGroup;
    delete systemTrayMenu;
    delete systemTray;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent.";
    this->hide();
    event->accept();
}

void MainWindow::Quit()
{
    qDebug() << "Quit Function.";
    *bAudioThreadDone = true;
    // 若窗口已经创建 则关闭窗口再退出
    if (this->bBackgroundWidgetCreated)
    {
        this->bBackgroundWidgetCreated = false;
        this->ResetWallpaper();    // 壁纸恢复
        delete backgroundWidget;
    }
    this->close();
}

/**
 * @brief 托盘刷新动作函数
 */
void MainWindow::Refresh()
{
    if (ui->btn_close->isEnabled())
    {
        ui->btn_close->click();
        ui->btn_show->click();
    }
}

/**
 * @brief <槽函数> 系统音频数据获取成功后调用此函数
 * @param data  音频数据
 * @param numFrames 音频数据长度
 * @param bDone 标志是否获取完成 改变此变量可退出音频录制线程
 */
void MainWindow::CopyData(BYTE *data, unsigned long numFrames, BOOL *bDone)
{
    bAudioThreadDone = bDone;
    if(this->bBackgroundWidgetCreated)
    {
        if(data != nullptr)
        {
            backgroundWidget->SetAudioData(data, static_cast<int>(numFrames));
        }
        else
        {
            backgroundWidget->SetAudioData(nullptr, 0);
        }

    }
}

/**
 * @brief <槽函数> 显示频谱按钮点击事件
 */
void MainWindow::Slot_OnShowBtnClicked()
{
    backgroundWidget = new BackgroundWidget(nullptr);
    backgroundWidget->SetSpectrumStyle(ui->comboBox_StyleSeletec->currentIndex());              // 设置频谱样式
    backgroundWidget->SetUpdateSpeed(ui->comboBox_UpdateSpeed->currentIndex());                 // 设置更新速度
    backgroundWidget->SetPainterOpacity(ui->comboBox_OpacityValue->currentText().toDouble());   // 设置透明度大小
    backgroundWidget->SetAmpGrade(ui->slider_AmpValue->value()/100.0);                          // 设置幅度大小
    backgroundWidget->SetBlurGrade(ui->slider_BlurValue->value());                              // 设置模糊度大小
    if(ui->radioButton_modeOnBackground->isChecked())
        backgroundWidget->SetDisplayArea(1);    // 设置频谱集成到桌面背景
    if(ui->radioButton_modeOnTaskbar->isChecked())
        backgroundWidget->SetDisplayArea(2);    // 设置频谱集成到任务栏
    ui->radioButton_modeOnBackground->setEnabled(false);
    ui->radioButton_modeOnTaskbar->setEnabled(false);
    backgroundWidget->show();
    ui->btn_show->setEnabled(false);
    ui->btn_close->setEnabled(true);
    this->bBackgroundWidgetCreated = true;
}

/**
 * @brief <槽函数> 关闭窗口按钮点击事件
 */
void MainWindow::Slot_OnCloseBtnClicked()
{
    this->bBackgroundWidgetCreated = false;
    delete backgroundWidget;
    this->ResetWallpaper();    // 恢复默认壁纸
    ui->btn_show->setEnabled(true);
    ui->btn_close->setEnabled(false);
    ui->radioButton_modeOnBackground->setEnabled(true);
    ui->radioButton_modeOnTaskbar->setEnabled(true);
}

/**
 * @brief <槽函数> 系统托盘图标点击触发事件
 * @param reason 触发原因
 */
void MainWindow::OnSystemTrayActived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:      // 左击
        {
            if(this->isHidden())
            {
                this->showNormal();
            }
            this->activateWindow();
            break;
        }
        case QSystemTrayIcon::DoubleClick:  // 左键双击
        {
            if(this->isHidden())
            {
                this->showNormal();
            }
            this->activateWindow();
            break;
        }
        case QSystemTrayIcon::Context:      // 右击
        {
            systemTrayMenu->show();
            break;
        }
        default: break;
    }
}

/**
 * @brief <槽函数> UI幅度大小滑块数值更改事件
 * @param value 更改后的数值大小
 */
void MainWindow::Slot_OnAmpSliderChanged(const int value)
{
    if(this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetAmpGrade(value/100.0);
        Config::config_save_amp_value(value);
    }
}

/**
 * @brief <槽函数> 模糊度大小滑块数值更改事件
 * @param value 更改后的数值大小
 */
void MainWindow::Slot_OnBlurSliderChanged(const int value)
{
    if(this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetBlurGrade(value);
    }
}

/**
 * @brief <槽函数> UI透明度大小下拉框数值更改事件
 * @param index 更改后的文本的下标
 */
void MainWindow::Slot_OnOpacityChanged(const int index)
{
    if(this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetPainterOpacity(ui->comboBox_OpacityValue->itemText(index).toDouble());
    }
}

/**
 * @brief <槽函数> 样式下拉框下标更改事件
 * @param index 更改后的下标
 */
void MainWindow::Slot_OnStyleChanged(const int index)
{
    if(this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetSpectrumStyle(index);
        Config::config_save_display_style(index);
    }
}

/**
 * @brief <槽函数> 刷新率下拉框下标更改事件
 * @param index 更改后的下标
 */
void MainWindow::Slot_OnUpdateSpeedChanged(const int index)
{
    if(this->bBackgroundWidgetCreated)
    {
        backgroundWidget->SetUpdateSpeed(index);
    }
}

/**
 * @brief <槽函数> 开机自启多选框状态改变事件
 * @param state 事件类型
 */
void MainWindow::Slot_OnAutoBootStateChange(const int state)
{
    const char *regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
    const char *key ="SpectrumDisplay";
    switch(state)
    {
        case Qt::CheckState::Checked:
        {
            QString filePath = qApp->applicationFilePath();
            filePath.replace("/", "\\");
            filePath = QString::asprintf("\"%s\" -s", filePath.toStdString().c_str());
            CommonHelp::WriteReg(regPath, key, filePath.toStdString().c_str());
            break;
        }
        case Qt::CheckState::Unchecked:
        {
            CommonHelp::DelRegKey(regPath, key);
            break;
        }
        default: break;
    }
}

/**
 * @brief 填充样式 radiobutton 点击事件
 */
void MainWindow::Slot_OnTextureRadioButtonClicked()
{
    this->backgroundWidget->SetTextureStyle(static_cast<Spectrum::TextureStyle>(this->textureButtonGroup->checkedId()));
}

void MainWindow::Slot_OnPureColorSelectButtonClicked()
{
    // rejected信号会在QColorDialog框关闭或按Cancel按钮时1发出，可通过绑定该信号来进行Cancel信号过滤
    // connect(colorDialog, SIGNAL(rejected()),this,SLOT(SetColor_1()));
    QColorDialog *colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(currentColor); // 设置对话框默认颜色
    connect(colorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(Slot_CurrentColorChanged(QColor)));    // 显示当前选中颜色的效果
    connect(colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(Slot_CurrentColorChanged(QColor)));          // OK信号连接
    colorDialog->setOption(QColorDialog::ShowAlphaChannel);
    colorDialog->exec();
    // qDebug() << color.rgb() << color.red() << color.green() << color.blue();
    delete colorDialog;
}

void MainWindow::Slot_CurrentColorChanged(QColor color)
{
    qDebug() << color.rgb() << color.red() << color.green() << color.blue();
    currentColor = color;
    QString styleStr = styleStr.asprintf("QPushButton{background-color: rgb(%d, %d, %d); \
                                          border-style: solid; \
                                          border-width: 0px;}", color.red(), color.green(), color.blue());
    ui->btn_PureColorSelect->setStyleSheet(styleStr);   // 设置按钮背景样式
    ui->btn_PureColorSelect->setText(QString().asprintf("(%d,%d,%d)", color.red(), color.green(), color.blue()));   // 设置按钮文本
    this->backgroundWidget->SetPureColor(&currentColor);
}

/**
 * @brief 刷新壁纸 恢复桌面壁纸
 */
void MainWindow::ResetWallpaper()
{
    // 调用该函数刷新壁纸，使得壁纸恢复
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0,
                          nullptr, SPIF_SENDWININICHANGE);
}
