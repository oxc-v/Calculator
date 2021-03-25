#include "historywidget.h"
#include "ui_historywidget.h"

#include <QStringListModel>

historyWidget::historyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::historyWidget)
{
    ui->setupUi(this);

    theListModel = new QStringListModel;
    ui->listView->setModel(theListModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

historyWidget::~historyWidget()
{
    delete ui;
}

// 设置QStringListModel的数据
void historyWidget::setStringList(const QStringList &list)
{
    theListModel->setStringList(list);
}

// 返回主窗口
void historyWidget::on_btnGoBack_pressed()
{
    emit closeHistoryWidget();
}


// 清除历史记录
void historyWidget::on_btnClearData_pressed()
{
    emit clearData();
}

