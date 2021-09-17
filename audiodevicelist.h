#ifndef AUDIODEVICELIST_H
#define AUDIODEVICELIST_H

#include <QList>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QComboBox>


class AudioDeviceList
{
public:
    AudioDeviceList(QComboBox *inputComboBox, QComboBox *outputComboBox);
};

#endif // AUDIODEVICELIST_H
