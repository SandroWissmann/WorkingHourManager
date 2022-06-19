#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>

#include "ControllerWeek.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QDate dateOfMonday{2022, 06, 06};

    QTime defaultWorkTime{7, 45};
    QTime pauseTimeMondayToThursday{0, 45};
    QTime pauseTimeFriday{0, 30};

    whm::ControllerWeek controllerWeek{dateOfMonday,
                                       defaultWorkTime,
                                       pauseTimeMondayToThursday,
                                       pauseTimeMondayToThursday,
                                       pauseTimeMondayToThursday,
                                       pauseTimeMondayToThursday,
                                       pauseTimeFriday};

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
    qmlContext->setContextProperty("controllerWeek", &controllerWeek);

    const QUrl url(u"qrc:/WorkingHourManager/frontend/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
