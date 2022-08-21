#include "config.h"

Config::Config()
{

}

// 写ini配置文件
void Config::write_config()
{
    QSettings * myini = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);
    myini->setValue("Spectrum/DisplayStyle", 1);
    delete myini;
}

// 读ini配置文件
void Config::read_config()
{
    QSettings * myini = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);
    int displayStyle = myini->value("Spectrum/DisplayStyle").toInt();
    delete myini;

    qDebug() << "DisplayStyle" << displayStyle ;
}

/**
 * @brief 读 DisplayStyle 配置
 * @return <int> DisplayStyle 配置
 */
int Config::config_read_display_style()
{
    QSettings * myini = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);
    int displayStyle = myini->value(CONFIG_SPECTRUM_DISPLAY_STYLE_NAME).toInt();
    delete myini;

    if(displayStyle > DisplayStyleNameList.count())
    {
        displayStyle = CONFIG_SPECTRUM_DISPLAY_STYLE_DEFAULT;
        config_save_display_style(displayStyle);
    }

    return displayStyle;
}

/**
 * @brief 写 DisplayStyle 配置
 * @param None
 */
void Config::config_save_display_style(int value)
{
    QSettings * myini = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);
    myini->setValue(CONFIG_SPECTRUM_DISPLAY_STYLE_NAME, value);
    delete myini;
}

/**
 * @brief 读 幅度大小 配置
 * @return <int> 幅度大小值 配置
 */
int Config::config_read_amp_value()
{
    QSettings * myini = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);
    int ampValue = myini->value(CONFIG_SPECTRUM_AMP_VALUE_NAME).toInt();
    delete myini;

    if(ampValue > 40)
    {
        ampValue = CONFIG_SPECTRUM_AMP_VALUE_DEFAULT;
        config_save_amp_value(ampValue);
    }

    return ampValue;
}

/**
 * @brief 写 DisplayStyle 配置
 * @param None
 */
void Config::config_save_amp_value(int value)
{
    QSettings * myini = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);
    myini->setValue(CONFIG_SPECTRUM_AMP_VALUE_NAME, value);
    delete myini;
}
