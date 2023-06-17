#include "audiorecordthread.h"

#define REFTIMES_PER_SEC  0
#define REFTIMES_PER_MILLISEC  0
//400000 400
#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != nullptr)  \
                { (punk)->Release(); (punk) = nullptr; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);


/**
 * @brief 打印音频端点设备设备名称
 * @param pwstrId   端点设备ID
 * @return
 */
HRESULT CMMNotificationClient::_PrintDeviceName(LPCWSTR pwstrId)
{
    HRESULT hr = S_OK;
    IMMDevice *pDevice = nullptr;
    IPropertyStore *pProps = nullptr;
    PROPVARIANT varString;

    CoInitialize(nullptr);
    PropVariantInit(&varString);

    if (m_pEnumerator == nullptr)
    {
        // 获取音频端点设备的枚举器
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                              nullptr, CLSCTX_INPROC_SERVER,
                              __uuidof(IMMDeviceEnumerator),
                              reinterpret_cast<void **>(&m_pEnumerator));
    }
    if (hr == S_OK)
    {
        hr = m_pEnumerator->GetDevice(pwstrId, &pDevice);
    }
    if (hr == S_OK)
    {
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
    }
    if (hr == S_OK)
    {
        // 获取端点设备的友好名称属性
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varString);
    }
    qDebug() << QString::fromWCharArray(varString.pwszVal) << " " << QString::fromWCharArray(pwstrId);

    PropVariantClear(&varString);

    // 释放设备
    SAFE_RELEASE(pProps);
    SAFE_RELEASE(pDevice);
    return hr;
}

/**
 * @brief 音频捕获线程运行函数
 */
void AudioRecordThread::run()
{
    HRESULT hr;
    HANDLE hTimerWakeUp = nullptr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    // REFERENCE_TIME hnsActualDuration;
    REFERENCE_TIME hnsDefaultDevicePeriod(0);
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    PWAVEFORMATEXTENSIBLE pEx;
    UINT32 packetLength = 0;
    BOOL bDone = FALSE;
    BYTE *pData;
    DWORD flags;

    qDebug() << "Audio record thread started.";

    while(1)
    {
        try
        {
            hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            if(BFailed(hr)) continue;

            hr = CoCreateInstance(
                   CLSID_MMDeviceEnumerator, nullptr,
                   CLSCTX_ALL, IID_IMMDeviceEnumerator,
                   reinterpret_cast<void **>(&pEnumerator));
            if(BFailed(hr)) continue;

            // 检索指定数据流方向和角色的默认音频端点
            hr = pEnumerator->GetDefaultAudioEndpoint(
                                eRender, eMultimedia, &pDevice);
            if(BFailed(hr)) continue;

            hr = pDevice->Activate(
                            IID_IAudioClient, CLSCTX_ALL,
                            nullptr, reinterpret_cast<void **>(&pAudioClient));
            if(BFailed(hr)) continue;

            hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, nullptr);

            // 获取音频设备默认音频格式
            hr = pAudioClient->GetMixFormat(&pwfx);
            if(BFailed(hr)) continue;

            pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
            pEx->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
            pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
            pEx->Samples.wValidBitsPerSample = 16;
            pwfx->wBitsPerSample = 16;
            pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
            pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

            hr = pAudioClient->Initialize(
                                 AUDCLNT_SHAREMODE_SHARED,
                                 AUDCLNT_STREAMFLAGS_LOOPBACK,
                                 hnsRequestedDuration,
                                 0,
                                 pwfx,
                                 nullptr);
            if(BFailed(hr)) continue;

            // Get the size of the allocated buffer.
            hr = pAudioClient->GetBufferSize(&bufferFrameCount);
            if(BFailed(hr)) continue;

            hr = pAudioClient->GetService(
                                 IID_IAudioCaptureClient,
                                 reinterpret_cast<void **>(&pCaptureClient));
            if(BFailed(hr)) continue;

            hTimerWakeUp = CreateWaitableTimer(nullptr, FALSE, nullptr);
            if(hTimerWakeUp == nullptr) continue;
            LARGE_INTEGER liFirstFire;
            liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
            LONG lTimeBetweenFires = static_cast<LONG>(hnsDefaultDevicePeriod / 2 / (10 * 1000)); // convert to milliseconds
            BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, nullptr, nullptr, FALSE);
            if(!bOK) continue;

            HANDLE waitArray[1] = { hTimerWakeUp };
            DWORD dwWaitResult;

            // Calculate the actual duration of the allocated buffer.
            // hnsActualDuration = static_cast<long long>(REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec);

            qDebug("wFormatTag:%d 通道数:%d 采样频率:%d 每秒数据字节数:%d 样本所需字节数:%d 样本位深:%d 更新周期:%d",
                                    pwfx->wFormatTag, pwfx->nChannels, pwfx->nSamplesPerSec, pwfx->nAvgBytesPerSec,
                                    pwfx->nBlockAlign, pwfx->wBitsPerSample, lTimeBetweenFires);

            hr = pAudioClient->Start();  // 开始录制音频数据
            if(BFailed(hr)) continue;

            while (bDone == FALSE)
            {
                // 等待数据准备好
                 dwWaitResult = WaitForMultipleObjects(sizeof(waitArray)/sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
                 if (WAIT_OBJECT_0 != dwWaitResult) break;

                // 判断默认音频设备是否发生改变
                if (myCMM.bDefaultDeviceChanged)
                {
                    myCMM.bDefaultDeviceChanged = false;
                    break;
                }

                hr = pCaptureClient->GetNextPacketSize(&packetLength);
                if(FAILED(hr)) break;

                while (packetLength != 0)
                {
                    // 从共享缓冲区中获取可用音频数据
                    hr = pCaptureClient->GetBuffer(&pData,
                                                   &numFramesAvailable,
                                                   &flags,
                                                   nullptr,
                                                   nullptr);
                    if(FAILED(hr)) break;

                    if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
                    {
                        pData = nullptr;  // Tell CopyData to write silence.
                    }

                    // 传递捕获的可用音频信号
                    emit DataReady(pData, numFramesAvailable, &bDone);

                    // 释放缓冲区
                    hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
                    if(FAILED(hr)) break;

                    // 获取下一个数据包大小
                    hr = pCaptureClient->GetNextPacketSize(&packetLength);
                    if(FAILED(hr)) break;
                }
            }
            hr = pAudioClient->Stop();  // 停止捕获音频数据.
            if(BFailed(hr)) continue;
        }
        catch(int error)
        {
            qDebug()<<"Audio recorder thread error: "<<error;
        }
        sleep(1);
    }
}

bool AudioRecordThread::BFailed(HRESULT hres)
{
    if(FAILED(hres))
    {
        CoTaskMemFree(pwfx);
        SAFE_RELEASE(pEnumerator);
        SAFE_RELEASE(pDevice);
        SAFE_RELEASE(pAudioClient);
        SAFE_RELEASE(pCaptureClient);
        return 1;
    }
    return 0;
}


//hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//if(FAILED(hr)) return;

//hr = CoCreateInstance(
//       CLSID_MMDeviceEnumerator, nullptr,
//       CLSCTX_ALL, IID_IMMDeviceEnumerator,
//       reinterpret_cast<void **>(&pEnumerator));
//if(FAILED(hr)) return;

//hr = pEnumerator->GetDefaultAudioEndpoint(
//                    eRender, eMultimedia, &pDevice);
//if(FAILED(hr)) return;

//hr = pDevice->Activate(
//                IID_IAudioClient, CLSCTX_ALL,
//                nullptr, reinterpret_cast<void **>(&pAudioClient));
//if(FAILED(hr)) return;

//hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, nullptr);

//hr = pAudioClient->GetMixFormat(&pwfx);
//if(FAILED(hr)) return;

//pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);

//pEx->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
//pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
//pEx->Samples.wValidBitsPerSample = 16;
//pwfx->wBitsPerSample = 16;
////    pwfx->nSamplesPerSec = 44800;
//pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
//pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

//hr = pAudioClient->Initialize(
//                     AUDCLNT_SHAREMODE_SHARED,
//                     AUDCLNT_STREAMFLAGS_LOOPBACK,
//                     hnsRequestedDuration,
//                     0,
//                     pwfx,
//                     nullptr);
//if(FAILED(hr)) return;

//// Get the size of the allocated buffer.
//hr = pAudioClient->GetBufferSize(&bufferFrameCount);
//if(FAILED(hr)) return;

//hr = pAudioClient->GetService(
//                     IID_IAudioCaptureClient,
//                     reinterpret_cast<void **>(&pCaptureClient));
//if(FAILED(hr)) return;

//hTimerWakeUp = CreateWaitableTimer(nullptr, FALSE, nullptr);
//if(hTimerWakeUp == nullptr) return;
//LARGE_INTEGER liFirstFire;
//liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
//LONG lTimeBetweenFires = static_cast<LONG>(hnsDefaultDevicePeriod / 2 / (10 * 1000)); // convert to milliseconds
//BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, nullptr, nullptr, FALSE);
//if(!bOK) return;

//HANDLE waitArray[1] = { hTimerWakeUp };
//DWORD dwWaitResult;

//// Calculate the actual duration of the allocated buffer.
//hnsActualDuration = static_cast<long long>(REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec);

////    qDebug() << "wFormatTag:" << pwfx->wFormatTag << "通道数量:" << pwfx->nChannels
////             << "采样频率:" << pwfx->nSamplesPerSec << "请求的平均数据传输率:" << pwfx->nAvgBytesPerSec
////             << "nBlockAlign:" << pwfx->nBlockAlign << "样本的位深:" << pwfx->wBitsPerSample;

//stra = QString::asprintf("wFormatTag:%d 通道数:%d 采样频率:%d 每秒数据字节数:%d 样本所需字节数:%d 样本位深:%d 更新周期:%d",
//                        pwfx->wFormatTag, pwfx->nChannels, pwfx->nSamplesPerSec, pwfx->nAvgBytesPerSec,
//                        pwfx->nBlockAlign, pwfx->wBitsPerSample, lTimeBetweenFires);
//emit ConsoleDataReady(&stra);

//hr = pAudioClient->Start();  // Start recording.
//if (FAILED(hr)) return;

//// Each loop fills about half of the shared buffer.
//while (bDone == FALSE)
//{
//    dwWaitResult = WaitForMultipleObjects(sizeof(waitArray)/sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
//    if(WAIT_OBJECT_0 != dwWaitResult) break;

//    hr = pCaptureClient->GetNextPacketSize(&packetLength);
//    if(FAILED(hr)) break;

//    while (packetLength != 0)
//    {
//        // Get the available data in the shared buffer.
//        hr = pCaptureClient->GetBuffer(
//                               &pData,
//                               &numFramesAvailable,
//                               &flags, nullptr, nullptr);
//        if(FAILED(hr)) break;

//        if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
//        {
//            pData = nullptr;  // Tell CopyData to write silence.
//        }

//        // Copy the available capture data to the audio sink.
//        emit DataReady(pData, numFramesAvailable, &bDone);

//        hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
//        if(FAILED(hr)) break;

//        hr = pCaptureClient->GetNextPacketSize(&packetLength);
//        if(FAILED(hr)) break;
//    }
//}

//hr = pAudioClient->Stop();  // Stop recording.
//EXIT_ON_ERROR(hr)

//Exit:
//CoTaskMemFree(pwfx);
//SAFE_RELEASE(pEnumerator)
//SAFE_RELEASE(pDevice)
//SAFE_RELEASE(pAudioClient)
//SAFE_RELEASE(pCaptureClient)
//qDebug() << "Audio recorder thread exited.";
