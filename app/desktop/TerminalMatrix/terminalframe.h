#ifndef TERMINALFRAME_H
#define TERMINALFRAME_H

#include <QWidget>
#include <QShortcut>
#include <QColorDialog>
#include <QFileDialog>
#include <qtermwidget.h>

namespace Ui {
class TerminalFrame;
}

class TerminalFrame : public QWidget
{
    Q_OBJECT

signals:
    void nextTerminalRequested(TerminalFrame *terminalFrame);

public:
    explicit TerminalFrame(QWidget *parent = nullptr,  QString init_title = "terminal",
                           QString init_path = "/home/suneids", QColor init_color =  Qt::gray);
    ~TerminalFrame();
    void applyFrameColor();

    QColor CurrentColor() const;
    QString Title() const;
    QString CurrentPath() const;

    void setCurrentColor(const QColor &color);
    void setTitle(const QString &title);
    void setPath(const QString &path);

    void focusTerminal();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::TerminalFrame *ui;
    QColor m_currentColor = Qt::gray;
    QString title, m_currentPath = "/home/suneids";
    QTermWidget *term = nullptr;
};

#endif // TERMINALFRAME_H
