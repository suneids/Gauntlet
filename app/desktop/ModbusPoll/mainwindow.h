#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aggregatorclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QModbusRtuSerialClient;
class QModbusDataUnit;

class QTimer;

enum class RegisterType {
    UInt16,
    Int16,
    UInt32_ABCD,
    Float32_ABCD
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showApp();
    void hideApp();

private slots:
    void refreshPorts();
    void toggleConnection();
    void rebuildRegisterTable();
    void pollRegisters();
    void writeRegister(int row, int column);
    void selectRegisterType(int row, int column);

    void saveConfig();
    void loadConfig();

private:
    void configureClient();
    void setConnectionUi(bool connected);
    void updateRegisterValues(const QModbusDataUnit &unit);

    Ui::MainWindow *ui;
    QModbusRtuSerialClient *m_modbus = nullptr;
    QTimer *m_pollTimer = nullptr;
    bool m_pollPending = false;
    AggregatorClient aggregatorClient;
    QHash<int, RegisterType> m_registerTypes;
    QHash<int, quint16> m_rawRegisters;
    long long int accepted_packes = 0, error_packets = 0;
};

#endif // MAINWINDOW_H
