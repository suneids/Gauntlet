#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort>
#include <QColor>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QPoint>

#include "timetracker.h"
#include <QQueue>
#include <QTimer>

#include "aggregatorclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showApp();
    void hideApp();

private:

    TimeTracker *timetracker_pg;

    AggregatorClient aggregatorClient;

protected:


    void logLine(const QString &text);


};
#endif // MAINWINDOW_H
