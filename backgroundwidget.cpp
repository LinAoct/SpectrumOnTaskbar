#include "backgroundwidget.h"

BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::MSWindowsFixedSizeDialogHint);   // 设置窗口为无边框模式
    this->setAttribute(Qt::WA_TranslucentBackground, true); // 设置窗口样式为透明

    cxScreen = GetSystemMetrics(SM_CXSCREEN);   // 获取屏幕宽度
    cyScreen = GetSystemMetrics(SM_CYSCREEN);   // 获取屏幕高度

    HWND hwnd = ::FindWindowExA(nullptr, nullptr, "Shell_TrayWnd", nullptr);
    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    taskbarHeight = rect.bottom - rect.top;

    AudioData = new short[32*2];

    SpecGraph = new Spectrum(this); // 实例化频谱显示 label
    this->SpecGraph->resize(cxScreen, taskbarHeight);   // 频谱显示窗口大小重设
    this->SpecGraph->move(0, 0);   // 频谱显示 label 位置移动

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(Slot_Time_Out()));
    this->timer->start(20);
}

BackgroundWidget::~BackgroundWidget()
{
    QString str = "Background widget destroied.";
    emit ConsoleDataReady(&str);
    delete timer;
    delete SpecGraph;
}

void BackgroundWidget::SetDisplayArea(const char area)
{
    if(area == 1)
    {
        this->setGeometry(0, cyScreen - taskbarHeight, cxScreen, taskbarHeight);  // 桌面背景模式
        SetBackgroundWMChild(this);
    }

    if(area == 2)
    {
        this->setGeometry(0, taskbarHeight - cyScreen, cxScreen, taskbarHeight);    // 任务栏模式
        SetTaskbarWMChild(this);
    }
    QString str = QString::asprintf("Screen size: %dx%d Taskbar height: %d ", cxScreen, cyScreen, taskbarHeight);
    emit ConsoleDataReady(&str);
}

void BackgroundWidget::Slot_Time_Out()
{
    int sampleCount = 32*2;
//    auto timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
//    unsigned int msTime = (timeNow.count() % 9999) + 1;
//    srand(msTime);
//    for(int i = 0; i<2*32; i++)
//        sampleData[i] = static_cast<short>(i * 10 * rand());

    this->SpecGraph->powerSpectrum(AudioData, sampleCount, 2, SpecGraph->FFT_result);
    this->SpecGraph->update();
}

void BackgroundWidget::SetAudioData(BYTE *data, const int interval)
{
//    auto timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
//    qDebug() << timeNow.count();
    for(int i=0; i<32*2; i+=2)
    {
//        AudioData[i] = static_cast<short>(data[(interval/32/2)*i+i*2] + data[(interval/32/2)*i+i*2+1]*256);
//        AudioData[i+1] = static_cast<short>(data[(interval/32/2)*i+(i*2)+2] + data[(interval/32/2)*i+(i*2)+3]*256);
        AudioData[i] = static_cast<short>(data[(interval/32/2)*i+i*2] + data[(interval/32/2)*i+i*2+1]*256);
        AudioData[i+1] = static_cast<short>(data[(interval/32/2)*i+(i*2)+2] + data[(interval/32/2)*i+(i*2)+3]*256);
        if(AudioData[i] == 1 || AudioData[i] == -1)
            AudioData[i] = 0;
        if(AudioData[i+1] == 1 || AudioData[i+1] == -1)
            AudioData[i+1] = 0;
//        cout << AudioData[i] << " " << AudioData[i+1] << " ";
    }
//    cout << endl << "-------------------------" << endl;
}

void BackgroundWidget::SetAmpGrade(double grade)
{
    this->SpecGraph->ampGrade = grade;
}


#ifdef _WIN32
void BackgroundWidget::SetBackgroundWMChild(QWidget* widget)
{
    HWND background = nullptr;
    HWND currentHWND = (HWND)widget->winId();
    HWND hwnd = ::FindWindowA("progman", "Program Manager");
    int retry = 0;
    while(1)
    {
        HWND worker = nullptr;
        do
        {
            worker = ::FindWindowExA(nullptr, worker, "WorkerW", nullptr);
            if(worker != nullptr)
            {
                char buff[200] = {0};

                int ret = GetClassName(worker, (WCHAR*)buff, sizeof(buff)*2);
                if(ret == 0)
                {
                    unsigned long err = GetLastError();
                    qDebug()<<"err:"<<err;
                }
                //QString className = QString::fromUtf16((char16_t*)buff);
            }
            if(GetParent(worker) == hwnd)
            {
                background = worker;
            }
        }
        while(worker != nullptr);

        if(background == nullptr)
        {
            SendMessageA(hwnd, 0x052C, 0, 0);
            qDebug()<<"send message to create WorkerW";
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
    SetParent(currentHWND, background);
}

void BackgroundWidget::SetTaskbarWMChild(QWidget* widget)
{
    HWND background = nullptr;
    HWND currentHWND = (HWND)widget->winId();
    background = ::FindWindowExA(nullptr, nullptr, "Shell_TrayWnd", nullptr);
    SetWindowPos(currentHWND, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    SetParent(currentHWND, background);
}

#endif
