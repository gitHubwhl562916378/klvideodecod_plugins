#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "QmlVideo/videorender.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    new VideoData;
//    qmlRegisterType<VideoRender>("MyModel",1,0,"VideoRender");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
