#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QJsonDocument>
#include <QJsonObject>>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint
        );

    QScreen *screen = QGuiApplication::primaryScreen();

    if(screen)
    {
        QRect r = screen->geometry();

        constexpr int PANEL_HEIGHT = 64;

        setGeometry(
            r.x(),
            r.bottom() - PANEL_HEIGHT + 1,
            r.width(),
            PANEL_HEIGHT
            );
    }

    // Принудительно создаём native X11 window.
    winId();

    applyDockProperties();
    QLocalServer::removeServer("aggregator");

    connect(&server, &QLocalServer::newConnection, this, &MainWindow::onNewConnection);

    if(!server.listen("aggregator"))
    {
        qDebug() << "Aggregator listen error:"
                 << server.errorString();
    }

    connect(ui->btn_greenhouse,&QPushButton::clicked, this, [this]()
        {
            selectApp("greenhouse");
        }
    );

    connect(ui->btn_led, &QPushButton::clicked, this, [this]()
        {
            selectApp("led");
        }
    );

    connect(ui->btn_hexapod, &QPushButton::clicked, this, [this]()
        {
            selectApp("hexapod");
        }
    );

    connect(ui->btn_modbus_poll, &QPushButton::clicked, this, [this]()
        {
            selectApp("modbus_poll");
        }
    );

    ui->btn_greenhouse->setIcon(QIcon("://icons/greenhouse.png"));
    ui->btn_hexapod->setIcon(QIcon("://icons/hexapod.png"));
    ui->btn_led->setIcon(QIcon("://icons/led_strip.png"));
    ui->btn_modbus_poll->setIcon(QIcon("://icons/modbus_poll.png"));

    QPushButton *icon_buttons[4] = {ui->btn_greenhouse, ui->btn_hexapod, ui->btn_led,  ui->btn_modbus_poll};
    for(int i = 0; i < 4; i++){
        icon_buttons[i]->setFlat(true);
        icon_buttons[i]->setFixedSize(48, 48);
        icon_buttons[i]->setIconSize(QSize(32, 32));
        icon_buttons[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        icon_buttons[i]->setFocusPolicy(Qt::NoFocus);
        icon_buttons[i]->setStyleSheet(R"(
            QPushButton {
                background: transparent;
                border: none;
                padding: 8px;
            }

            QPushButton:hover {
                background: rgba(255, 255, 255, 25);
                border-radius: 8px;
            }

            QPushButton:pressed {
                background: rgba(255, 255, 255, 45);
                border-radius: 8px;
            }

            QPushButton:checked {
                background: rgba(255, 255, 255, 45);
                border-radius: 8px;
            }
            )");
    }

}


void MainWindow::applyDockProperties()
{
    Display *display = XOpenDisplay(nullptr);
    if(!display) return;

    Window window = static_cast<Window>(winId());

    // Говорим WM: это системная панель / dock.
    Atom typeAtom = XInternAtom(display, "_NET_WM_WINDOW_TYPE",False);

    Atom dockAtom = XInternAtom(display,"_NET_WM_WINDOW_TYPE_DOCK",False);

    XChangeProperty(display, window, typeAtom, XA_ATOM, 32, PropModeReplace,
                    reinterpret_cast<unsigned char*>(&dockAtom), 1);

    QScreen *screen = QGuiApplication::primaryScreen();
    if(!screen) {
        XCloseDisplay(display);
        return;
    }

    QRect r = screen->geometry();

    constexpr unsigned long PANEL_HEIGHT = 64;

    /*
     * _NET_WM_STRUT:
     *
     * left
     * right
     * top
     * bottom
     */
    unsigned long strut[4] = {0, 0, 0, PANEL_HEIGHT};

    Atom strutAtom = XInternAtom(display, "_NET_WM_STRUT", False);

    XChangeProperty(display, window, strutAtom, XA_CARDINAL, 32,
                    PropModeReplace, reinterpret_cast<unsigned char*>(strut), 4);

    /*
     * _NET_WM_STRUT_PARTIAL:
     *
     * left, right, top, bottom,
     * left_start_y, left_end_y,
     * right_start_y, right_end_y,
     * top_start_x, top_end_x,
     * bottom_start_x, bottom_end_x
     */
    unsigned long partial[12] = {
        0,
        0,
        0,
        PANEL_HEIGHT,
        0, 0,
        0, 0,
        0, 0,
        static_cast<unsigned long>(r.x()), static_cast<unsigned long>(r.x() + r.width() - 1)
    };

    Atom partialAtom = XInternAtom(display,"_NET_WM_STRUT_PARTIAL",False);

    XChangeProperty(display, window, partialAtom, XA_CARDINAL, 32, PropModeReplace,
                    reinterpret_cast<unsigned char*>(partial), 12);

    XFlush(display);
    XCloseDisplay(display);
}


void MainWindow::onNewConnection(){
    while(server.hasPendingConnections()){
        QLocalSocket *socket = server.nextPendingConnection();

        clients.insert(socket);
        buffers[socket] = QByteArray();

        connect(socket, &QLocalSocket::readyRead, this, [this, socket]()
            {
                onClientReadyRead(socket);
            }
            );

        connect(socket, &QLocalSocket::disconnected, this, [this, socket]()
            {
                qDebug()
                    << "Disconnected:"
                    << appNames.value(socket);

                clients.remove(socket);
                buffers.remove(socket);
                appNames.remove(socket);

                socket->deleteLater();
            }
            );
    }
}


void MainWindow::onClientReadyRead(QLocalSocket *socket){
    buffers[socket] += socket->readAll();

    while(true){
        int pos = buffers[socket].indexOf('\n');

        if(pos < 0) return;

        QByteArray raw = buffers[socket].left(pos);

        buffers[socket].remove(0, pos + 1);

        QJsonDocument doc = QJsonDocument::fromJson(raw);

        if(!doc.isObject()) continue;

        QJsonObject obj = doc.object();

        QString type = obj["type"].toString();

        if(type == "register"){
            QString app =
                obj["app"].toString();

            appNames[socket] = app;

            qDebug()
                << "Registered:"
                << app;
        }
    }
}


void MainWindow::broadcastCommand(const QString &type, const QString &app){
    QJsonObject obj;

    obj["type"] = type;
    obj["app"]  = app;

    QByteArray data =
        QJsonDocument(obj)
            .toJson(QJsonDocument::Compact);

    data += '\n';

    for(QLocalSocket *socket : clients){
        if(socket->state() == QLocalSocket::ConnectedState){
            socket->write(data);
        }
    }
}


void MainWindow::selectApp(const QString &app){
    // Сначала гарантированно прячем всё.
    broadcastCommand("hide", "*");

    // Повторный клик по активной иконке:
    // просто всё остаётся скрытым.
    if(currentApp == app){
        currentApp.clear();
        updateSelectedButton();
        return;
    }

    // Теперь показываем только выбранное.
    broadcastCommand("show", app);

    currentApp = app;

    updateSelectedButton();
}


void MainWindow::updateSelectedButton(){
    ui->btn_greenhouse->setStyleSheet("");
    ui->btn_led->setStyleSheet("");
    ui->btn_hexapod->setStyleSheet("");

    QString style =
        "border: 2px solid white;"
        "border-radius: 8px;";

    if(currentApp == "greenhouse")   ui->btn_greenhouse->setStyleSheet(style);
    else if(currentApp == "led")     ui->btn_led->setStyleSheet(style);
    else if(currentApp == "hexapod") ui->btn_hexapod->setStyleSheet(style);
}


MainWindow::~MainWindow()
{
    delete ui;
}
