/********************************************************************************
** Form generated from reading UI file 'mainframe.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRAME_H
#define UI_MAINFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainFrame
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *buttonOpen;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *buttonDeleteContents;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonShowList;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QLabel *label_5;
    QSpinBox *spinHue;
    QLabel *label_6;
    QDoubleSpinBox *spinSaturation;
    QPushButton *buttonSepiaTone;
    QPushButton *buttonLuminance;
    QSpinBox *spinA;
    QSpinBox *spinB;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *tab_3;
    QPushButton *OtsuLabel;
    QPushButton *GaussianFilter;
    QDoubleSpinBox *SpinSigma;
    QPushButton *BoxFilter;
    QPushButton *MedianFilter;
    QLabel *label;
    QWidget *tab_4;
    QPushButton *HarrisCorner;
    QPushButton *Hough;
    QPushButton *Otical_BTN;
    QPushButton *NEXT_BTN;
    QWidget *tab;
    QPushButton *Btn_ImgBlending;
    QListWidget *listWidget;

    void setupUi(QDialog *MainFrame)
    {
        if (MainFrame->objectName().isEmpty())
            MainFrame->setObjectName(QStringLiteral("MainFrame"));
        MainFrame->resize(622, 461);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainFrame->sizePolicy().hasHeightForWidth());
        MainFrame->setSizePolicy(sizePolicy);
        MainFrame->setMinimumSize(QSize(0, 461));
        MainFrame->setModal(false);
        verticalLayout = new QVBoxLayout(MainFrame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(MainFrame);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 41));
        frame->setMaximumSize(QSize(16777215, 41));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        buttonOpen = new QToolButton(frame);
        buttonOpen->setObjectName(QStringLiteral("buttonOpen"));
        buttonOpen->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonOpen->sizePolicy().hasHeightForWidth());
        buttonOpen->setSizePolicy(sizePolicy2);
        buttonOpen->setMinimumSize(QSize(41, 41));
        buttonOpen->setMaximumSize(QSize(41, 41));
        buttonOpen->setLayoutDirection(Qt::LeftToRight);
        buttonOpen->setAutoFillBackground(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOpen->setIcon(icon);
        buttonOpen->setIconSize(QSize(41, 41));
        buttonOpen->setCheckable(false);
        buttonOpen->setAutoRepeat(false);
        buttonOpen->setAutoExclusive(false);
        buttonOpen->setPopupMode(QToolButton::DelayedPopup);
        buttonOpen->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonOpen->setAutoRaise(false);

        horizontalLayout_3->addWidget(buttonOpen);

        toolButton_2 = new QToolButton(frame);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        sizePolicy2.setHeightForWidth(toolButton_2->sizePolicy().hasHeightForWidth());
        toolButton_2->setSizePolicy(sizePolicy2);
        toolButton_2->setMinimumSize(QSize(41, 41));
        toolButton_2->setMaximumSize(QSize(41, 41));

        horizontalLayout_3->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(frame);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        sizePolicy2.setHeightForWidth(toolButton_3->sizePolicy().hasHeightForWidth());
        toolButton_3->setSizePolicy(sizePolicy2);
        toolButton_3->setMinimumSize(QSize(41, 41));
        toolButton_3->setMaximumSize(QSize(41, 41));

        horizontalLayout_3->addWidget(toolButton_3);

        buttonDeleteContents = new QToolButton(frame);
        buttonDeleteContents->setObjectName(QStringLiteral("buttonDeleteContents"));
        sizePolicy2.setHeightForWidth(buttonDeleteContents->sizePolicy().hasHeightForWidth());
        buttonDeleteContents->setSizePolicy(sizePolicy2);
        buttonDeleteContents->setMinimumSize(QSize(41, 41));
        buttonDeleteContents->setMaximumSize(QSize(41, 41));
        buttonDeleteContents->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/1-21.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDeleteContents->setIcon(icon1);
        buttonDeleteContents->setIconSize(QSize(41, 41));

        horizontalLayout_3->addWidget(buttonDeleteContents);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        buttonShowList = new QToolButton(frame);
        buttonShowList->setObjectName(QStringLiteral("buttonShowList"));
        sizePolicy2.setHeightForWidth(buttonShowList->sizePolicy().hasHeightForWidth());
        buttonShowList->setSizePolicy(sizePolicy2);
        buttonShowList->setMinimumSize(QSize(41, 41));
        buttonShowList->setMaximumSize(QSize(41, 41));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/2-3.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonShowList->setIcon(icon2);
        buttonShowList->setIconSize(QSize(82, 41));

        horizontalLayout_3->addWidget(buttonShowList);


        verticalLayout->addWidget(frame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, -1, -1, -1);
        tabWidget = new QTabWidget(MainFrame);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy3);
        tabWidget->setMinimumSize(QSize(299, 394));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush1(QColor(255, 85, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::NoRole, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::NoRole, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::NoRole, brush1);
        tabWidget->setPalette(palette);
        tabWidget->setCursor(QCursor(Qt::ArrowCursor));
        tabWidget->setAutoFillBackground(false);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(31, 31));
        tabWidget->setElideMode(Qt::ElideMiddle);
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 83, 31, 16));
        spinHue = new QSpinBox(tab_2);
        spinHue->setObjectName(QStringLiteral("spinHue"));
        spinHue->setGeometry(QRect(65, 80, 61, 22));
        spinHue->setAlignment(Qt::AlignCenter);
        spinHue->setMaximum(360);
        spinHue->setSingleStep(20);
        spinHue->setValue(250);
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(140, 83, 31, 16));
        spinSaturation = new QDoubleSpinBox(tab_2);
        spinSaturation->setObjectName(QStringLiteral("spinSaturation"));
        spinSaturation->setGeometry(QRect(163, 80, 62, 22));
        spinSaturation->setAlignment(Qt::AlignCenter);
        spinSaturation->setMaximum(1);
        spinSaturation->setSingleStep(0.1);
        spinSaturation->setValue(0.3);
        buttonSepiaTone = new QPushButton(tab_2);
        buttonSepiaTone->setObjectName(QStringLiteral("buttonSepiaTone"));
        buttonSepiaTone->setGeometry(QRect(30, 30, 201, 41));
        buttonSepiaTone->setAutoFillBackground(false);
        buttonSepiaTone->setIconSize(QSize(41, 41));
        buttonSepiaTone->setCheckable(false);
        buttonSepiaTone->setAutoRepeat(false);
        buttonSepiaTone->setAutoExclusive(false);
        buttonSepiaTone->setAutoDefault(true);
        buttonSepiaTone->setFlat(false);
        buttonLuminance = new QPushButton(tab_2);
        buttonLuminance->setObjectName(QStringLiteral("buttonLuminance"));
        buttonLuminance->setGeometry(QRect(40, 140, 181, 41));
        spinA = new QSpinBox(tab_2);
        spinA->setObjectName(QStringLiteral("spinA"));
        spinA->setGeometry(QRect(70, 210, 51, 21));
        spinA->setMaximum(255);
        spinA->setValue(80);
        spinB = new QSpinBox(tab_2);
        spinB->setObjectName(QStringLiteral("spinB"));
        spinB->setGeometry(QRect(170, 210, 42, 22));
        spinB->setMaximum(255);
        spinB->setValue(40);
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(30, 210, 31, 16));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(130, 210, 31, 16));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        OtsuLabel = new QPushButton(tab_3);
        OtsuLabel->setObjectName(QStringLiteral("OtsuLabel"));
        OtsuLabel->setGeometry(QRect(44, 50, 211, 51));
        GaussianFilter = new QPushButton(tab_3);
        GaussianFilter->setObjectName(QStringLiteral("GaussianFilter"));
        GaussianFilter->setGeometry(QRect(50, 160, 211, 50));
        SpinSigma = new QDoubleSpinBox(tab_3);
        SpinSigma->setObjectName(QStringLiteral("SpinSigma"));
        SpinSigma->setGeometry(QRect(150, 120, 64, 22));
        SpinSigma->setMaximum(2);
        SpinSigma->setSingleStep(0.1);
        BoxFilter = new QPushButton(tab_3);
        BoxFilter->setObjectName(QStringLiteral("BoxFilter"));
        BoxFilter->setGeometry(QRect(50, 220, 211, 41));
        MedianFilter = new QPushButton(tab_3);
        MedianFilter->setObjectName(QStringLiteral("MedianFilter"));
        MedianFilter->setGeometry(QRect(50, 270, 211, 40));
        label = new QLabel(tab_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 120, 50, 20));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        HarrisCorner = new QPushButton(tab_4);
        HarrisCorner->setObjectName(QStringLiteral("HarrisCorner"));
        HarrisCorner->setGeometry(QRect(30, 30, 221, 31));
        Hough = new QPushButton(tab_4);
        Hough->setObjectName(QStringLiteral("Hough"));
        Hough->setGeometry(QRect(30, 100, 221, 41));
        Otical_BTN = new QPushButton(tab_4);
        Otical_BTN->setObjectName(QStringLiteral("Otical_BTN"));
        Otical_BTN->setGeometry(QRect(30, 170, 221, 41));
        NEXT_BTN = new QPushButton(tab_4);
        NEXT_BTN->setObjectName(QStringLiteral("NEXT_BTN"));
        NEXT_BTN->setGeometry(QRect(100, 230, 80, 20));
        tabWidget->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        Btn_ImgBlending = new QPushButton(tab);
        Btn_ImgBlending->setObjectName(QStringLiteral("Btn_ImgBlending"));
        Btn_ImgBlending->setGeometry(QRect(40, 30, 191, 41));
        tabWidget->addTab(tab, QString());

        horizontalLayout->addWidget(tabWidget);

        listWidget = new QListWidget(MainFrame);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy4);
        listWidget->setMinimumSize(QSize(0, 394));
        QPalette palette1;
        QBrush brush2(QColor(255, 255, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush3(QColor(0, 0, 127, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        QBrush brush4(QColor(120, 120, 120, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        QBrush brush5(QColor(240, 240, 240, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        listWidget->setPalette(palette1);
        QFont font;
        font.setFamily(QStringLiteral("Times New Roman"));
        font.setPointSize(10);
        listWidget->setFont(font);

        horizontalLayout->addWidget(listWidget);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(MainFrame);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainFrame);
    } // setupUi

    void retranslateUi(QDialog *MainFrame)
    {
        MainFrame->setWindowTitle(QApplication::translate("MainFrame", "Homeworks", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonOpen->setToolTip(QApplication::translate("MainFrame", "open an image file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonOpen->setText(QString());
        toolButton_2->setText(QApplication::translate("MainFrame", "...", Q_NULLPTR));
        toolButton_3->setText(QApplication::translate("MainFrame", "...", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonDeleteContents->setToolTip(QApplication::translate("MainFrame", "close all forms", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonDeleteContents->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonShowList->setToolTip(QApplication::translate("MainFrame", "show the list view", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonShowList->setText(QString());
        label_5->setText(QApplication::translate("MainFrame", "Hue", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainFrame", "Sat.", Q_NULLPTR));
        buttonSepiaTone->setText(QApplication::translate("MainFrame", "Sepia Tone", Q_NULLPTR));
        buttonLuminance->setText(QApplication::translate("MainFrame", "Luminance Contrast", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainFrame", "A", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainFrame", "B", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainFrame", " cv 1 ", Q_NULLPTR));
        OtsuLabel->setText(QApplication::translate("MainFrame", "Otsu and Label", Q_NULLPTR));
        GaussianFilter->setText(QApplication::translate("MainFrame", "Gaussan Smoothing", Q_NULLPTR));
        BoxFilter->setText(QApplication::translate("MainFrame", "Box Filtering", Q_NULLPTR));
        MedianFilter->setText(QApplication::translate("MainFrame", "Median Filtering", Q_NULLPTR));
        label->setText(QApplication::translate("MainFrame", "Sigma", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainFrame", " cv 2 ", Q_NULLPTR));
        HarrisCorner->setText(QApplication::translate("MainFrame", "HarrisCorner", Q_NULLPTR));
        Hough->setText(QApplication::translate("MainFrame", "Hough", Q_NULLPTR));
        Otical_BTN->setText(QApplication::translate("MainFrame", "Optical flow", Q_NULLPTR));
        NEXT_BTN->setText(QApplication::translate("MainFrame", "->", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainFrame", " cv 3 ", Q_NULLPTR));
        Btn_ImgBlending->setText(QApplication::translate("MainFrame", "ImageBlending", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainFrame", " cv 4 ", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainFrame: public Ui_MainFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRAME_H
