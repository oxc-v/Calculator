#include "widget.h"

#include <QApplication>
#include <QTimer>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_OS_ANDROID
    // 设置程序启动画面延时2秒
    QTimer::singleShot(2000,NULL,[=](){
        QtAndroid::hideSplashScreen(500);
    });
#endif


    // 创建主窗口
    Widget w;
    w.setWindowTitle("OXC御用计算器");
    w.show();

    return a.exec();
}
