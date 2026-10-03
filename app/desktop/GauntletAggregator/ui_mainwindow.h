/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_greenhouse;
    QPushButton *btn_led;
    QPushButton *btn_hexapod;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_modbus_poll;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(40, -1, 40, -1);
        btn_greenhouse = new QPushButton(centralwidget);
        btn_greenhouse->setObjectName("btn_greenhouse");

        horizontalLayout->addWidget(btn_greenhouse);

        btn_led = new QPushButton(centralwidget);
        btn_led->setObjectName("btn_led");

        horizontalLayout->addWidget(btn_led);

        btn_hexapod = new QPushButton(centralwidget);
        btn_hexapod->setObjectName("btn_hexapod");
        btn_hexapod->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(btn_hexapod);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_modbus_poll = new QPushButton(centralwidget);
        btn_modbus_poll->setObjectName("btn_modbus_poll");

        horizontalLayout->addWidget(btn_modbus_poll);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_greenhouse->setText(QString());
        btn_led->setText(QString());
        btn_hexapod->setText(QString());
        btn_modbus_poll->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
