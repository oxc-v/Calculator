#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class historyWidget;

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

    // 跳转到历史记录窗口
    void on_btnHistory_pressed();

    // 实时显示表达式结果
    void resultDisplay(const QString &text);

    // 显示主窗口
    void showWidget();

    // 清除历史记录
    void clearHistory();

private:
    Ui::Widget *ui;

private:

    historyWidget *hw;          // 历史记录窗口
    QStringList historyList;    // 存放多个表达式

    bool isResult = false;      // 判断lineEdit显示的内容是否为表达式求值结果
};
#endif // WIDGET_H
