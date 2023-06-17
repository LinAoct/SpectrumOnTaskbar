#ifndef AUDIORECORDTHREAD_H
#define AUDIORECORDTHREAD_H

#include <windows.h>
#include <mmdeviceapi.h>
#include <combaseapi.h>
#include <mmreg.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <setupapi.h>
#include <initguid.h>
#include <functiondiscoverykeys_devpkey.h>
#include <QDebug>
#include <QThread>
#include <QTimer>


class CMMNotificationClient : public IMMNotificationClient
{
public:
    IMMDeviceEnumerator *m_pEnumerator;
    bool bDefaultDeviceChanged = false; // 标志默认音频设备是否改变
    CMMNotificationClient():
    // _cRef(1),
    m_pEnumerator(nullptr)
    {
        // 初始化COM
        ::CoInitialize(nullptr);
        HRESULT hr = S_OK;

        // 创建接口
        hr = CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            reinterpret_cast<void **>(&m_pEnumerator));

        if (hr==S_OK)
        {
            qDebug()<<"接口创建成功";
        }
        else
        {
            qDebug()<<"接口创建失败";
        }

        // 注册事件
        hr = m_pEnumerator->RegisterEndpointNotificationCallback(reinterpret_cast<IMMNotificationClient*>(this));
        if (hr==S_OK)
        {
            qDebug()<<"注册成功";
        }
        else
        {
            qDebug()<<"注册失败";
        }
    }

    ~CMMNotificationClient()
    {
        if (m_pEnumerator != nullptr)
        {
            m_pEnumerator->Release();
            m_pEnumerator = nullptr;
        }
        ::CoUninitialize();
    }

private:
    LONG _cRef;

    // 打印设备友好名称
    HRESULT _PrintDeviceName(LPCWSTR pwstrId);

    ULONG STDMETHODCALLTYPE AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG ulRef = InterlockedDecrement(&_cRef);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid, VOID **ppvInterface)
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvInterface = (IUnknown*)this;
        }
        else if (__uuidof(IMMNotificationClient) == riid)
        {
            AddRef();
            *ppvInterface = (IMMNotificationClient*)this;
        }
        else
        {
            *ppvInterface = nullptr;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
        EDataFlow flow, ERole role,
        LPCWSTR pwstrDeviceId)
    {
        qDebug()<<"OnDefaultDeviceChanged";
        _PrintDeviceName(pwstrDeviceId);
        QString pszFlow, pszRole;
        switch (flow)
        {
            case eRender:
                pszFlow = "eRender";
                break;
            case eCapture:
                pszFlow = "eCapture";
                break;
            default:
                break;
        }
        switch (role)
        {
            case eConsole:
                pszRole = "eConsole";
                break;
            case eMultimedia:
                pszRole = "eMultimedia";
                break;
            case eCommunications:   // 变更默认通信设备
                pszRole = "eCommunications";
                break;
            default:
                break;
        }
        // 判断默认设备是否发生改变 是则改变标志位
        if (flow == eRender && role == eMultimedia)
        {
            this->bDefaultDeviceChanged = true;
        }
        qDebug() << pszFlow << " " << pszRole;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
    {
        qDebug()<<"OnDeviceAdded";
        _PrintDeviceName(pwstrDeviceId);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
    {
        qDebug()<<"OnDeviceRemoved";
        _PrintDeviceName(pwstrDeviceId);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
        LPCWSTR pwstrDeviceId,
        DWORD dwNewState)
    {
        qDebug()<<"OnDeviceStateChanged"<< dwNewState;
        _PrintDeviceName(pwstrDeviceId);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
        LPCWSTR pwstrDeviceId,
        const PROPERTYKEY key)
    {
        // qDebug()<<"OnPropertyValueChanged";
        // _PrintDeviceName(pwstrDeviceId);
        return S_OK;
    }
};


class AudioRecordThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioRecordThread(){}
    ~AudioRecordThread(){}

private:
    CMMNotificationClient myCMM;
    WAVEFORMATEX *pwfx = nullptr;
    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IMMNotificationClient *pNotificationClient = nullptr;
    IAudioClient *pAudioClient = nullptr;
    IAudioCaptureClient *pCaptureClient = nullptr;

    bool BFailed(HRESULT hres);

protected:
    // QThread的虚函数
    void run();

signals:
    void DataReady(BYTE *data, unsigned long numFrames, BOOL *bDone);  // 音频数据捕获成功信号
    void OnAudioSilent();                       // 系统静音信号
    void OnDeviceInited(const QString *str);    // 端点设备初始化信号

public slots:

};

#endif // AUDIORECORDTHREAD_H
