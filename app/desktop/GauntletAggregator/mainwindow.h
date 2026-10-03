#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSet>
#include <QHash>
#include <QByteArray>

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
private:
    Ui::MainWindow *ui;

    void applyDockProperties();
    QLocalServer server;

    QSet<QLocalSocket*> clients;
    QHash<QLocalSocket*, QByteArray> buffers;
    QHash<QLocalSocket*, QString> appNames;

    QString currentApp;

    void onNewConnection();
    void onClientReadyRead(QLocalSocket *socket);

    void broadcastCommand(
        const QString &type,
        const QString &app
        );

    void selectApp(const QString &app);
    void updateSelectedButton();

};
#endif // MAINWINDOW_H
