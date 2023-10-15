#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

#define TRANSLATOR_PATH     ":/assets/qt_zh_CN.qm"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    bool bSilent = false;
    for(int i=0; i<argc; i++)
    {
        if (strcmp("-s", argv[i])==0)
        {
            bSilent = true;
        }
    }
    if (!bSilent)
    {
        window.show();
    }
    else
    {
        window.showMinimized();
        window.hide();
    }

    // QT 控件翻译设置
    QTranslator SysTranslator;
    SysTranslator.load(QString(TRANSLATOR_PATH));
    a.installTranslator(&SysTranslator);

    return a.exec();
}
