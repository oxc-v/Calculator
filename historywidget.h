#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>

namespace Ui {
class historyWidget;
}

class QStringListModel;

class historyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit historyWidget(QWidget *parent = nullptr);
    ~historyWidget();

    // 设置QStringListModel的数据
    void setStringList(const QStringList &list);

private slots:

    // 返回主窗口
    void on_btnGoBack_pressed();

    // 清除历史记录
    void on_btnClearData_pressed();

private:
    Ui::historyWidget *ui;

    QStringListModel *theListModel;

signals:

    // 关闭历史记录窗口时发射此信号
    void closeHistoryWidget();

    // 清除历史记录时发射此信号
    void clearData();
};

#endif // HISTORYWIDGET_H
