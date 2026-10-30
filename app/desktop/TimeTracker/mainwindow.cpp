#include "mainwindow.h"
#include <algorithm>
#include <QDir>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , aggregatorClient("time_tracker", this)
{
    timetracker_pg = new TimeTracker(this);
    setCentralWidget(timetracker_pg);


    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

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

}
\

MainWindow::~MainWindow()
{

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


