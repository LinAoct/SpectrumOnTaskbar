#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QDebug>
#include "spectrum.h"


#define CONFIG_SPECTRUM_DISPLAY_STYLE_DEFAULT   Spectrum::SmoothWaveStyle

#define CONFIG_SPECTRUM_AMP_VALUE_DEFAULT       3

#define CONFIG_FILE_NAME                        "config.ini"
#define CONFIG_SPECTRUM_DISPLAY_STYLE_NAME      "Spectrum/DisplayStyle"
#define CONFIG_SPECTRUM_AMP_VALUE_NAME          "Spectrum/Amp"

typedef struct _Config_Param
{
    int min;
    int max;
    int defaultValue;
    int currentValue;
} Config_Param_t;


class Config
{
public:
    Config();
    static void read_config();
    static void write_config();
    static int config_read_display_style();
    static void config_save_display_style(int value);
    static int config_read_amp_value();
    static void config_save_amp_value(int value);
};

#endif // CONFIG_H
