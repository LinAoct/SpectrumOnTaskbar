#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    bool bSilent = false;
    for(int i=0; i<argc; i++)
    {
        if(strcmp("-s", argv[i])==0)
        {
            bSilent = true;
        }
    }
    if(!bSilent)
    {
        window.show();
    }
    else
    {
        window.showMinimized();
        window.hide();
    }

    return a.exec();
}
