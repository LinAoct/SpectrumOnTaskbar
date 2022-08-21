#ifndef COLORSELECTWIDGET_H
#define COLORSELECTWIDGET_H

#include <QWidget>


/**
  * @brief 渐变颜色结构体
  */
typedef struct _GradientColorStruct
{
    short side;                         // 颜色点位置
    QColor color;                       // 颜色值
    struct _GradientColorStruct *next;  // 下一颜色结构体
}GradientColorStruct;


class ColorSelectWidget : public QWidget
{
    Q_OBJECT

public:
    ColorSelectWidget(QWidget *parent = nullptr);
};

#endif // COLORSELECTWIDGET_H
