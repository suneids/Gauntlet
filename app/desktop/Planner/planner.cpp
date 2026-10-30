#include "planner.h"
#include "ui_planner.h"
#include <QDate>
#include "ui_TaskCreateDialog.h"

constexpr int TASKS_PER_ROW = 5;



void Planner::openCreateTaskDialog(){
    QDialog* dialog = new QDialog(this);
    Ui::TaskCreateDialog* form = new Ui::TaskCreateDialog;
    form->setupUi(dialog);
    connect(form->btn_cancel, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(form->btn_create, &QPushButton::clicked, this, [this, dialog, form]() {
    QString header = form->ledit_header->text().trimmed();
    QString description = form->pledit_description->toPlainText().trimmed();
    int repeats = form->spin_repeats->value();

    if (header.isEmpty()) {
        return;
    }

    addTaskDetailed(header, description, repeats, false);

    dialog->accept();
    });
    connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);

    connect(dialog, &QObject::destroyed, this, [form]() {
        delete form;
    });

    dialog->open();
}


void Planner::addTaskDetailed(QString header, QString description, int repeats, bool isDaily){
    TaskData data;
    data.title = header;
    data.description = description;
    data.target = repeats;
    data.progress = 0;
    data.status = TaskStatus::Active;
    data.createdDate = QDate::currentDate();
    data.dueDate = QDate::currentDate();
    data.isDaily = isDaily;
    auto* task = new TaskItem(data, this);

    current_tasks.append(task);

    connect(task, &TaskItem::completed, this, [this](TaskItem* task) {
        completeTask(task);
        saveTasks();
    });

    connect(task, &TaskItem::cancelled, this, [this](TaskItem* task) {
        cancelTask(task);
        saveTasks();
    });

    connect(task, &TaskItem::deleteRequested, this, [this](TaskItem* task) {
        deleteTask(task);
        saveTasks();
    });

    connect(task, &TaskItem::changed, this, [this](TaskItem* task) {
        saveTasks();
    });

    rebuildCurrentTasksGrid();
    saveTasks();
}


void Planner::addDailyTasks(WeekDay day){
    addSportTasks(day);
    addCookingTasks();
    addOrderTasks();
}


void Planner::addSportTasks(WeekDay day){
    switch(day){
    case WeekDay::Monday:
        addSportHeavyPull();
        break;

    case WeekDay::Tuesday:
        addSportLight();
        break;

    case WeekDay::Wednesday:
        addSportMedium();
        break;

    case WeekDay::Thursday:
        addSportRecovery();
        break;

    case WeekDay::Friday:
        addSportHeavyPull();
        break;

    case WeekDay::Saturday:
        addSportDistance();
        break;

    case WeekDay::Sunday:
        addSportRecovery();
        break;
    }
}


Planner::Planner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Planner)
{
    ui->setupUi(this);

    ui->gridLayout_currentTasks->setAlignment(Qt::AlignTop);
    ui->vlayout_finished_tasks->setAlignment(Qt::AlignTop);

    connect(ui->btn_add_task, &QPushButton::clicked,
            this, &Planner::openCreateTaskDialog);
    connect(ui->btn_clear_tasks, &QPushButton::clicked,
            this, &Planner::clearTasks);

    connect(ui->btn_current_tasks, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pg_current_tasks);
    });
    connect(ui->btn_finished_tasks, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pg_finished_tasks);
    });


    WeekDay day;
    int currentDayNum = QDate::currentDate().dayOfWeek();
    switch (currentDayNum) {
        case 1: day = WeekDay::Monday; break;
        case 2: day = WeekDay::Tuesday; break;
        case 3: day = WeekDay::Wednesday; break;
        case 4: day = WeekDay::Thursday; break;
        case 5: day = WeekDay::Friday; break;
        case 6: day = WeekDay::Saturday; break;
        case 7: day = WeekDay::Sunday; break;
        default: day = WeekDay::Monday; // формально не должен случиться
    }
    setupCurrentTasksGrid();
    addDailyTasks(day);
}


void Planner::setupCurrentTasksGrid()
{
    auto* grid = ui->gridLayout_currentTasks;

    for (int col = 0; col < TASKS_PER_ROW; ++col) {
        grid->setColumnStretch(col, 1);
    }

    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);
}


void Planner::addSportHeavyPull(){
    bool daily = true;
    addTaskDetailed("Спорт: Отжимания", "Сделать 50 отжиманий", 50, daily);
    addTaskDetailed("Спорт: Подтягивания", "Сделать 50 подтягиваний за день подходами по 5-7, не до отказа", 50, daily);
    addTaskDetailed("Спорт: Дистанция", "Преодолеть 5 км", 5, daily);
    addTaskDetailed("Спорт: Планка", "Простоять в планке суммарно 150 секунд", 150, daily);
}


void Planner::addSportMedium(){
    bool daily = true;
    addTaskDetailed("Спорт: Отжимания", "Сделать 50 отжиманий", 50, daily);
    addTaskDetailed("Спорт: Подтягивания", "Сделать 35 подтягиваний без отказа", 35, daily);
    addTaskDetailed("Спорт: Дистанция", "Преодолеть 5 км", 5, daily);
    addTaskDetailed("Спорт: Планка", "Простоять в планке суммарно 120 секунд", 120, daily);
}


void Planner::addSportLight(){
    bool daily = true;
    addTaskDetailed("Спорт: Отжимания", "Сделать 40 отжиманий", 40, daily);
    addTaskDetailed("Спорт: Подтягивания", "Сделать 25 подтягиваний легко, без отказа", 25, daily);
    addTaskDetailed("Спорт: Дистанция", "Преодолеть 3-5 км", 5, daily);
    addTaskDetailed("Спорт: Планка", "Простоять в планке суммарно 90 секунд", 90, daily);
}


void Planner::addSportRecovery(){
    bool daily = true;
    addTaskDetailed("Спорт: Восстановление", "Не делать тяжёлые подтягивания; пройти 3 км или сделать лёгкую разминку", 3, daily);
    addTaskDetailed("Спорт: Планка", "Лёгкая планка суммарно 60 секунд, без боли", 60, daily);
}


void Planner::addSportDistance(){
    bool daily = true;
    addTaskDetailed("Спорт: Отжимания", "Сделать 40 отжиманий", 40, daily);
    addTaskDetailed("Спорт: Подтягивания", "Сделать 20 подтягиваний легко", 20, daily);
    addTaskDetailed("Спорт: Дистанция+", "Преодолеть 10 км", 5, daily);
}


void Planner::addCookingTasks(){
    bool daily = true;
    addTaskDetailed("Ежедневное: Готовка1", "На выбор: заготовить морковь/лук/картофель", 1, daily);
}


void Planner::addOrderTasks(){
    bool daily = true;
    addTaskDetailed("Ежедневное: Порядок", "Уменьшить окружающий хаос в течение 15 минут", 1, daily);
}


void Planner::clearTasks()
{
    while (!current_tasks.isEmpty()) {
        deleteTask(current_tasks.first());
    }
}
\

void Planner::rebuildCurrentTasksGrid()
{
    QGridLayout* grid = ui->gridLayout_currentTasks;

    while (QLayoutItem* item = grid->takeAt(0)) {
        // ВАЖНО: виджеты не удаляем, удаляем только layout-item оболочку
        delete item;
    }

    for (int i = 0; i < current_tasks.size(); ++i) {
        int row = i / TASKS_PER_ROW;
        int col = i % TASKS_PER_ROW;

        grid->addWidget(current_tasks[i], row, col);
    }
}

void Planner::completeTask(TaskItem* task){
    if (!task) {
        return;
    }

    if (!current_tasks.removeOne(task)) {
        return;
    }

    ui->gridLayout_currentTasks->removeWidget(task);

    finished_tasks.append(task);

    rebuildCurrentTasksGrid();

    ui->vlayout_finished_tasks->addWidget(task);
}


void Planner::cancelTask(TaskItem* task){
    if (!task) {
        return;
    }

    if (!current_tasks.removeOne(task)) {
        return;
    }

    ui->gridLayout_currentTasks->removeWidget(task);

    finished_tasks.append(task);

    rebuildCurrentTasksGrid();

    ui->vlayout_finished_tasks->addWidget(task);
}


void Planner::deleteTask(TaskItem* task){
    if (!task) {
        return;
    }

    current_tasks.removeOne(task);
    finished_tasks.removeOne(task);

    ui->gridLayout_currentTasks->removeWidget(task);
    ui->vlayout_finished_tasks->removeWidget(task);

    rebuildCurrentTasksGrid();

    task->deleteLater();
}



Planner::~Planner()
{

    delete ui;
}


// JSON+
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSaveFile>
#include <QDir>
#include <QStandardPaths>

QString Planner::tasksFilePath() const
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (dirPath.isEmpty()) {
        dirPath = QDir::currentPath();
    }

    QDir dir(dirPath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.filePath("tasks.json");
}


void Planner::saveTasks(){
    QJsonObject root;
    root["version"] = 1;

    QJsonArray tasksArray;

    for (TaskItem* task : current_tasks) {
        if (!task) {
            continue;
        }

        tasksArray.append(taskToJson(task->data()));
    }

    for (TaskItem* task : finished_tasks) {
        if (!task) {
            continue;
        }

        tasksArray.append(taskToJson(task->data()));
    }

    root["tasks"] = tasksArray;

    QJsonDocument doc(root);

    QSaveFile file(tasksFilePath());

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open tasks file for writing:" << tasksFilePath();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));

    if (!file.commit()) {
        qDebug() << "Cannot commit tasks file:" << tasksFilePath();
    }
}


QString Planner::statusToString(TaskStatus status){
    switch (status) {
    case TaskStatus::Active:    return "active";
    case TaskStatus::Completed: return "completed";
    case TaskStatus::Cancelled: return "cancelled";
    case TaskStatus::Failed:    return "failed";
    }

    return "active";
}


TaskStatus Planner::statusFromString(const QString& status){
    if (status == "completed") {
        return TaskStatus::Completed;
    }

    if (status == "cancelled") {
        return TaskStatus::Cancelled;
    }

    if (status == "failed") {
        return TaskStatus::Failed;
    }

    return TaskStatus::Active;
}


QJsonObject Planner::taskToJson(const TaskData& task){
    QJsonObject obj;

    obj["id"] = task.id;
    obj["title"] = task.title;
    obj["description"] = task.description;

    obj["progress"] = task.progress;
    obj["target"] = task.target;

    obj["status"] = statusToString(task.status);

    obj["createdDate"] = task.createdDate.toString(Qt::ISODate);
    obj["dueDate"] = task.dueDate.toString(Qt::ISODate);
    obj["completedDate"] = task.completedDate.isValid()
                               ? task.completedDate.toString(Qt::ISODate)
                               : "";

    obj["isDaily"] = task.isDaily;
    obj["dailyKey"] = task.dailyKey;

    return obj;
}


TaskData Planner::taskFromJson(const QJsonObject& obj)
{
    TaskData task;

    task.id = obj["id"].toString();
    task.title = obj["title"].toString();
    task.description = obj["description"].toString();

    task.progress = obj["progress"].toInt(0);
    task.target = obj["target"].toInt(1);

    task.status = statusFromString(obj["status"].toString());

    task.createdDate = QDate::fromString(obj["createdDate"].toString(), Qt::ISODate);
    task.dueDate = QDate::fromString(obj["dueDate"].toString(), Qt::ISODate);

    QString completed = obj["completedDate"].toString();
    if (!completed.isEmpty()) {
        task.completedDate = QDate::fromString(completed, Qt::ISODate);
    }

    task.isDaily = obj["isDaily"].toBool(false);
    task.dailyKey = obj["dailyKey"].toString();

    if (task.target < 1) {
        task.target = 1;
    }

    if (task.progress < 0) {
        task.progress = 0;
    }

    if (task.progress > task.target) {
        task.progress = task.target;
    }

    return task;
}

