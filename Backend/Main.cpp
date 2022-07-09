#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>

#include "Main/Backend.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    whm::Date firstDate{2021, 01, 01};
    whm::Time defaultWorkTimePerDay{7, 45};
    whm::Time pauseTimeMondayToThursday{0, 45};
    whm::Time pauseTimeFriday{0, 30};

    whm::Backend backend{
        firstDate,
        defaultWorkTimePerDay,
        {pauseTimeMondayToThursday,
         pauseTimeMondayToThursday,
         pauseTimeMondayToThursday,
         pauseTimeMondayToThursday,
         pauseTimeFriday}};

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "WorkingHourManager_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;

    auto qmlContext = engine.rootContext();
    qmlContext->setContextProperty("backend", &backend);

    const QUrl url(u"qrc:/WorkingHourManager/Frontend/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
