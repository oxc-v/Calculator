#include "widget.h"

#include <QApplication>
#include <QTimer>
#include <QtAndroid>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置程序启动画面延时1秒
    QTimer::singleShot(1000,NULL,[=](){
        QtAndroid::hideSplashScreen(500);
    });

    // 创建主窗口
    Widget w;
    w.setWindowTitle("OXC御用计算器");
    w.show();

    return a.exec();
}
