#ifndef PLANNER_H
#define PLANNER_H

#include <QWidget>
#include "taskitem.h"

enum class WeekDay {
    Monday = 1,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

namespace Ui {
class Planner;
class TaskItem;
}

struct TaskView
{
    QWidget* root = nullptr;
    Ui::TaskItem* ui = nullptr;
};

class Planner : public QWidget
{
    Q_OBJECT
public slots:
    void openCreateTaskDialog();
public:
    explicit Planner(QWidget *parent = nullptr);
    ~Planner();

private:
    Ui::Planner *ui;
    QVector<TaskItem*> current_tasks;
    QVector<TaskItem*> finished_tasks;

    void addTaskDetailed(QString header, QString description, int repeats, bool isDaily);
    void addDailyTasks(WeekDay day);
    void addSportTasks(WeekDay day);
    void addSportLight();
    void addSportMedium();
    void addSportHeavyPull();
    void addSportDistance();
    void addSportRecovery();
    void addCookingTasks();
    void addOrderTasks();
    void addTask();
    void clearTasks();
    void setupCurrentTasksGrid();


    void completeTask(TaskItem* task);
    void cancelTask(TaskItem* task);
    void deleteTask(TaskItem* task);
    void rebuildCurrentTasksGrid();

    QString tasksFilePath() const;
    void saveTasks();
    QString statusToString(TaskStatus status);
    TaskStatus statusFromString(const QString& status);
    QJsonObject taskToJson(const TaskData& task);
    TaskData taskFromJson(const QJsonObject& obj);
};

#endif // PLANNER_H
