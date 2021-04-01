#include "qanimationstackedwidget.h"

#include <QPropertyAnimation>
#include <QPainter>

QAnimationStackedWidget::QAnimationStackedWidget(QWidget *parent)
    :QStackedWidget(parent)
{
    isAnimation = false;    // 出始状态
    duration = 1000;        //设置默认的时间间隔

    //初始化animation框架、并连接信号和槽
    animation = new QPropertyAnimation(this, QByteArray());
    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));
}

void QAnimationStackedWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)

    if(isAnimation) {

        QPainter paint(this);

        // 获取Widget
        QWidget *currentWidget = widget(currentIndex());
        QWidget *nextWidget    = widget(nextIndex);

        // 将Widget绘制到Pixmap上面
        QPixmap currentPixmap(currentWidget->size());
        QPixmap nextPixmap(nextWidget->size());
        currentWidget->render(&currentPixmap);
        nextWidget->render(&nextPixmap);

        // 获取Widget的大小
        QRect currentRect = currentWidget->geometry();
        QRect nextRect    = nextWidget->geometry();

        double value = currentValue.toDouble();

        // 绘制Widget
        QRectF currentR1;
        QRectF currentR2;
        QRectF nextR1;
        QRectF nextR2;
        if (slide == right) {

            // 绘制当前Widget
            currentR1.setRect(value, 0.0, currentRect.width() - value, currentRect.height());
            currentR2.setRect(value, 0.0, currentRect.width() - value, currentRect.height());
            paint.drawPixmap(currentR1, currentPixmap, currentR1);

            // 绘制下一个Widget
            nextR1.setRect(0.0, 0.0, value, nextRect.height());
            nextR2.setRect(nextRect.width() - value, 0.0, value, nextRect.height());
            paint.drawPixmap(nextR1, nextPixmap, nextR2);

        } else if (slide == left) {
            // 绘制当前Widget
            currentR1.setRect(0.0, 0.0, currentRect.width() - value, currentRect.height());
            currentR2.setRect(0.0, 0.0, currentRect.width() - value, currentRect.height());
            paint.drawPixmap(currentR1, currentPixmap, currentR1);

            // 绘制下一个Widget
            nextR1.setRect(nextRect.width() - value, 0.0, value, nextRect.height());
            nextR2.setRect(0.0, 0.0, value, nextRect.height());
            paint.drawPixmap(nextR1, nextPixmap, nextR2);
        }
    }
}

void QAnimationStackedWidget::setDuration(int time)
{
    duration = time;
}

void QAnimationStackedWidget::setEasingCurve(const QEasingCurve &easing)
{
    animation->setEasingCurve(easing);
}

// 向左滑动
void QAnimationStackedWidget::slideLeft()
{
    slide = left;
    setAnimation();
}

// 向右滑动
void QAnimationStackedWidget::slideRight()
{
    slide = right;
    setAnimation();
}

void QAnimationStackedWidget::valueChanged_slot(QVariant value)
{
    currentValue = value;
    update();
}

void QAnimationStackedWidget::animationFinished()
{
    isAnimation = false;
    widget(currentIndex())->show();
    setCurrentIndex(nextIndex);
}

void QAnimationStackedWidget::setAnimation()
{
    //如果正在动画，退出函数
    if( isAnimation )
        return;

    isAnimation = true;

    // 计算切换页面的索引
    widgetCount = count();
    int c = currentIndex();
    if (slide == left)
        nextIndex = (c - 1 + widgetCount) % widgetCount;
    else
        nextIndex = (c + 1) % widgetCount;

    //隐藏当前的widget
    widget(c)->hide();

    // 开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    animation->setStartValue(0);
    animation->setEndValue(g.width());
    animation->setDuration(duration);
    animation->start();
}

void QAnimationStackedWidget::paintPrevious(QPainter &p, int index)
{
    // 获得当前页面的Widget
    QWidget *w = widget(index);
    QPixmap pixmap(w->size());

    // 将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
    w->render(&pixmap);
    QRect r = w->geometry();

    // 绘制当前的Widget
    double value = currentValue.toDouble();
    QRectF r1(value, 0.0, r.width() - value, r.height());
    QRectF r2(value, 0.0, r.width() - value, r.height());
    p.drawPixmap(r1, pixmap, r2);
}

void QAnimationStackedWidget::paintNext(QPainter &p, int index)
{
    QWidget *nextWidget = widget(index);
    QRect r = geometry();

    //这行代码不加会有bug，第一次切换的时候，QStackedWidget并没有为child分配大小
    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = currentValue.toDouble();
    QRectF r1(0.0, 0.0, value, r.height());
    QRectF r2(r.width() - value, 0.0, value, r.height());
    p.drawPixmap(r1, nextPixmap, r2);
}
