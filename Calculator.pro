QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

lessThan(QT_VERSION, 6): QT += androidextras

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculate.cpp \
    main.cpp \
    qanimationstackedwidget.cpp \
    widget.cpp

HEADERS += \
    calculate.h \
    qanimationstackedwidget.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc \
    qssType.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_sources

DISTFILES += \
    android_sources/AndroidManifest.xml \
    android_sources/build.gradle \
    android_sources/gradle.properties \
    android_sources/gradle/wrapper/gradle-wrapper.jar \
    android_sources/gradle/wrapper/gradle-wrapper.properties \
    android_sources/gradlew \
    android_sources/gradlew.bat \
    android_sources/res/values/libs.xml
