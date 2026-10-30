#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), aggregatorClient("planner", this)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    planner_pg = new Planner(this);

    setCentralWidget(planner_pg);

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



MainWindow::~MainWindow()
{
    delete ui;
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
