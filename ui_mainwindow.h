/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionquit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget_main;
    QWidget *tab_custom;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *radioButton_modeOnBackground;
    QRadioButton *radioButton_modeOnTaskbar;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_show;
    QPushButton *btn_close;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *checkBox_BAutoBoot;
    QSpacerItem *verticalSpacer;
    QWidget *tab_style;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QComboBox *comboBox_StyleSeletec;
    QFrame *line;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QComboBox *comboBox_OpacityValue;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *comboBox_UpdateSpeed;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_7;
    QRadioButton *radioButton_TextureStyle_Soild;
    QPushButton *btn_PureColorSelect;
    QFrame *line_4;
    QVBoxLayout *verticalLayout_8;
    QRadioButton *radioButton_TextureStyle_Gradient;
    QVBoxLayout *verticalLayout_ColorSelector;
    QVBoxLayout *verticalLayout_10;
    QRadioButton *radioButton_TextureStyle_StableRGB;
    QVBoxLayout *verticalLayout_11;
    QRadioButton *radioButton_TextureStyle_SlideRGB;
    QFrame *line_5;
    QVBoxLayout *verticalLayout_9;
    QRadioButton *radioButton_TextureStyle_Pattern;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QSlider *slider_AmpValue;
    QLabel *label_3;
    QFrame *line_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QSlider *slider_BlurValue;
    QLabel *label_11;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(740, 294);
        actionquit = new QAction(MainWindow);
        actionquit->setObjectName(QString::fromUtf8("actionquit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 10, 6, 10);
        tabWidget_main = new QTabWidget(centralWidget);
        tabWidget_main->setObjectName(QString::fromUtf8("tabWidget_main"));
        tabWidget_main->setLayoutDirection(Qt::LeftToRight);
        tabWidget_main->setAutoFillBackground(false);
        tabWidget_main->setTabPosition(QTabWidget::North);
        tabWidget_main->setTabShape(QTabWidget::Rounded);
        tabWidget_main->setElideMode(Qt::ElideNone);
        tabWidget_main->setUsesScrollButtons(true);
        tabWidget_main->setTabsClosable(false);
        tabWidget_main->setMovable(false);
        tabWidget_main->setTabBarAutoHide(false);
        tab_custom = new QWidget();
        tab_custom->setObjectName(QString::fromUtf8("tab_custom"));
        verticalLayout_6 = new QVBoxLayout(tab_custom);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        radioButton_modeOnBackground = new QRadioButton(tab_custom);
        radioButton_modeOnBackground->setObjectName(QString::fromUtf8("radioButton_modeOnBackground"));
        radioButton_modeOnBackground->setChecked(true);

        horizontalLayout_5->addWidget(radioButton_modeOnBackground);

        radioButton_modeOnTaskbar = new QRadioButton(tab_custom);
        radioButton_modeOnTaskbar->setObjectName(QString::fromUtf8("radioButton_modeOnTaskbar"));
        radioButton_modeOnTaskbar->setChecked(false);

        horizontalLayout_5->addWidget(radioButton_modeOnTaskbar);


        verticalLayout_6->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_show = new QPushButton(tab_custom);
        btn_show->setObjectName(QString::fromUtf8("btn_show"));

        horizontalLayout->addWidget(btn_show);

        btn_close = new QPushButton(tab_custom);
        btn_close->setObjectName(QString::fromUtf8("btn_close"));

        horizontalLayout->addWidget(btn_close);


        verticalLayout_6->addLayout(horizontalLayout);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(6, 6, 6, 6);
        checkBox_BAutoBoot = new QCheckBox(tab_custom);
        checkBox_BAutoBoot->setObjectName(QString::fromUtf8("checkBox_BAutoBoot"));

        horizontalLayout_10->addWidget(checkBox_BAutoBoot);


        verticalLayout_6->addLayout(horizontalLayout_10);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);

        tabWidget_main->addTab(tab_custom, QString());
        tab_style = new QWidget();
        tab_style->setObjectName(QString::fromUtf8("tab_style"));
        verticalLayout_5 = new QVBoxLayout(tab_style);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(6, 6, 6, 6);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(6, 6, 6, 6);
        label_8 = new QLabel(tab_style);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        comboBox_StyleSeletec = new QComboBox(tab_style);
        comboBox_StyleSeletec->setObjectName(QString::fromUtf8("comboBox_StyleSeletec"));
        comboBox_StyleSeletec->setMinimumSize(QSize(0, 21));

        horizontalLayout_8->addWidget(comboBox_StyleSeletec);


        horizontalLayout_4->addLayout(horizontalLayout_8);

        line = new QFrame(tab_style);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(6, 6, 6, 6);
        label_7 = new QLabel(tab_style);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        comboBox_OpacityValue = new QComboBox(tab_style);
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->addItem(QString());
        comboBox_OpacityValue->setObjectName(QString::fromUtf8("comboBox_OpacityValue"));
        comboBox_OpacityValue->setMinimumSize(QSize(0, 21));
        comboBox_OpacityValue->setEditable(false);
        comboBox_OpacityValue->setMaxCount(10);

        horizontalLayout_7->addWidget(comboBox_OpacityValue);


        horizontalLayout_4->addLayout(horizontalLayout_7);

        line_2 = new QFrame(tab_style);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(6, 6, 6, 6);
        label_4 = new QLabel(tab_style);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        comboBox_UpdateSpeed = new QComboBox(tab_style);
        comboBox_UpdateSpeed->addItem(QString());
        comboBox_UpdateSpeed->addItem(QString());
        comboBox_UpdateSpeed->addItem(QString());
        comboBox_UpdateSpeed->addItem(QString());
        comboBox_UpdateSpeed->addItem(QString());
        comboBox_UpdateSpeed->setObjectName(QString::fromUtf8("comboBox_UpdateSpeed"));
        comboBox_UpdateSpeed->setMinimumSize(QSize(0, 21));

        horizontalLayout_3->addWidget(comboBox_UpdateSpeed);


        horizontalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout_4);

        groupBox = new QGroupBox(tab_style);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_12 = new QHBoxLayout(groupBox);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(6, 6, 6, 6);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        radioButton_TextureStyle_Soild = new QRadioButton(groupBox);
        radioButton_TextureStyle_Soild->setObjectName(QString::fromUtf8("radioButton_TextureStyle_Soild"));

        verticalLayout_7->addWidget(radioButton_TextureStyle_Soild);

        btn_PureColorSelect = new QPushButton(groupBox);
        btn_PureColorSelect->setObjectName(QString::fromUtf8("btn_PureColorSelect"));

        verticalLayout_7->addWidget(btn_PureColorSelect);


        horizontalLayout_12->addLayout(verticalLayout_7);

        line_4 = new QFrame(groupBox);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        horizontalLayout_12->addWidget(line_4);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        radioButton_TextureStyle_Gradient = new QRadioButton(groupBox);
        radioButton_TextureStyle_Gradient->setObjectName(QString::fromUtf8("radioButton_TextureStyle_Gradient"));

        verticalLayout_8->addWidget(radioButton_TextureStyle_Gradient);

        verticalLayout_ColorSelector = new QVBoxLayout();
        verticalLayout_ColorSelector->setSpacing(6);
        verticalLayout_ColorSelector->setObjectName(QString::fromUtf8("verticalLayout_ColorSelector"));

        verticalLayout_8->addLayout(verticalLayout_ColorSelector);


        horizontalLayout_12->addLayout(verticalLayout_8);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        radioButton_TextureStyle_StableRGB = new QRadioButton(groupBox);
        radioButton_TextureStyle_StableRGB->setObjectName(QString::fromUtf8("radioButton_TextureStyle_StableRGB"));

        verticalLayout_10->addWidget(radioButton_TextureStyle_StableRGB);


        horizontalLayout_12->addLayout(verticalLayout_10);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        radioButton_TextureStyle_SlideRGB = new QRadioButton(groupBox);
        radioButton_TextureStyle_SlideRGB->setObjectName(QString::fromUtf8("radioButton_TextureStyle_SlideRGB"));

        verticalLayout_11->addWidget(radioButton_TextureStyle_SlideRGB);


        horizontalLayout_12->addLayout(verticalLayout_11);

        line_5 = new QFrame(groupBox);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);

        horizontalLayout_12->addWidget(line_5);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        radioButton_TextureStyle_Pattern = new QRadioButton(groupBox);
        radioButton_TextureStyle_Pattern->setObjectName(QString::fromUtf8("radioButton_TextureStyle_Pattern"));

        verticalLayout_9->addWidget(radioButton_TextureStyle_Pattern);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_9->addWidget(label_5);


        horizontalLayout_12->addLayout(verticalLayout_9);


        verticalLayout_5->addWidget(groupBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(6, 6, 6, 6);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(tab_style);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(tab_style);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        slider_AmpValue = new QSlider(tab_style);
        slider_AmpValue->setObjectName(QString::fromUtf8("slider_AmpValue"));
        slider_AmpValue->setMinimum(0);
        slider_AmpValue->setMaximum(40);
        slider_AmpValue->setValue(3);
        slider_AmpValue->setSliderPosition(3);
        slider_AmpValue->setOrientation(Qt::Horizontal);
        slider_AmpValue->setTickPosition(QSlider::TicksBelow);
        slider_AmpValue->setTickInterval(0);

        horizontalLayout_6->addWidget(slider_AmpValue);

        label_3 = new QLabel(tab_style);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_6->addWidget(label_3);


        verticalLayout_3->addLayout(horizontalLayout_6);


        horizontalLayout_2->addLayout(verticalLayout_3);

        line_3 = new QFrame(tab_style);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(tab_style);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_4->addWidget(label_9);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(tab_style);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_9->addWidget(label_10);

        slider_BlurValue = new QSlider(tab_style);
        slider_BlurValue->setObjectName(QString::fromUtf8("slider_BlurValue"));
        slider_BlurValue->setMaximum(20);
        slider_BlurValue->setValue(4);
        slider_BlurValue->setOrientation(Qt::Horizontal);
        slider_BlurValue->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_9->addWidget(slider_BlurValue);

        label_11 = new QLabel(tab_style);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_9->addWidget(label_11);


        verticalLayout_4->addLayout(horizontalLayout_9);


        horizontalLayout_2->addLayout(verticalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_2);

        tabWidget_main->addTab(tab_style, QString());

        verticalLayout->addWidget(tabWidget_main);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget_main->setCurrentIndex(1);
        comboBox_StyleSeletec->setCurrentIndex(-1);
        comboBox_OpacityValue->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionquit->setText(QApplication::translate("MainWindow", "quit", nullptr));
        radioButton_modeOnBackground->setText(QApplication::translate("MainWindow", "\351\233\206\346\210\220\345\210\260\346\241\214\351\235\242\350\203\214\346\231\257(\351\234\200\350\246\201\344\273\273\345\212\241\346\240\217\351\200\217\346\230\216)", nullptr));
        radioButton_modeOnTaskbar->setText(QApplication::translate("MainWindow", "\351\233\206\346\210\220\345\210\260\344\273\273\345\212\241\346\240\217\346\250\241\345\274\217\357\274\210Debuging\357\274\211", nullptr));
        btn_show->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\351\242\221\350\260\261", nullptr));
        btn_close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\351\242\221\350\260\261\346\230\276\347\244\272", nullptr));
        checkBox_BAutoBoot->setText(QApplication::translate("MainWindow", "\345\274\200\346\234\272\350\207\252\345\220\257", nullptr));
        tabWidget_main->setTabText(tabWidget_main->indexOf(tab_custom), QApplication::translate("MainWindow", "\345\270\270\350\247\204", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "\351\242\221\350\260\261\346\240\267\345\274\217", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "\351\200\217\346\230\216\345\272\246", nullptr));
        comboBox_OpacityValue->setItemText(0, QApplication::translate("MainWindow", "1", nullptr));
        comboBox_OpacityValue->setItemText(1, QApplication::translate("MainWindow", "0.9", nullptr));
        comboBox_OpacityValue->setItemText(2, QApplication::translate("MainWindow", "0.8", nullptr));
        comboBox_OpacityValue->setItemText(3, QApplication::translate("MainWindow", "0.7", nullptr));
        comboBox_OpacityValue->setItemText(4, QApplication::translate("MainWindow", "0.6", nullptr));
        comboBox_OpacityValue->setItemText(5, QApplication::translate("MainWindow", "0.5", nullptr));
        comboBox_OpacityValue->setItemText(6, QApplication::translate("MainWindow", "0.4", nullptr));
        comboBox_OpacityValue->setItemText(7, QApplication::translate("MainWindow", "0.3", nullptr));
        comboBox_OpacityValue->setItemText(8, QApplication::translate("MainWindow", "0.2", nullptr));
        comboBox_OpacityValue->setItemText(9, QApplication::translate("MainWindow", "0.1", nullptr));

        comboBox_OpacityValue->setCurrentText(QApplication::translate("MainWindow", "1", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260\351\242\221\347\216\207", nullptr));
        comboBox_UpdateSpeed->setItemText(0, QApplication::translate("MainWindow", "\345\277\253", nullptr));
        comboBox_UpdateSpeed->setItemText(1, QApplication::translate("MainWindow", "\350\276\203\345\277\253", nullptr));
        comboBox_UpdateSpeed->setItemText(2, QApplication::translate("MainWindow", "\344\270\255", nullptr));
        comboBox_UpdateSpeed->setItemText(3, QApplication::translate("MainWindow", "\350\276\203\346\205\242", nullptr));
        comboBox_UpdateSpeed->setItemText(4, QApplication::translate("MainWindow", "\346\205\242", nullptr));

        groupBox->setTitle(QApplication::translate("MainWindow", "\345\241\253\345\205\205\346\240\267\345\274\217", nullptr));
        radioButton_TextureStyle_Soild->setText(QApplication::translate("MainWindow", "\347\272\257\350\211\262\345\241\253\345\205\205", nullptr));
        btn_PureColorSelect->setText(QString());
        radioButton_TextureStyle_Gradient->setText(QApplication::translate("MainWindow", "\346\270\220\345\217\230\345\241\253\345\205\205", nullptr));
        radioButton_TextureStyle_StableRGB->setText(QApplication::translate("MainWindow", "\345\233\272\345\256\232RGB\345\241\253\345\205\205", nullptr));
        radioButton_TextureStyle_SlideRGB->setText(QApplication::translate("MainWindow", "\345\217\230\345\271\273RGB\345\241\253\345\205\205", nullptr));
        radioButton_TextureStyle_Pattern->setText(QApplication::translate("MainWindow", "\345\233\276\346\241\210\345\241\253\345\205\205", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\350\277\231\346\230\257\344\270\200\345\274\240\345\233\276\347\211\207", nullptr));
        label->setText(QApplication::translate("MainWindow", "\345\271\205\345\272\246\345\244\247\345\260\217", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\346\227\240", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\346\233\264\345\244\247", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "\346\250\241\347\263\212\345\272\246\357\274\210\350\266\212\345\244\247CPU\345\215\240\347\224\250\350\266\212\345\244\232\357\274\211", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "\346\227\240", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "\346\233\264\346\250\241\347\263\212", nullptr));
        tabWidget_main->setTabText(tabWidget_main->indexOf(tab_style), QApplication::translate("MainWindow", "\346\240\267\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
