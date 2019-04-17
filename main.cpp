#include <QApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include "mainwindow.h"
#include "simulatorstatehistory.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    SimulatorStateHistory history;
    MainWindow mainWnd(engine.rootObjects()[0], history, *engine.rootContext());

    return app.exec();
}
