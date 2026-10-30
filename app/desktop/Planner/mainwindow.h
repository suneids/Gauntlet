#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QPoint>
#include "planner.h"

#include "taskitem.h"
#include "aggregatorclient.h"
#include <QQueue>
#include <QTimer>


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
public slots:

private:
    Ui::MainWindow *ui;
    AggregatorClient aggregatorClient;
    Planner *planner_pg;

protected:

};
#endif // MAINWINDOW_H
