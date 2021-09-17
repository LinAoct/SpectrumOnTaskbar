#include "audiodevicelist.h"


AudioDeviceList::AudioDeviceList(QComboBox *inputComboBox, QComboBox *outputComboBox)
{
    // 获取输入音频设备名称
    QVector<QString> aDeviceListI;
    QList<QAudioDeviceInfo> audioDeviceListI = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListI)
    {
        QString strName = devInfo.deviceName();
        if (devInfo.isNull()) continue;
        if (strName[0] == 65533) continue;
        bool bFound = false;
        foreach (QString dev, aDeviceListI)
        {
            if (strName == dev)
            {
                bFound = true;
            }
        }
        if (bFound == true) continue;
        aDeviceListI.push_back(strName);
        inputComboBox->addItem(strName);
    }

    //获取输出音频设备名称
    QVector<QString> aDeviceListO;
    QList<QAudioDeviceInfo> audioDeviceListO = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListO)
    {
        QString strName = devInfo.deviceName();
        if (devInfo.isNull()) continue;
        if (strName[0] == 65533) continue;
        bool bFound = false;
        foreach (QString dev, aDeviceListO)
        {
            if (strName == dev)
            {
                bFound = true;
            }
        }
        if (bFound == true)
            continue;
        aDeviceListO.push_back(strName);
        outputComboBox->addItem(strName);
    }
}

