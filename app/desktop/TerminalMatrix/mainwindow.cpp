#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

#define MATRIX_COLS 4
#define MATRIX_ROWS 3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , aggregatorClient("terminal_matrix", this)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    for (int row = 0; row < MATRIX_ROWS; ++row)
    {
        for (int col = 0; col < MATRIX_COLS; ++col)
        {
            auto *term = new TerminalFrame(this);

            terminals.append(term);
            ui->gridLayout->addWidget(term, row, col);
            connect(term, &TerminalFrame::nextTerminalRequested, this, [this](TerminalFrame *current) {
                const int index = terminals.indexOf(current);

                if (index < 0 || terminals.isEmpty())
                    return;

                const int next = (index + 1) % terminals.size();

                terminals[next]->focusTerminal();
            });
        }
    }
    qApp->installEventFilter(this);

    connect(ui->btnSaveConfig, &QPushButton::clicked, this, &MainWindow::saveConfig);
    connect(ui->btnLoadConfig, &QPushButton::clicked, this, &MainWindow::loadConfig);

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



void MainWindow::saveConfig(){
    const QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Terminal Matrix config",
        {},
        "JSON files (*.json)"
        );

    if (fileName.isEmpty())
        return;

    QJsonObject root;
    root["version"] = 1;

    QJsonArray terminalsArray;

    for (int id = 0; id < MATRIX_COLS * MATRIX_ROWS; ++id)
    {
        TerminalFrame *currentTerm = terminals[id];

        QJsonObject term;
        term["id"]    = id;
        term["color"] = currentTerm->CurrentColor().name();
        term["label"] = currentTerm->Title();
        term["cwd"]   = currentTerm->CurrentPath();

        terminalsArray.append(term);
    }

    root["terminals"] = terminalsArray;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,"Save config","Could not open file for writing."
            );
        return;
    }

    file.write(
        QJsonDocument(root).toJson(QJsonDocument::Indented)
        );

}


void MainWindow::loadConfig()
{
    const QString fileName = QFileDialog::getOpenFileName(this, "Open Terminal Matrix config", {}, "JSON files (*.json)");

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

    const QJsonObject root = document.object();
    const QJsonArray jsonTerminals = root["terminals"].toArray();

    for (TerminalFrame *term : terminals)
    {
        ui->gridLayout->removeWidget(term);
        delete term;
    }
    terminals.clear();

    for (const QJsonValue &value : jsonTerminals){
        const QJsonObject term = value.toObject();

        const int id = term["id"].toInt();
        const QString title = term["label"].toString();
        const QString path = term["cwd"].toString();
        const QColor color(term["color"].toString());

        auto *newTerm = new TerminalFrame(this, title, path, color);

        terminals.append(newTerm);

        const int row = id / MATRIX_COLS;
        const int col = id % MATRIX_COLS;

        ui->gridLayout->addWidget(newTerm, row, col);
    }
    for (int i = terminals.size() - 1; i > 0; --i)
        QWidget::setTabOrder(terminals[i], terminals[i - 1]);
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
