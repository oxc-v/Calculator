#include "widget.h"
#include "ui_widget.h"
#include "calculate.h"

#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 显示计算页面
    ui->stackedWidget->setCurrentIndex(0);

    // 设置图标大小
#ifdef Q_OS_ANDROID
    ui->btnHistory->setIconSize(QSize(128, 128));
    ui->btnBack->setIconSize(QSize(128, 128));
#endif
    // 加载qss样式表
    QFile file(":/myStyle/pushButtonStyle.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    // 连接信号和槽函数
    connect(ui->btn0, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn1, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn2, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn3, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn4, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn5, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn6, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn7, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn8, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btn9, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnDivide, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnMultiply, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnSubtract, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnAdd, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnLeft, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnRight, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnPoint, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->btnPrecent, SIGNAL(pressed()), this, SLOT(on_btnClicked()));
    connect(ui->lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(resultDisplay(const QString &)));
}

Widget::~Widget()
{
    delete ui;
}

// 显示按钮text
void Widget::on_btnClicked()
{
    // 获取触发信号的button按钮的指针
    QPushButton *btn = qobject_cast<QPushButton *>(sender());

    // 如果此时lineEdit显示的是上一次运算的结果
    if (isResult) {

        // 清除lineEdit上的内容
        ui->lineEdit->clear();

        // 重新置为false
        isResult = false;
    }

    // 将button的text显示在lineEdit上
    ui->lineEdit->insert(btn->text());
}

// 计算表达式
void Widget::on_btnEqual_pressed()
{
    // 解析表达式
    Calculate ca = Calculate(ui->lineEdit->text());

    // 清除lineOutput的内容
    ui->lineOutput->clear();

    if (!ca.correct) {

        // 表达式有语法或词法错误
        ui->lineOutput->setText("Error");

    } else {

        // 此时lineEdit显示的是运算结果
        isResult = true;

        // 显示历史记录
        QString str = ui->lineEdit->text() + "\n" + "="+ QString::number(ca.result);
        QListWidgetItem *newItem = new QListWidgetItem(str);
        newItem->setTextAlignment(Qt::AlignRight);
        ui->listWidget->addItem(newItem);

        // 清除lineEdit的内容并显示表达式的值
        ui->lineEdit->clear();
        ui->lineEdit->setText(QString::number(ca.result));
    }
}

// 清除所有字符
void Widget::on_btnClear_pressed()
{
    // 清除lineOutput和lineEdit的内容
    ui->lineEdit->clear();
    ui->lineOutput->clear();
}

// 删除一个字符
void Widget::on_btnBack_pressed()
{
    ui->lineEdit->backspace();
}

// 设置正负
void Widget::on_btnNegative_pressed()    // 有bug
{
    // 判断一个字符是否为数字
    bool digit = false;

    // 获取lineEdit的内容
    QString str = ui->lineEdit->text();

    // 如果lineEdit为空，不用进行正负设置
    if (str.isEmpty())
        return;

    // 扫描表达式
    int i;
    for (i = str.size() - 1; i >= 0; i--) {
        if (str.at(i) <= '9' && str.at(i) >= '0')
            digit = true;
        else
            break;
    }

    if (digit) {

        // 如果表达式的最后一个单词是数字，添加负号
        str.insert(i + 1, '(');
        str.insert(i + 2, '-');
        str.insert(str.size(), ')');

    } else if (str.at(str.size() - 1) == ')' && str.size() >= 4){

        // 如果表达式的最后几个单词是(-数字)形式，则将()和-去掉
        int j;
        for (j = str.size() - 2; j >= 0; j--) {

            while (str.at(j) <= '9' && str.at(j) >= '0') {

                j--;
                if (j < 0)
                    break;
            }
            if (j + 1 < str.size() && str.at(j) == '(' && str.at(j + 1) == '-') {

                str.remove(j, 2);
                str.remove(str.size() - 1, 1);
                break;
            }
        }
    }

    // 重新设置lineEdit的内容
    ui->lineEdit->clear();
    ui->lineEdit->setText(str);
}

// 显示历史记录界面
void Widget::on_btnHistory_pressed()
{
    // 获取当前页面索引
    int index = ui->stackedWidget->currentIndex();

    // 切换页面和图标
    if (index == 0) {
        ui->btnHistory->setIcon(QIcon("://android_sources/images/goBack.png"));
        ui->stackedWidget->setCurrentIndex(1);
    }
    else {
        ui->btnHistory->setIcon(QIcon(":/btnIcon/android_sources/images/history.png"));
        ui->stackedWidget->setCurrentIndex(0);
    }
}

// 点击历史记录条项
void Widget::on_listWidget_itemPressed(QListWidgetItem *item)
{
    // 提取运算结果
    QString str = item->text();
    QStringList list = str.split("=");
    str = list.at(list.size() - 1);

    // 如果结果是负数，则为结果添加()
    bool ok = false;
    double result = str.toDouble(&ok);
    if (ok && result < 0) {
        str.insert(0, '(');
        str.insert(str.size(), ')');
    }

    if (isResult) {
        ui->lineEdit->setText(str);
        isResult = false;
    } else {
        ui->lineEdit->insert(str);
    }

    ui->listWidget->clearSelection();
}

// 清除历史记录
void Widget::on_btnClearData_pressed()
{
    ui->listWidget->clear();
}

// 实时显示表达式结果
void Widget::resultDisplay(const QString &text)
{

    ui->lineOutput->clear();

    // 如果lineEdit的内容不是空的并且不是表达式的结果
    if (!isResult && !text.isEmpty()) {

        Calculate tmp = Calculate(text);
        if (!tmp.correct) {

            ui->lineOutput->setText("Error");
        } else {

            ui->lineOutput->setText(QString::number(tmp.result));
        }
    }
}

