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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "GLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QGridLayout *gridLayout;
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QSpacerItem *horizontalSpacer;
    GLWidget *widget;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(927, 483);
        gridLayout = new QGridLayout(MainWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(91, 421));
        centralWidget->setMaximumSize(QSize(91, 421));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 71, 41));
        label->setMinimumSize(QSize(71, 41));
        label->setMaximumSize(QSize(71, 41));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 60, 91, 21));
        pushButton->setMaximumSize(QSize(91, 21));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 190, 71, 41));
        label_2->setMaximumSize(QSize(71, 41));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);
        label_2->setWordWrap(false);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 90, 91, 21));
        pushButton_2->setMaximumSize(QSize(91, 21));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(0, 120, 91, 21));
        pushButton_3->setMaximumSize(QSize(91, 21));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(0, 150, 91, 21));
        pushButton_4->setMaximumSize(QSize(91, 21));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(0, 240, 91, 21));
        pushButton_5->setMaximumSize(QSize(91, 21));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(0, 270, 91, 21));
        pushButton_6->setMaximumSize(QSize(91, 21));
        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(0, 300, 91, 21));
        pushButton_7->setMaximumSize(QSize(91, 21));

        gridLayout->addWidget(centralWidget, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(2, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        widget = new GLWidget(MainWindow);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(801, 451));
        widget->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(widget, 0, 2, 2, 1);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), widget, SLOT(drawPolyline()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), widget, SLOT(drawCircle()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), widget, SLOT(drawRectangle()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), widget, SLOT(drawEllipse()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), widget, SLOT(editCopy()));
        QObject::connect(pushButton_7, SIGNAL(clicked()), widget, SLOT(editTranslation()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ELVIS", 0));
        label->setText(QApplication::translate("MainWindow", "Drawing", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Polyline", 0));
        label_2->setText(QApplication::translate("MainWindow", "Editing", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Circle", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Rectangle", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Ellipse", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Copy", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "Delete", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "Translate", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
