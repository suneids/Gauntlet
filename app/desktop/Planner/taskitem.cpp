#include "taskitem.h"
#include "ui_TaskItem.h"

#include <QPushButton>
#include <QSpinBox>
#include <QProgressBar>
#include <QLabel>

TaskItem::TaskItem(const TaskData& data, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TaskItem)
    , m_data(data){
    ui->setupUi(this);

    connect(ui->btn_complete, &QPushButton::clicked, this, [this]() {
        addProgress(1);
    });

    connect(ui->btn_complete_multy, &QPushButton::clicked, this, [this]() {
        addProgress(ui->spinBox_complete_multy->value());
    });

    connect(ui->btn_cancel, &QPushButton::clicked, this, [this]() {
        if (m_data.status == TaskStatus::Active) {
            cancelTask();
        } else {
            emit deleteRequested(this);
        }
    });

    syncUiFromData();
}


TaskItem::~TaskItem(){
    delete ui;
}


TaskData TaskItem::data() const{
    return m_data;
}


void TaskItem::setData(const TaskData& data){
    m_data = data;
    syncUiFromData();
}


void TaskItem::addProgress(int amount){
    if (m_data.status != TaskStatus::Active) {
        return;
    }

    if (amount <= 0) {
        return;
    }

    m_data.progress += amount;

    if (m_data.progress > m_data.target) {
        m_data.progress = m_data.target;
    }


    if (m_data.progress >= m_data.target) {
        finishTask();
        return;
    }
    syncUiFromData();
    emit changed(this);

}


void TaskItem::finishTask(){
    if (m_data.status != TaskStatus::Active) {
        return;
    }

    m_data.status = TaskStatus::Completed;
    m_data.completedDate = QDate::currentDate();

    syncUiFromData();

    emit completed(this);
    emit changed(this);
}


void TaskItem::cancelTask(){
    if (m_data.status != TaskStatus::Active) {
        return;
    }

    m_data.status = TaskStatus::Cancelled;

    syncUiFromData();

    emit cancelled(this);
    emit changed(this);
}


void TaskItem::syncUiFromData(){
    if (m_data.target < 1) {
        m_data.target = 1;
    }

    if (m_data.progress < 0) {
        m_data.progress = 0;
    }

    if (m_data.progress > m_data.target) {
        m_data.progress = m_data.target;
    }

    ui->lbl_header->setText(m_data.title);
    ui->lbl_description->setText(m_data.description);
    ui->lbl_description->setWordWrap(true);
    ui->lbl_description->setVisible(!m_data.description.trimmed().isEmpty());

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(m_data.target);
    ui->progressBar->setValue(m_data.progress);
    ui->progressBar->setFormat("%p%");
    ui->progressBar->setAlignment(Qt::AlignCenter);

    ui->progressBar->setStyleSheet(
        "QProgressBar {"
        "    background-color: #111923;"
        "    border: 1px solid #2b3a4a;"
        "    border-radius: 4px;"
        "    text-align: center;"
        "    color: white;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #8a2be2;"
        "    border-radius: 3px;"
        "}"
        );

    ui->spinBox_complete_multy->setMinimum(1);
    ui->spinBox_complete_multy->setMaximum(m_data.target);

    bool active = m_data.status == TaskStatus::Active;

    ui->btn_complete->setEnabled(active);
    ui->btn_complete_multy->setEnabled(active);
    ui->spinBox_complete_multy->setEnabled(active);

    if (m_data.status == TaskStatus::Active) {
        ui->btn_complete->setText("Зачесть +1");
        ui->btn_complete_multy->setText("Зачесть");
        ui->btn_cancel->setText("×");
    }

    if (m_data.status == TaskStatus::Completed) {
        ui->btn_complete->setText("Выполнено");
        ui->btn_complete_multy->setText("Зачесть");
        ui->btn_cancel->setText("Удалить");
    }

    if (m_data.status == TaskStatus::Cancelled) {
        ui->btn_complete->setText("Отменено");
        ui->btn_complete_multy->setText("Зачесть");
        ui->btn_cancel->setText("Удалить");
    }

    if (m_data.status == TaskStatus::Failed) {
        ui->btn_complete->setText("Провалено");
        ui->btn_complete_multy->setText("Зачесть");
        ui->btn_cancel->setText("Удалить");
    }
}
