#include "audiorecordthread.h"

#define REFTIMES_PER_SEC  10000
#define REFTIMES_PER_MILLISEC  10

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


void AudioRecordThread::run()
{
    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IAudioClient *pAudioClient = nullptr;
    IAudioCaptureClient *pCaptureClient = nullptr;
    WAVEFORMATEX *pwfx = nullptr;
    UINT32 packetLength = 0;
    BOOL bDone = FALSE;
    BYTE *pData;
    DWORD flags;
    QString stra;
    QString str = "Audio Record Thread started.";
    emit ConsoleDataReady(&str);

    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    hr = CoCreateInstance(
           CLSID_MMDeviceEnumerator, nullptr,
           CLSCTX_ALL, IID_IMMDeviceEnumerator,
           reinterpret_cast<void **>(&pEnumerator));
    EXIT_ON_ERROR(hr)

    hr = pEnumerator->GetDefaultAudioEndpoint(
                        eRender, eMultimedia, &pDevice);
    EXIT_ON_ERROR(hr)

    hr = pDevice->Activate(
                    IID_IAudioClient, CLSCTX_ALL,
                    nullptr, reinterpret_cast<void **>(&pAudioClient));
    EXIT_ON_ERROR(hr)

    hr = pAudioClient->GetMixFormat(&pwfx);
    EXIT_ON_ERROR(hr)

//    pwfx->wFormatTag = WAVE_FORMAT_PCM;
//    pwfx->nBlockAlign = 8;
//    pwfx->wBitsPerSample = 16;
//    pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

    PWAVEFORMATEXTENSIBLE pEx;
    pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);

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
    EXIT_ON_ERROR(hr)
//    qDebug() << "wFormatTag:" << pwfx->wFormatTag << "通道数量:" << pwfx->nChannels
//             << "采样频率:" << pwfx->nSamplesPerSec << "请求的平均数据传输率:" << pwfx->nAvgBytesPerSec
//             << "nBlockAlign:" << pwfx->nBlockAlign << "样本的位深:" << pwfx->wBitsPerSample;

    stra = QString::asprintf("wFormatTag:%d 通道数量:%d 采样频率:%d 请求的平均数据传输率:%d nBlockAlign:%d 样本位深:%d",
                            pwfx->wFormatTag, pwfx->nChannels, pwfx->nSamplesPerSec,pwfx->nAvgBytesPerSec,
                            pwfx->nBlockAlign, pwfx->wBitsPerSample);
    emit ConsoleDataReady(&stra);

    // Get the size of the allocated buffer.
    hr = pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hr)

    hr = pAudioClient->GetService(
                         IID_IAudioCaptureClient,
                         reinterpret_cast<void **>(&pCaptureClient));
    EXIT_ON_ERROR(hr)

    // Notify the audio sink which format to use.
//    hr = pMySink->SetFormat(pwfx);
    EXIT_ON_ERROR(hr)

    // Calculate the actual duration of the allocated buffer.
    hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

    hr = pAudioClient->Start();  // Start recording.
    EXIT_ON_ERROR(hr)

    // Each loop fills about half of the shared buffer.
    while (bDone == FALSE)
    {
        // Sleep for half the buffer duration.
        Sleep(hnsActualDuration/REFTIMES_PER_MILLISEC/2);
//        msleep(10);

        hr = pCaptureClient->GetNextPacketSize(&packetLength);
        EXIT_ON_ERROR(hr)

        while (packetLength != 0)
        {
            // Get the available data in the shared buffer.
            hr = pCaptureClient->GetBuffer(
                                   &pData,
                                   &numFramesAvailable,
                                   &flags, nullptr, nullptr);
            EXIT_ON_ERROR(hr)

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
            {
                pData = nullptr;  // Tell CopyData to write silence.
            }

            // Copy the available capture data to the audio sink.
            emit DataReady(pData, numFramesAvailable, &bDone);
            EXIT_ON_ERROR(hr)
//            qDebug() << "numFrame:" << numFramesAvailable << "hnsAD:" << hnsActualDuration << "packetLength:" << packetLength;

            hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
            EXIT_ON_ERROR(hr)

            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hr)
        }
    }

    hr = pAudioClient->Stop();  // Stop recording.
    EXIT_ON_ERROR(hr)

Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pCaptureClient)
    qDebug() << "Audio Record Thread quit.";
};


MyAudioSink::MyAudioSink() : QObject ()
{
//    QTimer *timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(Slot_OnTimeOut()));
//    timer->start(hnsActualDuration/REFTIMES_PER_MILLISEC/2);
}

bool MyAudioSink::CopyData(BYTE *data, unsigned long numFrames, BOOL *bDone)
{
    if(data)
        qDebug() << data[0] << numFrames << *bDone;
    return true;
}

void MyAudioSink::Slot_OnTimeOut()
{

}


HRESULT RecordAudioStream(MyAudioSink *pMySink)
{
    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IAudioClient *pAudioClient = nullptr;
    IAudioCaptureClient *pCaptureClient = nullptr;
    WAVEFORMATEX *pwfx = nullptr;
    UINT32 packetLength = 0;
    BOOL bDone = FALSE;
    BYTE *pData;
    DWORD flags;

    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    hr = CoCreateInstance(
           CLSID_MMDeviceEnumerator, nullptr,
           CLSCTX_ALL, IID_IMMDeviceEnumerator,
           (void**)&pEnumerator);
    EXIT_ON_ERROR(hr)

    hr = pEnumerator->GetDefaultAudioEndpoint(
                        eRender, eMultimedia, &pDevice);
    EXIT_ON_ERROR(hr)

    hr = pDevice->Activate(
                    IID_IAudioClient, CLSCTX_ALL,
                    nullptr, (void**)&pAudioClient);
    EXIT_ON_ERROR(hr)

    hr = pAudioClient->GetMixFormat(&pwfx);
    EXIT_ON_ERROR(hr)

    hr = pAudioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         AUDCLNT_STREAMFLAGS_LOOPBACK,
                         hnsRequestedDuration,
                         0,
                         pwfx,
                         nullptr);
    EXIT_ON_ERROR(hr)

    // Get the size of the allocated buffer.
    hr = pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hr)

    hr = pAudioClient->GetService(
                         IID_IAudioCaptureClient,
                         (void**)&pCaptureClient);
    EXIT_ON_ERROR(hr)

    // Notify the audio sink which format to use.
//    hr = pMySink->SetFormat(pwfx);
    EXIT_ON_ERROR(hr)

    // Calculate the actual duration of the allocated buffer.
    hnsActualDuration = (double)REFTIMES_PER_SEC *
                     bufferFrameCount / pwfx->nSamplesPerSec;

    hr = pAudioClient->Start();  // Start recording.
    EXIT_ON_ERROR(hr)

    // Each loop fills about half of the shared buffer.
    while (bDone == FALSE)
    {
        // Sleep for half the buffer duration.
        Sleep(hnsActualDuration/REFTIMES_PER_MILLISEC/2);

        hr = pCaptureClient->GetNextPacketSize(&packetLength);
        EXIT_ON_ERROR(hr)

        while (packetLength != 0)
        {
            // Get the available data in the shared buffer.
            hr = pCaptureClient->GetBuffer(
                                   &pData,
                                   &numFramesAvailable,
                                   &flags, nullptr, nullptr);
            EXIT_ON_ERROR(hr)

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
            {
                pData = nullptr;  // Tell CopyData to write silence.
            }

            // Copy the available capture data to the audio sink.
            hr = pMySink->CopyData(pData, numFramesAvailable, &bDone);
            EXIT_ON_ERROR(hr)

            hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
            EXIT_ON_ERROR(hr)

            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hr)
        }
    }

    hr = pAudioClient->Stop();  // Stop recording.
    EXIT_ON_ERROR(hr)

Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pCaptureClient)

    return hr;
}
