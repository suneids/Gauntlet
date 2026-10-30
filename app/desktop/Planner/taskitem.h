#pragma once

#include <QWidget>
#include <QString>
#include <QDate>

namespace Ui {
class TaskItem;
}

enum class TaskStatus {
    Active,
    Completed,
    Cancelled,
    Failed
};

struct TaskData {
    QString id;

    QString title;
    QString description;

    int progress = 0;
    int target = 1;

    TaskStatus status = TaskStatus::Active;

    QDate createdDate;
    QDate dueDate;
    QDate completedDate;

    bool isDaily = false;
    QString dailyKey;
};

class TaskItem : public QWidget
{
    Q_OBJECT

public:
    explicit TaskItem(const TaskData& data, QWidget* parent = nullptr);
    ~TaskItem();

    TaskData data() const;
    void setData(const TaskData& data);

signals:
    void completed(TaskItem* task);
    void cancelled(TaskItem* task);
    void deleteRequested(TaskItem* task);
    void changed(TaskItem* task);

private:
    Ui::TaskItem* ui;
    TaskData m_data;

    void syncUiFromData();
    void addProgress(int amount);
    void finishTask();
    void cancelTask();
};
