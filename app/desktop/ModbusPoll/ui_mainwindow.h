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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutMain;
    QGroupBox *groupConnection;
    QGridLayout *gridConnection;
    QLabel *labelPort;
    QComboBox *comboPort;
    QPushButton *btnRefreshPorts;
    QLabel *labelBaud;
    QComboBox *comboBaud;
    QLabel *labelParity;
    QComboBox *comboParity;
    QLabel *labelDataBits;
    QComboBox *comboDataBits;
    QLabel *labelStopBits;
    QComboBox *comboStopBits;
    QLabel *labelSlave;
    QSpinBox *spinSlave;
    QPushButton *btnConnect;
    QGroupBox *groupPolling;
    QHBoxLayout *horizontalLayoutPolling;
    QLabel *labelStartAddress;
    QSpinBox *spinStartAddress;
    QLabel *labelRegisterCount;
    QSpinBox *spinRegisterCount;
    QLabel *labelPollPeriod;
    QSpinBox *spinPollPeriod;
    QSpacerItem *horizontalSpacerPolling;
    QPushButton *btnOpenConfig;
    QPushButton *btnSaveConfig;
    QTableWidget *tableRegisters;
    QHBoxLayout *horizontalLayoutStatus;
    QLabel *labelStatus;
    QSpacerItem *horizontalSpacerStatus;
    QLabel *labelLastPoll;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelTotalPackets;
    QLabel *labelTotalPackets_value;
    QLabel *labelErrorPackets;
    QLabel *labelErrorPackets_value;
    QPushButton *btnClearCounters;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(980, 680);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutMain = new QVBoxLayout(centralwidget);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        groupConnection = new QGroupBox(centralwidget);
        groupConnection->setObjectName("groupConnection");
        gridConnection = new QGridLayout(groupConnection);
        gridConnection->setObjectName("gridConnection");
        labelPort = new QLabel(groupConnection);
        labelPort->setObjectName("labelPort");

        gridConnection->addWidget(labelPort, 0, 0, 1, 1);

        comboPort = new QComboBox(groupConnection);
        comboPort->setObjectName("comboPort");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboPort->sizePolicy().hasHeightForWidth());
        comboPort->setSizePolicy(sizePolicy);

        gridConnection->addWidget(comboPort, 0, 1, 1, 1);

        btnRefreshPorts = new QPushButton(groupConnection);
        btnRefreshPorts->setObjectName("btnRefreshPorts");

        gridConnection->addWidget(btnRefreshPorts, 0, 2, 1, 1);

        labelBaud = new QLabel(groupConnection);
        labelBaud->setObjectName("labelBaud");

        gridConnection->addWidget(labelBaud, 0, 3, 1, 1);

        comboBaud = new QComboBox(groupConnection);
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->addItem(QString());
        comboBaud->setObjectName("comboBaud");

        gridConnection->addWidget(comboBaud, 0, 4, 1, 1);

        labelParity = new QLabel(groupConnection);
        labelParity->setObjectName("labelParity");

        gridConnection->addWidget(labelParity, 0, 5, 1, 1);

        comboParity = new QComboBox(groupConnection);
        comboParity->addItem(QString());
        comboParity->addItem(QString());
        comboParity->addItem(QString());
        comboParity->setObjectName("comboParity");

        gridConnection->addWidget(comboParity, 0, 6, 1, 1);

        labelDataBits = new QLabel(groupConnection);
        labelDataBits->setObjectName("labelDataBits");

        gridConnection->addWidget(labelDataBits, 1, 0, 1, 1);

        comboDataBits = new QComboBox(groupConnection);
        comboDataBits->addItem(QString());
        comboDataBits->addItem(QString());
        comboDataBits->setObjectName("comboDataBits");

        gridConnection->addWidget(comboDataBits, 1, 1, 1, 1);

        labelStopBits = new QLabel(groupConnection);
        labelStopBits->setObjectName("labelStopBits");

        gridConnection->addWidget(labelStopBits, 1, 3, 1, 1);

        comboStopBits = new QComboBox(groupConnection);
        comboStopBits->addItem(QString());
        comboStopBits->addItem(QString());
        comboStopBits->setObjectName("comboStopBits");

        gridConnection->addWidget(comboStopBits, 1, 4, 1, 1);

        labelSlave = new QLabel(groupConnection);
        labelSlave->setObjectName("labelSlave");

        gridConnection->addWidget(labelSlave, 1, 5, 1, 1);

        spinSlave = new QSpinBox(groupConnection);
        spinSlave->setObjectName("spinSlave");
        spinSlave->setMinimum(1);
        spinSlave->setMaximum(247);
        spinSlave->setValue(1);

        gridConnection->addWidget(spinSlave, 1, 6, 1, 1);

        btnConnect = new QPushButton(groupConnection);
        btnConnect->setObjectName("btnConnect");
        btnConnect->setMinimumSize(QSize(110, 0));

        gridConnection->addWidget(btnConnect, 1, 7, 1, 1);


        verticalLayoutMain->addWidget(groupConnection);

        groupPolling = new QGroupBox(centralwidget);
        groupPolling->setObjectName("groupPolling");
        horizontalLayoutPolling = new QHBoxLayout(groupPolling);
        horizontalLayoutPolling->setObjectName("horizontalLayoutPolling");
        labelStartAddress = new QLabel(groupPolling);
        labelStartAddress->setObjectName("labelStartAddress");

        horizontalLayoutPolling->addWidget(labelStartAddress);

        spinStartAddress = new QSpinBox(groupPolling);
        spinStartAddress->setObjectName("spinStartAddress");
        spinStartAddress->setMaximum(65535);

        horizontalLayoutPolling->addWidget(spinStartAddress);

        labelRegisterCount = new QLabel(groupPolling);
        labelRegisterCount->setObjectName("labelRegisterCount");

        horizontalLayoutPolling->addWidget(labelRegisterCount);

        spinRegisterCount = new QSpinBox(groupPolling);
        spinRegisterCount->setObjectName("spinRegisterCount");
        spinRegisterCount->setMinimum(1);
        spinRegisterCount->setMaximum(125);
        spinRegisterCount->setValue(10);

        horizontalLayoutPolling->addWidget(spinRegisterCount);

        labelPollPeriod = new QLabel(groupPolling);
        labelPollPeriod->setObjectName("labelPollPeriod");

        horizontalLayoutPolling->addWidget(labelPollPeriod);

        spinPollPeriod = new QSpinBox(groupPolling);
        spinPollPeriod->setObjectName("spinPollPeriod");
        spinPollPeriod->setMinimum(50);
        spinPollPeriod->setMaximum(60000);
        spinPollPeriod->setSingleStep(50);
        spinPollPeriod->setValue(500);

        horizontalLayoutPolling->addWidget(spinPollPeriod);

        horizontalSpacerPolling = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutPolling->addItem(horizontalSpacerPolling);

        btnOpenConfig = new QPushButton(groupPolling);
        btnOpenConfig->setObjectName("btnOpenConfig");

        horizontalLayoutPolling->addWidget(btnOpenConfig);

        btnSaveConfig = new QPushButton(groupPolling);
        btnSaveConfig->setObjectName("btnSaveConfig");

        horizontalLayoutPolling->addWidget(btnSaveConfig);


        verticalLayoutMain->addWidget(groupPolling);

        tableRegisters = new QTableWidget(centralwidget);
        if (tableRegisters->columnCount() < 6)
            tableRegisters->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableRegisters->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableRegisters->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableRegisters->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableRegisters->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableRegisters->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableRegisters->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableRegisters->setObjectName("tableRegisters");
        tableRegisters->setAlternatingRowColors(true);
        tableRegisters->setSelectionMode(QAbstractItemView::SingleSelection);
        tableRegisters->setSelectionBehavior(QAbstractItemView::SelectItems);
        tableRegisters->setRowCount(0);
        tableRegisters->setColumnCount(6);
        tableRegisters->horizontalHeader()->setStretchLastSection(false);

        verticalLayoutMain->addWidget(tableRegisters);

        horizontalLayoutStatus = new QHBoxLayout();
        horizontalLayoutStatus->setObjectName("horizontalLayoutStatus");
        labelStatus = new QLabel(centralwidget);
        labelStatus->setObjectName("labelStatus");

        horizontalLayoutStatus->addWidget(labelStatus);

        horizontalSpacerStatus = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutStatus->addItem(horizontalSpacerStatus);

        labelLastPoll = new QLabel(centralwidget);
        labelLastPoll->setObjectName("labelLastPoll");

        horizontalLayoutStatus->addWidget(labelLastPoll);


        verticalLayoutMain->addLayout(horizontalLayoutStatus);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        labelTotalPackets = new QLabel(centralwidget);
        labelTotalPackets->setObjectName("labelTotalPackets");

        horizontalLayout_2->addWidget(labelTotalPackets);

        labelTotalPackets_value = new QLabel(centralwidget);
        labelTotalPackets_value->setObjectName("labelTotalPackets_value");

        horizontalLayout_2->addWidget(labelTotalPackets_value);

        labelErrorPackets = new QLabel(centralwidget);
        labelErrorPackets->setObjectName("labelErrorPackets");

        horizontalLayout_2->addWidget(labelErrorPackets);

        labelErrorPackets_value = new QLabel(centralwidget);
        labelErrorPackets_value->setObjectName("labelErrorPackets_value");

        horizontalLayout_2->addWidget(labelErrorPackets_value);

        btnClearCounters = new QPushButton(centralwidget);
        btnClearCounters->setObjectName("btnClearCounters");

        horizontalLayout_2->addWidget(btnClearCounters);


        verticalLayoutMain->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Modbus Poll Linux", nullptr));
        groupConnection->setTitle(QCoreApplication::translate("MainWindow", "Connection", nullptr));
        labelPort->setText(QCoreApplication::translate("MainWindow", "Port", nullptr));
        btnRefreshPorts->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        labelBaud->setText(QCoreApplication::translate("MainWindow", "Baud", nullptr));
        comboBaud->setItemText(0, QCoreApplication::translate("MainWindow", "1200", nullptr));
        comboBaud->setItemText(1, QCoreApplication::translate("MainWindow", "2400", nullptr));
        comboBaud->setItemText(2, QCoreApplication::translate("MainWindow", "4800", nullptr));
        comboBaud->setItemText(3, QCoreApplication::translate("MainWindow", "9600", nullptr));
        comboBaud->setItemText(4, QCoreApplication::translate("MainWindow", "19200", nullptr));
        comboBaud->setItemText(5, QCoreApplication::translate("MainWindow", "38400", nullptr));
        comboBaud->setItemText(6, QCoreApplication::translate("MainWindow", "57600", nullptr));
        comboBaud->setItemText(7, QCoreApplication::translate("MainWindow", "115200", nullptr));

        labelParity->setText(QCoreApplication::translate("MainWindow", "Parity", nullptr));
        comboParity->setItemText(0, QCoreApplication::translate("MainWindow", "None", nullptr));
        comboParity->setItemText(1, QCoreApplication::translate("MainWindow", "Even", nullptr));
        comboParity->setItemText(2, QCoreApplication::translate("MainWindow", "Odd", nullptr));

        labelDataBits->setText(QCoreApplication::translate("MainWindow", "Data bits", nullptr));
        comboDataBits->setItemText(0, QCoreApplication::translate("MainWindow", "8", nullptr));
        comboDataBits->setItemText(1, QCoreApplication::translate("MainWindow", "7", nullptr));

        labelStopBits->setText(QCoreApplication::translate("MainWindow", "Stop bits", nullptr));
        comboStopBits->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        comboStopBits->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));

        labelSlave->setText(QCoreApplication::translate("MainWindow", "Slave ID", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        groupPolling->setTitle(QCoreApplication::translate("MainWindow", "Polling", nullptr));
        labelStartAddress->setText(QCoreApplication::translate("MainWindow", "Start address", nullptr));
        labelRegisterCount->setText(QCoreApplication::translate("MainWindow", "Registers", nullptr));
        labelPollPeriod->setText(QCoreApplication::translate("MainWindow", "Period, ms", nullptr));
        btnOpenConfig->setText(QCoreApplication::translate("MainWindow", "Open config", nullptr));
        btnSaveConfig->setText(QCoreApplication::translate("MainWindow", "Save config", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableRegisters->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableRegisters->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableRegisters->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Value", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableRegisters->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableRegisters->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableRegisters->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Value", nullptr));
        labelStatus->setText(QCoreApplication::translate("MainWindow", "Disconnected", nullptr));
        labelLastPoll->setText(QCoreApplication::translate("MainWindow", "Last poll: \342\200\224", nullptr));
        labelTotalPackets->setText(QCoreApplication::translate("MainWindow", "Total packets:", nullptr));
        labelTotalPackets_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelErrorPackets->setText(QCoreApplication::translate("MainWindow", "Errors:", nullptr));
        labelErrorPackets_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        btnClearCounters->setText(QCoreApplication::translate("MainWindow", "Clear counters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
