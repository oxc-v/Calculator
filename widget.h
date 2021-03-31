#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QListWidgetItem;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    // 显示按钮text
    void on_btnClicked();

    // 计算表达式
    void on_btnEqual_pressed();

    // 清除所有字符
    void on_btnClear_pressed();

    // 删除一个字符
    void on_btnBack_pressed();

    // 设置正负
    void on_btnNegative_pressed();

    // 显示历史记录界面
    void on_btnHistory_pressed();

    // 点击历史记录条项
    void on_listWidget_itemPressed(QListWidgetItem *item);

    // 清除历史记录
    void on_btnClearData_pressed();

    // 实时显示表达式结果
    void resultDisplay(const QString &text);

private:
    Ui::Widget *ui;

private:

    bool isResult = false;      // 判断lineEdit显示的内容是否为表达式求值结果
};
#endif // WIDGET_H
