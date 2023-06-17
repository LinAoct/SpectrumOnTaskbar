#define  MSG_APPBAR_MSGID WM_USER+100

#include "backgroundwidget.h"

BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
{
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::MSWindowsFixedSizeDialogHint |   // 设置窗口为无边框模式
//                         Qt::WindowTransparentForInput | Qt::WindowStaysOnBottomHint);  // 设置窗体不接受输入事件 实现鼠标穿透
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::MSWindowsFixedSizeDialogHint |   // 设置窗口为无边框模式
                         Qt::WindowTransparentForInput);  // 设置窗体不接受输入事件 实现鼠标穿透
    this->setAttribute(Qt::WA_TranslucentBackground, true); // 设置窗口样式为透明
    this->setWindowTitle("SpectrumDisplay");

    cxScreen = GetSystemMetrics(SM_CXSCREEN);   // 获取屏幕宽度
    cyScreen = GetSystemMetrics(SM_CYSCREEN);   // 获取屏幕高度

    HWND hwnd = ::FindWindowExA(nullptr, nullptr, "Shell_TrayWnd", nullptr);
    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    taskbarHeight = rect.bottom - rect.top;
    taskbarX = rect.left;
    taskbarY = rect.top;

    SpecGraph = new Spectrum(this); // 实例化频谱显示 label
    this->SpecGraph->resize(cxScreen, taskbarHeight);   // 频谱显示窗口大小重设
    this->SpecGraph->move(0, 0);   // 频谱显示 label 位置移动
    this->SpecGraph->SetTextureStyle();

    AudioData = new short[this->SpecGraph->FFTPoint*2];

    this->RegAppBarData();


//    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(Slot_Time_Out()));
//    this->timer->start(20);
}

BackgroundWidget::~BackgroundWidget()
{
    qDebug() << "Background widget destroied.";
    this->UnRegAppBarData();
//    delete timer;
    delete [] AudioData;
    delete SpecGraph;
}

void BackgroundWidget::SetDisplayArea(const char area)
{
    switch (area)
    {
        case 1:
        {
            HWND hwnd = reinterpret_cast<HWND>(this->winId());  // 频谱窗口句柄
            SetBackgroundWMChild(this);         // 设置为桌面壁纸子窗口
            HWND parent = ::GetParent(hwnd);    // 获取频谱窗口的父窗口
            RECT rect;
            ::GetWindowRect(parent, &rect);     // 获取父窗口的位置信息
            // 移动子窗口位置
            int X = taskbarX, Y = taskbarY;
            if (rect.left < 0)  // 若多屏显示 且主屏幕非最左侧
            {
                X -= rect.left;
            }
            if (rect.top < 0)  // 若多屏显示 且主屏幕非最上侧
            {
                Y -= rect.top;
            }
            ::MoveWindow(hwnd, X, Y, cxScreen, taskbarHeight, true);
            break;
        }
        case 2:
        {
            this->setGeometry(0, taskbarHeight - cyScreen, cxScreen, taskbarHeight);    // 任务栏模式
//        this->setGeometry(0, 0, cxScreen, taskbarHeight);  // 桌面背景模式
            SetTaskbarWMChild(this);
            break;
        }
        default: break;
    }
    qDebug("主屏幕大小: %dx%d | 任务栏高度: %d (%d, %d)",
           cxScreen, cyScreen, taskbarHeight, taskbarX, taskbarY);
}

void BackgroundWidget::SetAudioData(BYTE *data, const int size)
{
    // 检测当前系统活动窗口是否为全屏 是则停止显示
    if(this->bPause)
    {
        return;
    }

    static char delayCount = 0;
    // 若数据大小为0 则表示系统静音 频谱置0
    if(size == 0)
    {
        memset(AudioData, 0, this->SpecGraph->FFTPoint*2);
        this->SpecGraph->CalculatePowerSpectrum(AudioData,
                                                this->SpecGraph->FFTPoint*2,
                                                2,
                                                SpecGraph->FFT_result);
        this->SpecGraph->update();
        return;
    }

    // 遍历音频缓存数据
    for(int i=0; i<size; i++)
    {
        if(m_Cnt != this->SpecGraph->FFTPoint*2)
        {
            AudioData[m_Cnt++] = static_cast<short>(data[i*4] + data[i*4+1]*256);   // 左声道数据
            AudioData[m_Cnt++] = static_cast<short>(data[i*4+2] + data[i*4+3]*256); // 右声道数据
        }
        else
        {
            if(delayCount < updateSpeed) break;
            this->SpecGraph->CalculatePowerSpectrum(AudioData,
                                                    this->SpecGraph->FFTPoint*2,
                                                    2,
                                                    SpecGraph->FFT_result);
            this->SpecGraph->update();
            // this->SetLower();
            m_Cnt = 0;
            delayCount = 0;
            break;
        }
    }
    delayCount++;
}

void BackgroundWidget::SetAmpGrade(double grade)
{
    this->SpecGraph->ampGrade = grade;
}

/**
 * @brief 设置频谱模糊
 * @param grade 模糊半径
 */
void BackgroundWidget::SetBlurGrade(const int grade)
{
    this->SpecGraph->SetBlurGrade(grade);
}

/**
 * @brief 设置频谱透明度
 * @param value 透明度大小 1为不透明 0为全透明
 */
void BackgroundWidget::SetPainterOpacity(const double value)
{
    this->SpecGraph->opacityValue = value;
}

/**
 * @brief 设置频谱样式
 * @param value 频谱样式
 */
void BackgroundWidget::SetSpectrumStyle(const int value)
{
    this->SpecGraph->spectrumStyle = value;
}

/**
 * @brief 设置频谱更新速度
 * @param value 更新速度
 */
void BackgroundWidget::SetUpdateSpeed(const int value)
{
    this->updateSpeed = value+3;   // 设置更新频率
}

/**
 * @brief 设置频谱填充样式
 * @param value TextureStyle填充样式枚举值
 */
void BackgroundWidget::SetTextureStyle(const Spectrum::TextureStyle style)
{
    this->SpecGraph->SetTextureStyle(style);
}

/**
 * @brief 设置频谱显示为纯色
 * @param color 要显示的颜色
 */
void BackgroundWidget::SetPureColor(const QColor *color)
{
    this->SpecGraph->PureColor = *color;
    this->SpecGraph->SetTextureStyle(Spectrum::SolidStyle);
}

void BackgroundWidget::SetBackgroundWMChild(QWidget* widget)
{
    HWND background = nullptr;
    HWND currentHWND = reinterpret_cast<HWND>(widget->winId());
    HWND hwnd = ::FindWindowA("progman", "Program Manager");
    int retry = 0;
    while(1)
    {
        HWND worker = nullptr;
        do
        {
            worker = ::FindWindowExA(nullptr, worker, "WorkerW", nullptr);
            if(GetParent(worker) == hwnd)
            {
                background = worker;
            }
        }
        while(worker != nullptr);

        if(background == nullptr)
        {
            SendMessageA(hwnd, 0x052C, 0, 0);
        }
        else
        {
            break;
        }
        if(retry++ > 3)
        {
            return;
        }
    }
    // 设置频谱窗口为桌面壁纸子窗口
    ::SetParent(currentHWND, background);
    // 设置窗口样式属性
    ::SetWindowLongW(reinterpret_cast<HWND>(widget->winId()),
                     GWL_STYLE,
                     WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN);
    // 更新窗口样式缓存
    ::SetWindowPos(reinterpret_cast<HWND>(this->winId()),
                   HWND_TOP,
                   taskbarX,
                   taskbarY,
                   cxScreen,
                   taskbarHeight,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void BackgroundWidget::SetTaskbarWMChild(QWidget* widget)
{
    HWND background = nullptr;
    HWND currentHWND = reinterpret_cast<HWND>(widget->winId());

    background = ::FindWindowExA(nullptr, nullptr, "Shell_TrayWnd", nullptr);

//    background = ::FindWindowExA(background, nullptr, "ReBarWindow32", nullptr);

//    this->SetLower();
    SetParent(currentHWND, background);
    ::SetWindowLongW(reinterpret_cast<HWND>(widget->winId()),
                     GWL_STYLE,
                     WS_CHILD | WS_TILED);
    ::SetWindowLongW(reinterpret_cast<HWND>(widget->winId()),
                     GWL_EXSTYLE,
                     WS_EX_LAYERED);
//    this->lower();
    this->setGeometry(0, 0, 1920, 40);
    // 更新窗口样式缓存
    ::SetWindowPos(reinterpret_cast<HWND>(widget->winId()),
                   HWND_BOTTOM,
                   taskbarX,
                   taskbarY,
                   cxScreen,
                   taskbarHeight,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
}

void BackgroundWidget::SetLower()
{
//    this->setGeometry(0, -taskbarHeight, cxScreen, taskbarHeight);    // 任务栏模式
    this->lower();
    HWND hwnd = ::FindWindowA("Shell_TrayWnd", nullptr);
//    hwnd = ::FindWindowExA(hwnd, nullptr, "ReBarWindow32", nullptr);
//    SendMessageA(hwnd, 0x052C, 0, 0);
    ::PostMessageA(hwnd, WM_NULL, 0, 0);
    ::RedrawWindow(hwnd, nullptr, nullptr, RDW_FRAME|RDW_VALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

/**
 * @brief 注册AppBarData 实现接收 Explorer 广播的全屏消息
 */
void BackgroundWidget::RegAppBarData()
{
    memset(&this->appBarData, 0, sizeof(appBarData));
    // 设定 appBarData 的参数
    this->appBarData.cbSize = sizeof(APPBARDATA);
    this->appBarData.hWnd = reinterpret_cast<HWND>(this->winId());;
    this->appBarData.uCallbackMessage = MSG_APPBAR_MSGID;

    qDebug() << this->appBarData.hWnd;

    ::SHAppBarMessage(ABM_NEW, &this->appBarData);
}

/**
 * @brief 反注册AppBarData
 */
void BackgroundWidget::UnRegAppBarData()
{
//    APPBARDATA appBarData;
//    memset(&appBarData, 0, sizeof(appBarData));
//    appBarData.cbSize = sizeof(APPBARDATA);
//    appBarData.hWnd = reinterpret_cast<HWND>(this->winId());;
//    appBarData.uCallbackMessage = MSG_APPBAR_MSGID;

    ::SHAppBarMessage(ABM_REMOVE, &this->appBarData);
}

/**
 * @brief 窗体接收到消息事件
 * @param eventType 事件类型
 * @param message   事件消息
 * @param result
 * @return
 */
bool BackgroundWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG *msg = reinterpret_cast<MSG*>(message);
    if(msg->message == MSG_APPBAR_MSGID &&
       msg->wParam == ABN_FULLSCREENAPP)
    {
        this->bPause = static_cast<bool>(msg->lParam);
        qDebug() << "接收到全屏广播信息 bPause: " << this->bPause;
        qDebug() << msg->hwnd;
    }

    return 0;
}
