#ifndef QANIMATIONSTACKEDWIDGET_H
#define QANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QVariant>

class QPropertyAnimation;

class QAnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
//    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)

public:
    explicit QAnimationStackedWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *ev) override;

public:
    void setDuration(int time);     // 设置动画持续的时间
    void setEasingCurve(const QEasingCurve &easing);          // 设置动画曲线
    void slideLeft();               // 向左滑动
    void slideRight();              // 向右滑动

public slots:
    void valueChanged_slot(QVariant value);  // 属性动画值改变的槽
    void animationFinished();                // 动画切换完成

private:
    void setAnimation();                            // 设置动画的基本信息
    void paintPrevious(QPainter &p, int index);     // 绘制前一个页面
    void paintNext(QPainter &p, int index);         // 绘制下一个页面

private:
    QPropertyAnimation *animation;  // 动画框架
    int duration;                   // 动画持续时间
    bool isAnimation;               // 是否正在动画
    QVariant currentValue;          // 被动画改变的值
    int widgetCount;                // StackWidget中的成员数
    int nextIndex;                  // 下一个要切换的索引

    // 页面滑动的方式
    enum slideWay{left, right} slide = left;
};

#endif // QANIMATIONSTACKEDWIDGET_H
