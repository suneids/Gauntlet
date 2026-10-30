#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QFileInfo>
#include <QTableWidgetItem>

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

struct TimeEntry
{
    QString windowName;
    qint64 seconds = 0;
};

namespace Ui {
class TimeTracker;
}

class TimeTableItem : public QTableWidgetItem
{
public:
    TimeTableItem(const QString& text, qint64 seconds)
        : QTableWidgetItem(text), seconds(seconds) {}

    bool operator<(const QTableWidgetItem& other) const override
    {
        auto* item = dynamic_cast<const TimeTableItem*>(&other);

        return item
                   ? seconds < item->seconds
                   : QTableWidgetItem::operator<(other);
    }

private:
    qint64 seconds;
};

class TimeTracker : public QWidget
{
    Q_OBJECT

public:
    explicit TimeTracker(QWidget *parent = nullptr);
    ~TimeTracker();

private slots:
    void tick();
    void resetPeriod();

private:
    Ui::TimeTracker *ui;

    QTimer tickTimer;
    QTimer periodTimer;

    QString currentWindow;
    QDateTime lastTick;

    QHash<QString, qint64> currentPeriod;
    QHash<QString, qint64> total;

    QString getActiveWindowName();

    void addTime(const QString& windowName, qint64 seconds);
    void updateCurrentTable();
    void updateTotalTable();
    QString formatSeconds(qint64 seconds) const;
};


#endif // TIMETRACKER_H
