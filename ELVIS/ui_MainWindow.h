/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "GLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QGridLayout *gridLayout;
    QWidget *widget2;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_3;
    QPushButton *pushButton_18;
    QPushButton *pushButton_19;
    QPushButton *pushButton_20;
    GLWidget *widget;
    QWidget *centralWidget_2;
    QLabel *label_5;
    QPushButton *pushButton_25;
    QPushButton *pushButton_26;
    QPushButton *pushButton_27;
    QPushButton *pushButton_28;
    QPushButton *pushButton_29;
    QPushButton *pushButton_30;
    QPushButton *pushButton_31;
    QPushButton *pushButton_32;
    QPushButton *pushButton_33;
    QPushButton *pushButton_34;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1084, 637);
        gridLayout = new QGridLayout(MainWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget2 = new QWidget(MainWindow);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setMinimumSize(QSize(909, 10));
        widget2->setMaximumSize(QSize(16777215, 26));
        pushButton_9 = new QPushButton(widget2);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(10, 0, 61, 21));
        pushButton_10 = new QPushButton(widget2);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(90, 0, 61, 21));
        pushButton_11 = new QPushButton(widget2);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(170, 0, 71, 21));

        gridLayout->addWidget(widget2, 0, 0, 1, 3);

        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(91, 441));
        centralWidget->setMaximumSize(QSize(91, 16777215));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 71, 31));
        label->setMaximumSize(QSize(71, 41));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 40, 91, 21));
        pushButton->setMaximumSize(QSize(91, 21));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 70, 91, 21));
        pushButton_2->setMaximumSize(QSize(91, 21));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(0, 100, 91, 21));
        pushButton_3->setMaximumSize(QSize(91, 21));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(0, 130, 91, 21));
        pushButton_4->setMaximumSize(QSize(91, 21));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 190, 71, 41));
        label_3->setMaximumSize(QSize(71, 41));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);
        label_3->setWordWrap(false);
        pushButton_18 = new QPushButton(centralWidget);
        pushButton_18->setObjectName(QStringLiteral("pushButton_18"));
        pushButton_18->setGeometry(QRect(0, 230, 91, 21));
        pushButton_18->setMaximumSize(QSize(91, 21));
        pushButton_19 = new QPushButton(centralWidget);
        pushButton_19->setObjectName(QStringLiteral("pushButton_19"));
        pushButton_19->setGeometry(QRect(0, 260, 91, 21));
        pushButton_19->setMaximumSize(QSize(91, 21));
        pushButton_20 = new QPushButton(centralWidget);
        pushButton_20->setObjectName(QStringLiteral("pushButton_20"));
        pushButton_20->setGeometry(QRect(0, 290, 91, 21));
        pushButton_20->setMaximumSize(QSize(91, 21));

        gridLayout->addWidget(centralWidget, 1, 0, 1, 1);

        widget = new GLWidget(MainWindow);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(872, 587));
        widget->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(widget, 1, 1, 1, 1);

        centralWidget_2 = new QWidget(MainWindow);
        centralWidget_2->setObjectName(QStringLiteral("centralWidget_2"));
        centralWidget_2->setMinimumSize(QSize(91, 371));
        centralWidget_2->setMaximumSize(QSize(91, 16777215));
        centralWidget_2->setLayoutDirection(Qt::RightToLeft);
        label_5 = new QLabel(centralWidget_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 71, 41));
        label_5->setMaximumSize(QSize(71, 41));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);
        label_5->setWordWrap(false);
        pushButton_25 = new QPushButton(centralWidget_2);
        pushButton_25->setObjectName(QStringLiteral("pushButton_25"));
        pushButton_25->setGeometry(QRect(0, 140, 91, 21));
        pushButton_25->setMaximumSize(QSize(91, 21));
        pushButton_26 = new QPushButton(centralWidget_2);
        pushButton_26->setObjectName(QStringLiteral("pushButton_26"));
        pushButton_26->setGeometry(QRect(0, 170, 91, 21));
        pushButton_26->setMaximumSize(QSize(91, 21));
        pushButton_27 = new QPushButton(centralWidget_2);
        pushButton_27->setObjectName(QStringLiteral("pushButton_27"));
        pushButton_27->setGeometry(QRect(0, 200, 91, 21));
        pushButton_27->setMaximumSize(QSize(91, 21));
        pushButton_28 = new QPushButton(centralWidget_2);
        pushButton_28->setObjectName(QStringLiteral("pushButton_28"));
        pushButton_28->setGeometry(QRect(0, 50, 91, 21));
        pushButton_28->setMaximumSize(QSize(91, 21));
        pushButton_29 = new QPushButton(centralWidget_2);
        pushButton_29->setObjectName(QStringLiteral("pushButton_29"));
        pushButton_29->setGeometry(QRect(0, 80, 91, 21));
        pushButton_29->setMaximumSize(QSize(91, 21));
        pushButton_30 = new QPushButton(centralWidget_2);
        pushButton_30->setObjectName(QStringLiteral("pushButton_30"));
        pushButton_30->setGeometry(QRect(0, 110, 91, 21));
        pushButton_30->setMaximumSize(QSize(91, 21));
        pushButton_31 = new QPushButton(centralWidget_2);
        pushButton_31->setObjectName(QStringLiteral("pushButton_31"));
        pushButton_31->setGeometry(QRect(0, 230, 91, 21));
        pushButton_31->setMaximumSize(QSize(91, 21));
        pushButton_32 = new QPushButton(centralWidget_2);
        pushButton_32->setObjectName(QStringLiteral("pushButton_32"));
        pushButton_32->setGeometry(QRect(0, 260, 91, 21));
        pushButton_32->setMaximumSize(QSize(91, 21));
        pushButton_33 = new QPushButton(centralWidget_2);
        pushButton_33->setObjectName(QStringLiteral("pushButton_33"));
        pushButton_33->setGeometry(QRect(0, 290, 91, 21));
        pushButton_33->setMaximumSize(QSize(91, 21));
        pushButton_34 = new QPushButton(centralWidget_2);
        pushButton_34->setObjectName(QStringLiteral("pushButton_34"));
        pushButton_34->setGeometry(QRect(0, 320, 91, 21));
        pushButton_34->setMaximumSize(QSize(91, 21));

        gridLayout->addWidget(centralWidget_2, 1, 2, 1, 1);


        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), widget, SLOT(drawPolyline()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), widget, SLOT(drawCircle()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), widget, SLOT(drawRectangle()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), widget, SLOT(drawEllipse()));
        QObject::connect(pushButton_18, SIGNAL(clicked()), widget, SLOT(viewGrid()));
        QObject::connect(pushButton_19, SIGNAL(clicked()), widget, SLOT(viewZoomIn()));
        QObject::connect(pushButton_20, SIGNAL(clicked()), widget, SLOT(viewZoomOut()));
        QObject::connect(pushButton_28, SIGNAL(clicked(bool)), widget, SLOT(editSelection()));
        QObject::connect(pushButton_29, SIGNAL(clicked(bool)), widget, SLOT(editVertexSel()));
        QObject::connect(pushButton_30, SIGNAL(clicked(bool)), widget, SLOT(editPolySel()));
        QObject::connect(pushButton_25, SIGNAL(clicked(bool)), widget, SLOT(editCopy()));
        QObject::connect(pushButton_26, SIGNAL(clicked(bool)), widget, SLOT(editDelete()));
        QObject::connect(pushButton_27, SIGNAL(clicked(bool)), widget, SLOT(editTranslation()));
        QObject::connect(pushButton_31, SIGNAL(clicked()), widget, SLOT(editCreateGroup()));
        QObject::connect(pushButton_32, SIGNAL(clicked(bool)), widget, SLOT(editDelGroup()));
        QObject::connect(pushButton_33, SIGNAL(clicked()), widget, SLOT(editIncLayer()));
        QObject::connect(pushButton_34, SIGNAL(clicked()), widget, SLOT(editDecLayer()));
        QObject::connect(pushButton_9, SIGNAL(clicked()), widget, SLOT(fileLoad()));
        QObject::connect(pushButton_10, SIGNAL(clicked()), widget, SLOT(fileSave()));
        QObject::connect(pushButton_11, SIGNAL(clicked()), widget, SLOT(fileSaveSVG()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ELVIS", 0));
        pushButton_9->setText(QApplication::translate("MainWindow", "Open", 0));
        pushButton_10->setText(QApplication::translate("MainWindow", "Save", 0));
        pushButton_11->setText(QApplication::translate("MainWindow", "Save SVG", 0));
        label->setText(QApplication::translate("MainWindow", "Drawing", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Polyline", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Circle", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Rectangle", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Ellipse", 0));
        label_3->setText(QApplication::translate("MainWindow", "Viewing", 0));
        pushButton_18->setText(QApplication::translate("MainWindow", "Grid", 0));
        pushButton_19->setText(QApplication::translate("MainWindow", "Zoom+", 0));
        pushButton_20->setText(QApplication::translate("MainWindow", "Zoom-", 0));
        label_5->setText(QApplication::translate("MainWindow", "Editing", 0));
        pushButton_25->setText(QApplication::translate("MainWindow", "Copy", 0));
        pushButton_26->setText(QApplication::translate("MainWindow", "Delete", 0));
        pushButton_27->setText(QApplication::translate("MainWindow", "Translate", 0));
        pushButton_28->setText(QApplication::translate("MainWindow", "Selection", 0));
        pushButton_29->setText(QApplication::translate("MainWindow", "Vertex Sel.", 0));
        pushButton_30->setText(QApplication::translate("MainWindow", "Poly Sel.", 0));
        pushButton_31->setText(QApplication::translate("MainWindow", "Group", 0));
        pushButton_32->setText(QApplication::translate("MainWindow", "Ungroup", 0));
        pushButton_33->setText(QApplication::translate("MainWindow", "Inc. Layer", 0));
        pushButton_34->setText(QApplication::translate("MainWindow", "Dec. Layer", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
