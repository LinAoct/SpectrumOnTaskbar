#ifndef AUDIORECORDTHREAD_H
#define AUDIORECORDTHREAD_H

#include <windows.h>
#include <mmdeviceapi.h>
#include <combaseapi.h>
#include <mmreg.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <QDebug>
#include <QThread>
#include <QTimer>


class MyAudioSink : public QObject
{
    Q_OBJECT
public:
    MyAudioSink();
    bool CopyData(BYTE *data, unsigned long numFrames, BOOL *bDone);

private slots:
    void Slot_OnTimeOut();
};

HRESULT RecordAudioStream(MyAudioSink *pMySink);

class AudioRecordThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioRecordThread(){}
    ~AudioRecordThread(){}

private:


protected:
    // QThread的虚函数
    void run();

signals:
    void DataReady(BYTE *data, unsigned long numFrames, BOOL *bDone);  // 处理完成信号
    void ConsoleDataReady(const QString *str);

public slots:
};

#endif // AUDIORECORDTHREAD_H
