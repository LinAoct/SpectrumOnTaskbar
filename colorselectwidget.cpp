#include "colorselectwidget.h"

ColorSelectWidget::ColorSelectWidget(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(0, 0, 100, 600);
    this->setStyleSheet("background-color: rgb(85, 255, 255);");

}
