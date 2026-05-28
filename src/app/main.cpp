#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TtqApp", "Main");

    // used by settings
    QCoreApplication::setOrganizationName("xnelma");
    QCoreApplication::setOrganizationDomain("github.com/xnelma");
    QCoreApplication::setApplicationName("Times Tables Quiz");

    return app.exec();
}
