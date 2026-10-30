#include "terminalframe.h"
#include "ui_terminalframe.h"
#include <QMenu>
#include <QAction>

namespace
{
constexpr int FRAME_WIDTH = 4;
}

TerminalFrame::TerminalFrame(QWidget *parent, QString init_title, QString init_path, QColor init_color)
    : QWidget(parent)
    , ui(new Ui::TerminalFrame)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("TerminalFrame");
    setContentsMargins(FRAME_WIDTH, FRAME_WIDTH, FRAME_WIDTH, FRAME_WIDTH);
    term = new QTermWidget(0, ui->terminalHost);
    term->setContextMenuPolicy(Qt::CustomContextMenu);
    term->installEventFilter(this);

    connect(term, &QWidget::customContextMenuRequested, this, [this](const QPoint &pos){
        QMenu menu(this);

        QAction *copyAction  = menu.addAction("Copy");
        QAction *pasteAction = menu.addAction("Paste");

        connect(copyAction, &QAction::triggered, term, &QTermWidget::copyClipboard);

        connect(pasteAction, &QAction::triggered, term, &QTermWidget::pasteClipboard);

        menu.exec(term->mapToGlobal(pos));
    });

    auto *nextTerminalShortcut = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_Tab), term);
    nextTerminalShortcut->setContext(Qt::WidgetWithChildrenShortcut);

    connect(nextTerminalShortcut, &QShortcut::activated, this, [this]() {
        emit nextTerminalRequested(this);
    });
    term->setColorScheme("Linux");

    auto *copyShortcut = new QShortcut(QKeySequence("Ctrl+Shift+C"), term);
    copyShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(copyShortcut, &QShortcut::activated, term, &QTermWidget::copyClipboard);

    auto *pasteShortcut = new QShortcut(QKeySequence("Ctrl+Shift+V"), term);
    pasteShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(pasteShortcut, &QShortcut::activated, term, &QTermWidget::pasteClipboard);

    auto *layout = new QVBoxLayout(ui->terminalHost);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(term);

    term->setShellProgram("/bin/bash");
    term->startShellProgram();
    setCurrentColor(init_color);
    setTitle(init_title);
    setPath(init_path.isEmpty() ? "/home/suneids" : init_path);

    connect(ui->btnColor, &QPushButton::clicked, this, [this]()
    {
        QColor color = QColorDialog::getColor(m_currentColor, this, "Terminal color");

        if (color.isValid()) setCurrentColor(color);
    });

    connect(ui->btnPath, &QPushButton::clicked, this, [this](){
        QString path = QFileDialog::getExistingDirectory(this, "Terminal directory", m_currentPath);

        if (!path.isEmpty()) setPath(path);
    });
    ui->btnPath->setIcon(QIcon("://icons/directory.png"));
    ui->btnColor->setIcon(QIcon("://icons/palette.png"));
}


bool TerminalFrame::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == term && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        const bool shiftTab =
            keyEvent->key() == Qt::Key_Backtab ||
            (keyEvent->key() == Qt::Key_Tab &&
             keyEvent->modifiers().testFlag(Qt::ShiftModifier));

        if (shiftTab) {
            emit nextTerminalRequested(this);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}


void TerminalFrame::focusTerminal()
{
    term->setFocus();
}


TerminalFrame::~TerminalFrame()
{
    delete ui;
}

void TerminalFrame::applyFrameColor()
{
    setStyleSheet(QString(
        "#TerminalFrame {"
        " border: %1px solid %2;"
        " border-radius: 6px;"
        "}"
    ).arg(FRAME_WIDTH).arg(m_currentColor.name()));
}



QColor TerminalFrame::CurrentColor() const { return m_currentColor; }
QString TerminalFrame::Title()       const { return ui->leditTitle->text(); }
QString TerminalFrame::CurrentPath() const { return m_currentPath; }


void TerminalFrame::setCurrentColor(const QColor &color){
    m_currentColor = color; applyFrameColor();
}


void TerminalFrame::setTitle(const QString &title){
    ui->leditTitle->setText(title);
}


void TerminalFrame::setPath(const QString &path){
    if(path.isEmpty()) return;
    m_currentPath = path;
    term->changeDir(m_currentPath);
    term->sendText("clear\n");
}
