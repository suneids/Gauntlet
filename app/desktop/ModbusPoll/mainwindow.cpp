#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHash>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusReply>
#include <QModbusRtuSerialClient>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalBlocker>
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_modbus(new QModbusRtuSerialClient(this))
    , m_pollTimer(new QTimer(this))
    , aggregatorClient("modbus_poll", this)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->tableRegisters->verticalHeader()->setVisible(false);
    ui->comboBaud->setCurrentText("9600");
    ui->comboParity->setCurrentText("None");
    ui->comboDataBits->setCurrentText("8");
    ui->comboStopBits->setCurrentText("1");

    ui->tableRegisters->horizontalHeader()->setStretchLastSection(true);

    connect(ui->btnRefreshPorts, &QPushButton::clicked,this, &MainWindow::refreshPorts);
    connect(ui->btnConnect, &QPushButton::clicked,this, &MainWindow::toggleConnection);
    connect(ui->btnSaveConfig, &QPushButton::clicked, this, &MainWindow::saveConfig);
    connect(ui->btnOpenConfig, &QPushButton::clicked, this, &MainWindow::loadConfig);

    connect(ui->spinStartAddress, &QSpinBox::valueChanged,this, &MainWindow::rebuildRegisterTable);
    connect(ui->spinRegisterCount, &QSpinBox::valueChanged,this, &MainWindow::rebuildRegisterTable);

    connect(ui->spinPollPeriod, &QSpinBox::valueChanged,this, [this](int value) {
        m_pollTimer->setInterval(value);
    });

    connect(m_pollTimer, &QTimer::timeout, this, &MainWindow::pollRegisters);

    connect(ui->tableRegisters, &QTableWidget::cellDoubleClicked, this, [this](int row, int column) {
        if (column == 0 || column == 3)
            selectRegisterType(row, column);

        if (column == 2 || column == 5)
            writeRegister(row, column);
    });

    connect(m_modbus, &QModbusDevice::stateChanged,this, [this](QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            setConnectionUi(true);
            ui->labelStatus->setText("Connected");
            m_pollPending = false;
            m_pollTimer->start(ui->spinPollPeriod->value());
            pollRegisters();
        }
        else if (state == QModbusDevice::UnconnectedState) {
            m_pollTimer->stop();
            m_pollPending = false;
            setConnectionUi(false);
            ui->labelStatus->setText("Disconnected");
            ui->labelLastPoll->setText("Last poll: —");
        }
    });

    connect(m_modbus, &QModbusDevice::errorOccurred, this, [this](QModbusDevice::Error error) {
        if (error == QModbusDevice::NoError)
            return;

        ui->labelStatus->setText(
            QString("Error: %1").arg(m_modbus->errorString()));
    });

    connect(ui->btnClearCounters, &QPushButton::clicked, this, [this](){
        error_packets = 0;
        accepted_packes = 0;
        ui->labelErrorPackets_value->setText(QString::number(error_packets));
        ui->labelTotalPackets_value->setText(QString::number(error_packets + accepted_packes));
    });

    m_pollTimer->setInterval(ui->spinPollPeriod->value());

    aggregatorClient.connectToAggregator();
    connect(
        &aggregatorClient,
        &AggregatorClient::showRequested,
        this,
        &MainWindow::showApp
    );

    connect(
        &aggregatorClient,
        &AggregatorClient::hideRequested,
        this,
        &MainWindow::hideApp
    );

    refreshPorts();
    rebuildRegisterTable();
    setConnectionUi(false);
}

MainWindow::~MainWindow(){
    if (m_modbus->state() != QModbusDevice::UnconnectedState)
        m_modbus->disconnectDevice();

    delete ui;
}

void MainWindow::refreshPorts(){
    const QString previousPort = ui->comboPort->currentData().toString();

    ui->comboPort->clear();

    for (const QSerialPortInfo &port : QSerialPortInfo::availablePorts()) {
        QString label = port.systemLocation();

        if (!port.description().isEmpty())
            label += QString(" — %1").arg(port.description());

        ui->comboPort->addItem(label, port.systemLocation());
    }

    const int oldIndex = ui->comboPort->findData(previousPort);
    if (oldIndex >= 0)
        ui->comboPort->setCurrentIndex(oldIndex);
}


void MainWindow::toggleConnection(){
    if (m_modbus->state() != QModbusDevice::UnconnectedState) {
        m_pollTimer->stop();
        m_modbus->disconnectDevice();
        return;
    }

    if (ui->comboPort->currentIndex() < 0) {
        QMessageBox::warning(this, "Modbus", "No serial port selected.");
        return;
    }

    configureClient();
    ui->labelStatus->setText("Connecting...");

    if (!m_modbus->connectDevice()) {
        ui->labelStatus->setText(
            QString("Connect failed: %1").arg(m_modbus->errorString()));
    }
}

void MainWindow::configureClient(){
    m_modbus->setConnectionParameter(QModbusDevice::SerialPortNameParameter, ui->comboPort->currentData().toString());

    m_modbus->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, ui->comboBaud->currentText().toInt());

    QSerialPort::Parity parity = QSerialPort::NoParity;
    if (ui->comboParity->currentText() == "Even")     parity = QSerialPort::EvenParity;
    else if (ui->comboParity->currentText() == "Odd") parity = QSerialPort::OddParity;

    m_modbus->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);

    const QSerialPort::DataBits dataBits = (ui->comboDataBits->currentText() == "7") ? QSerialPort::Data7 : QSerialPort::Data8;

    m_modbus->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);

    const QSerialPort::StopBits stopBits =(ui->comboStopBits->currentText() == "2") ? QSerialPort::TwoStop : QSerialPort::OneStop;

    m_modbus->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);

    m_modbus->setTimeout(500);
    m_modbus->setNumberOfRetries(1);
}

void MainWindow::setConnectionUi(bool connected){
    ui->btnConnect->setText(connected ? "Disconnect" : "Connect");

    ui->comboPort->setEnabled(!connected);
    ui->btnRefreshPorts->setEnabled(!connected);
    ui->comboBaud->setEnabled(!connected);
    ui->comboParity->setEnabled(!connected);
    ui->comboDataBits->setEnabled(!connected);
    ui->comboStopBits->setEnabled(!connected);
}


void MainWindow::rebuildRegisterTable(){
    QHash<int, QString> oldNames;

    // Сохраняем имена из обеих половин таблицы
    for (int row = 0; row < ui->tableRegisters->rowCount(); ++row) {

        for (int side = 0; side < 2; ++side) {

            const int addressColumn = side * 3;
            const int nameColumn = addressColumn + 1;

            const auto *addressItem = ui->tableRegisters->item(row, addressColumn);

            const auto *nameItem = ui->tableRegisters->item(row, nameColumn);

            if (addressItem && nameItem && !addressItem->text().isEmpty()) {
                oldNames.insert(addressItem->text().toInt(), nameItem->text());
            }
        }
    }


    const int startAddress = ui->spinStartAddress->value();
    const int count = ui->spinRegisterCount->value();

    // Сколько строк потребуется на одну половину.
    // 10 регистров -> 5 строк
    // 11 регистров -> 6 строк
    const int rowsPerSide = (count + 1) / 2;

    ui->tableRegisters->clearContents();
    ui->tableRegisters->setRowCount(rowsPerSide);


    for (int i = 0; i < count; ++i) {

        const int address = startAddress + i;

        int row;
        int addressColumn;

        if (i < rowsPerSide) {
            // Левая половина
            row = i;
            addressColumn = 0;
        }
        else {
            // Правая половина
            row = i - rowsPerSide;
            addressColumn = 3;
        }

        const int nameColumn = addressColumn + 1;
        const int valueColumn = addressColumn + 2;

        auto *addressItem = new QTableWidgetItem(QString::number(address));

        addressItem->setFlags(addressItem->flags() & ~Qt::ItemIsEditable);

        auto *nameItem = new QTableWidgetItem(oldNames.value(address));
        auto *valueItem = new QTableWidgetItem("-");

        valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);

        ui->tableRegisters->setItem(row, addressColumn, addressItem);

        ui->tableRegisters->setItem(row, nameColumn, nameItem);

        ui->tableRegisters->setItem(row, valueColumn, valueItem);
    }
}


void MainWindow::pollRegisters(){
    if (m_modbus->state() != QModbusDevice::ConnectedState)
        return;

    // Если предыдущий запрос ещё не завершился — новый не отправляем.
    if (m_pollPending)
        return;

    const int startAddress = ui->spinStartAddress->value();
    const int count = ui->spinRegisterCount->value();
    const int slaveId = ui->spinSlave->value();

    QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, startAddress, static_cast<quint16>(count));

    QModbusReply *reply = m_modbus->sendReadRequest(request, slaveId);

    if (!reply) {
        ui->labelStatus->setText(QString("Read request failed: %1").arg(m_modbus->errorString()));
        return;
    }

    m_pollPending = true;

    connect(reply, &QModbusReply::finished, this, [this, reply]() {
        m_pollPending = false;

        if (reply->error() == QModbusDevice::NoError) {

            const QModbusDataUnit unit = reply->result();
            updateRegisterValues(unit);

            ui->labelStatus->setText("Connected");
            ui->labelLastPoll->setText("Last poll: OK");
            accepted_packes++;
        }
        else {
            ui->labelStatus->setText(QString("Read error: %1").arg(reply->errorString()));

            ui->labelLastPoll->setText("Last poll: error");
            error_packets++;
        }
        ui->labelErrorPackets_value->setText(QString::number(error_packets));
        ui->labelTotalPackets_value->setText(QString::number(error_packets + accepted_packes));

        reply->deleteLater();
    });
}


void MainWindow::updateRegisterValues(const QModbusDataUnit &unit){
    const int count = static_cast<int>(unit.valueCount());
    const int rowsPerSide = (ui->spinRegisterCount->value() + 1) / 2;

    bool continuation = false;

    for (int i = 0; i < count; ++i) {
        int row;
        int valueColumn;

        if (i < rowsPerSide) {
            row = i;
            valueColumn = 2;
        }
        else {
            row = i - rowsPerSide;
            valueColumn = 5;
        }

        auto *item = ui->tableRegisters->item(row, valueColumn);

        if (!item)
            continue;

        // Этот регистр уже является второй половиной предыдущего 32-битного значения.
        if (continuation) {
            item->setText("-");
            continuation = false;
            continue;
        }

        const int address = unit.startAddress() + i;
        const RegisterType type = m_registerTypes.value(address, RegisterType::UInt16);

        switch (type) {
        case RegisterType::UInt16:
            item->setText(QString::number(unit.value(i)));
            break;

        case RegisterType::Int16: {
            const qint16 value = static_cast<qint16>(unit.value(i));
            item->setText(QString::number(value));
            break;
        }

        case RegisterType::UInt32_ABCD: {
            if (i + 1 >= count) {
                item->setText("?");
                break;
            }

            const quint32 value =
                (static_cast<quint32>(unit.value(i)) << 16) |
                static_cast<quint32>(unit.value(i + 1));

            item->setText(QString::number(value));
            continuation = true;
            break;
        }

        case RegisterType::Float32_ABCD: {
            if (i + 1 >= count) {
                item->setText("?");
                break;
            }

            const quint32 raw =
                (static_cast<quint32>(unit.value(i)) << 16) |
                static_cast<quint32>(unit.value(i + 1));

            float value;
            std::memcpy(&value, &raw, sizeof(value));

            item->setText(QString::number(value, 'g', 7));
            continuation = true;
            break;
        }
        }
    }
}


void MainWindow::writeRegister(int row, int column){
    // Value находится только в колонках 2 и 5.
    if (column != 2 && column != 5)
        return;

    if (m_modbus->state() != QModbusDevice::ConnectedState)
        return;

    // Если кликнули Value слева:
    // Address находится в column 0.
    //
    // Если Value справа:
    // Address находится в column 3.
    const int addressColumn =
        (column == 2) ? 0 : 3;

    const auto *addressItem =
        ui->tableRegisters->item(row, addressColumn);

    const auto *valueItem =
        ui->tableRegisters->item(row, column);

    if (!addressItem || !valueItem)
        return;

    const int address =
        addressItem->text().toInt();

    bool currentOk = false;

    int currentValue =
        valueItem->text().toInt(&currentOk);

    if (!currentOk)
        currentValue = 0;

    bool ok = false;

    const int newValue =
        QInputDialog::getInt(
            this,
            "Write Holding Register",
            QString("Register %1:").arg(address),
            currentValue,
            0,
            65535,
            1,
            &ok);

    if (!ok)
        return;

    QModbusDataUnit request(
        QModbusDataUnit::HoldingRegisters,
        address,
        1);

    request.setValue(
        0,
        static_cast<quint16>(newValue));

    QModbusReply *reply =
        m_modbus->sendWriteRequest(
            request,
            ui->spinSlave->value());

    if (!reply) {
        QMessageBox::warning(
            this,
            "Modbus write",
            m_modbus->errorString());

        return;
    }

    connect(reply, &QModbusReply::finished,
            this, [this, reply, address]() {

                if (reply->error() != QModbusDevice::NoError) {

                    QMessageBox::warning(
                        this,
                        "Modbus write",
                        QString("Register %1:\n%2")
                            .arg(address)
                            .arg(reply->errorString()));
                }

                reply->deleteLater();

                // После записи сразу перечитаем значения.
                pollRegisters();
            });
}


void MainWindow::selectRegisterType(int row, int column){
    auto *addressItem = ui->tableRegisters->item(row, column);

    if (!addressItem)
        return;

    const int address = addressItem->text().toInt();

    const QStringList types = {
        "UInt16",
        "Int16",
        "UInt32 ABCD",
        "Float32 ABCD"
    };

    const RegisterType currentType = m_registerTypes.value(address, RegisterType::UInt16);

    int currentIndex = 0;

    switch (currentType) {
    case RegisterType::UInt16:       currentIndex = 0; break;
    case RegisterType::Int16:        currentIndex = 1; break;
    case RegisterType::UInt32_ABCD:  currentIndex = 2; break;
    case RegisterType::Float32_ABCD: currentIndex = 3; break;
    }

    bool ok = false;
    const QString selected = QInputDialog::getItem(this, "Register type", QString("Register %1").arg(address), types, currentIndex, false, &ok);

    if (!ok)
        return;

    if (selected == "UInt16")
        m_registerTypes.remove(address);
    else if (selected == "Int16")
        m_registerTypes[address] = RegisterType::Int16;
    else if (selected == "UInt32 ABCD")
        m_registerTypes[address] = RegisterType::UInt32_ABCD;
    else if (selected == "Float32 ABCD")
        m_registerTypes[address] = RegisterType::Float32_ABCD;

    pollRegisters();
}


void MainWindow::saveConfig(){
    const QString fileName = QFileDialog::getSaveFileName(this, "Save Modbus config", {}, "JSON files (*.json)");

    if (fileName.isEmpty())
        return;

    QJsonObject connection;
    connection["port"] = ui->comboPort->currentData().toString();
    connection["baud"] = ui->comboBaud->currentText().toInt();
    connection["parity"] = ui->comboParity->currentText();
    connection["dataBits"] = ui->comboDataBits->currentText().toInt();
    connection["stopBits"] = ui->comboStopBits->currentText().toInt();
    connection["slave"] = ui->spinSlave->value();

    QJsonObject poll;
    poll["startAddress"] = ui->spinStartAddress->value();
    poll["registerCount"] = ui->spinRegisterCount->value();
    poll["periodMs"] = ui->spinPollPeriod->value();

    QJsonObject registerNames;

    for (int row = 0; row < ui->tableRegisters->rowCount(); ++row) {
        for (int side = 0; side < 2; ++side) {
            const int addressColumn = side * 3;
            const int nameColumn = addressColumn + 1;

            const auto *addressItem = ui->tableRegisters->item(row, addressColumn);
            const auto *nameItem = ui->tableRegisters->item(row, nameColumn);

            if (!addressItem || !nameItem)
                continue;

            const QString name = nameItem->text();

            if (!name.isEmpty())
                registerNames[addressItem->text()] = name;
        }
    }

    QJsonObject registerTypes;

    for (auto it = m_registerTypes.constBegin(); it != m_registerTypes.constEnd(); ++it) {
        QString typeName;

        switch (it.value()) {
        case RegisterType::UInt16:
            typeName = "UInt16";
            break;

        case RegisterType::Int16:
            typeName = "Int16";
            break;

        case RegisterType::UInt32_ABCD:
            typeName = "UInt32_ABCD";
            break;

        case RegisterType::Float32_ABCD:
            typeName = "Float32_ABCD";
            break;
        }

        registerTypes[QString::number(it.key())] = typeName;
    }

    QJsonObject root;
    root["version"] = 1;
    root["connection"] = connection;
    root["poll"] = poll;
    root["registerNames"] = registerNames;
    root["registerTypes"] = registerTypes;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save config", "Could not open file for writing.");
        return;
    }

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
}


void MainWindow::loadConfig(){
    const QString fileName = QFileDialog::getOpenFileName(this, "Open Modbus config", {}, "JSON files (*.json)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Open config", "Could not open file.");
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        QMessageBox::warning(this, "Open config", "Invalid JSON file.");
        return;
    }

    if (m_modbus->state() != QModbusDevice::UnconnectedState)
        m_modbus->disconnectDevice();

    const QJsonObject root = document.object();
    const QJsonObject connection = root["connection"].toObject();
    const QJsonObject poll = root["poll"].toObject();
    const QJsonObject registerNames = root["registerNames"].toObject();
    const QJsonObject registerTypes = root["registerTypes"].toObject();

    refreshPorts();

    const QString port = connection["port"].toString();
    int portIndex = ui->comboPort->findData(port);

    if (portIndex < 0 && !port.isEmpty()) {
        ui->comboPort->addItem(port, port);
        portIndex = ui->comboPort->count() - 1;
    }

    if (portIndex >= 0)
        ui->comboPort->setCurrentIndex(portIndex);

    ui->comboBaud->setCurrentText(QString::number(connection["baud"].toInt(9600)));
    ui->comboParity->setCurrentText(connection["parity"].toString("None"));
    ui->comboDataBits->setCurrentText(QString::number(connection["dataBits"].toInt(8)));
    ui->comboStopBits->setCurrentText(QString::number(connection["stopBits"].toInt(1)));
    ui->spinSlave->setValue(connection["slave"].toInt(1));

    {
        QSignalBlocker blockStart(ui->spinStartAddress);
        QSignalBlocker blockCount(ui->spinRegisterCount);

        ui->spinStartAddress->setValue(poll["startAddress"].toInt(0));
        ui->spinRegisterCount->setValue(poll["registerCount"].toInt(10));
    }

    ui->spinPollPeriod->setValue(poll["periodMs"].toInt(500));

    rebuildRegisterTable();

    for (int row = 0; row < ui->tableRegisters->rowCount(); ++row) {
        for (int side = 0; side < 2; ++side) {
            const int addressColumn = side * 3;
            const int nameColumn = addressColumn + 1;

            auto *addressItem = ui->tableRegisters->item(row, addressColumn);
            auto *nameItem = ui->tableRegisters->item(row, nameColumn);

            if (!addressItem || !nameItem)
                continue;

            const QString address = addressItem->text();

            if (registerNames.contains(address))
                nameItem->setText(registerNames[address].toString());
        }
    }

    m_registerTypes.clear();
    for (auto it = registerTypes.constBegin(); it != registerTypes.constEnd(); ++it) {
        const int address = it.key().toInt();
        const QString typeName = it.value().toString();

        if (typeName == "Int16")
            m_registerTypes[address] = RegisterType::Int16;
        else if (typeName == "UInt32_ABCD")
            m_registerTypes[address] = RegisterType::UInt32_ABCD;
        else if (typeName == "Float32_ABCD")
            m_registerTypes[address] = RegisterType::Float32_ABCD;
    }
}


void MainWindow::showApp()
{
    showMaximized();
    raise();
    activateWindow();
}

void MainWindow::hideApp()
{
    hide();
}
