#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "terminalframe.h"
#include "aggregatorclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showApp();
    void hideApp();

private slots:
    void saveConfig();
    void loadConfig();
private:
    Ui::MainWindow *ui;
    AggregatorClient aggregatorClient;
    QVector<TerminalFrame *> terminals;
};
#endif // MAINWINDOW_H
