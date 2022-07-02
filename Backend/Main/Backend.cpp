#include "Backend.hpp"

#include "Backend/ControllerYear.hpp"
#include "Common/ControllerWeek.hpp"
#include "Common/ControllerWeek/ControllerDay.hpp"

#include "Common/TimeConvert.hpp"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringLiteral>

namespace whm {

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const QDate &firstDate,
    QTime defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
    QObject *parent);

QDate goBackToPreviousMonday(const QDate &firstDate);

// similar to build in functions QDate::addDays but negative and only for
// one day
void removeDay(QDate &date);

QDate makeCurrentDate();

QJsonObject makeApplicationDataJsonObject(
    const QTime &defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
    const QVector<QObject *> &controllerWeeks);

QJsonArray makeHolidaysPerYearJsonArray();

QJsonArray
makePauseTimesPerDayJsonArray(const std::array<QTime, 5> &pauseTimesPerDay);

QJsonArray makeWeeksJsonArray(const QVector<QObject *> &controllerWeeks);

QJsonObject makeWeekJsonObject(const ControllerWeek &controllerWeek);

QJsonArray makeDaysJsonArray(const QVector<QObject *> &controllerDays);

QJsonObject makeDayJsonObject(const ControllerDay &controllerDay);

} // namespace

Backend::Backend(
    const QDate &firstDate,
    const QTime &defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
    QObject *parent)
    : QObject{parent}, m_defaultWorkTimePerDay{defaultWorkTimePerDay},
      m_pauseTimesPerDay{pauseTimesPerDay},
      m_controllerWeeks{makeControllerWeeksUntilCurrentWeek(
          firstDate,
          defaultWorkTimePerDay,
          pauseTimesPerDay,
          this)}
{
}

QVector<QObject *> Backend::controllerWeeks() const
{
    return m_controllerWeeks;
}

QVector<QObject *> Backend::controllerYears() const
{
    return m_controllerYears;
}

void Backend::saveToFile()
{
    QFile saveFile{QStringLiteral("save.json")};

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << Q_FUNC_INFO << "Couldn't open save file.";
        return;
    }

    QJsonObject jsonObject = makeApplicationDataJsonObject(
        m_defaultWorkTimePerDay, m_pauseTimesPerDay, m_controllerWeeks);

    saveFile.write(QJsonDocument(jsonObject).toJson());
}

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const QDate &firstDate,
    QTime defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
    QObject *parent)
{
    auto startDate = goBackToPreviousMonday(firstDate);

    // auto startDate = firstDate;
    auto endDate = makeCurrentDate();

    QVector<QObject *> controllerWeeks;
    while (startDate <= endDate) {
        controllerWeeks.emplace_back(new ControllerWeek{
            startDate, defaultWorkTimePerDay, pauseTimesPerDay, parent});
        startDate = startDate.addDays(7);
    }

    return controllerWeeks;
}

QDate goBackToPreviousMonday(const QDate &firstDate)
{
    QDate date = firstDate;
    constexpr int dayOfWeekMonday = 1;

    while (date.dayOfWeek() != dayOfWeekMonday) {
        removeDay(date);
    }

    return date;
}

void removeDay(QDate &date)
{
    auto year = date.year();
    auto month = date.month();
    auto day = date.day();

    date.setDate(year, month, day - 1);

    // we were at start of the month so go back one month
    if (!date.isValid()) {

        date.setDate(year, month - 1, 1);
        auto daysInMonth = date.daysInMonth();
        date.setDate(year, month - 1, daysInMonth);

        // we were at start of the year so go back one year
        if (!date.isValid()) {
            date.setDate(year - 1, 12, 31);
        }
    }

    if (!date.isValid()) {
        qDebug() << Q_FUNC_INFO << "Date is invalid";
    }
}

QDate makeCurrentDate()
{
    auto currentDateTime = QDateTime::currentDateTime();
    return currentDateTime.date();
}

QJsonObject makeApplicationDataJsonObject(
    const QTime &defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
    const QVector<QObject *> &controllerWeeks)
{
    QJsonObject jsonObject;
    jsonObject["defaultWorkTimePerDay"] = timeToString(defaultWorkTimePerDay);
    jsonObject["holidaysPerYear"] = makeHolidaysPerYearJsonArray();
    jsonObject["pauseTimesPerDay"] =
        makePauseTimesPerDayJsonArray(pauseTimesPerDay);
    jsonObject["weeks"] = makeWeeksJsonArray(controllerWeeks);
    return jsonObject;
}

// TODO implement holidays calculation and store holidays
// this has to be array based on many years are show up in the list.
QJsonArray makeHolidaysPerYearJsonArray()
{
    QJsonArray jsonArray;
    // count should be based on how many years we have
    jsonArray.insert(0, QJsonValue{20});
    jsonArray.insert(1, QJsonValue{30});
    return jsonArray;
}

QJsonArray
makePauseTimesPerDayJsonArray(const std::array<QTime, 5> &pauseTimesPerDay)
{
    QJsonArray jsonArray;
    for (auto i = 0; i < pauseTimesPerDay.size(); ++i) {
        QJsonValue jsonValue = timeToString(pauseTimesPerDay[i]);
        jsonArray.insert(i, jsonValue);
    }
    return jsonArray;
}

QJsonArray makeWeeksJsonArray(const QVector<QObject *> &controllerWeeks)
{
    QJsonArray jsonArray;
    for (const auto &controllerWeekAsQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        QJsonObject jsonObject = makeWeekJsonObject(*controllerWeek);
        jsonArray.append(jsonObject);
    }
    return jsonArray;
}

QJsonObject makeWeekJsonObject(const ControllerWeek &controllerWeek)
{
    auto controllerDays = controllerWeek.controllerDays();
    QJsonObject jsonObject;
    QJsonArray jsonArray = makeDaysJsonArray(controllerDays);
    jsonObject["days"] = jsonArray;
    return jsonObject;
}

QJsonArray makeDaysJsonArray(const QVector<QObject *> &controllerDays)
{
    QJsonArray jsonArray;
    for (const auto &controllerDayAsQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);
        QJsonObject jsonObject = makeDayJsonObject(*controllerDay);
        jsonArray.append(jsonObject);
    }
    return jsonArray;
}

QJsonObject makeDayJsonObject(const ControllerDay &controllerDay)
{
    QJsonObject jsonObject;
    jsonObject["date"] = controllerDay.date();
    jsonObject["startTime"] = controllerDay.startTime();
    jsonObject["endTime"] = controllerDay.endTime();
    jsonObject["isHoliday"] = controllerDay.isHoliday();
    jsonObject["isVaccation"] = controllerDay.isVaccation();
    // TODO implement ignore function so weeks are not used for calc the times
    // holidays etc.
    jsonObject["isIgnore"] = false;
    return jsonObject;
}

} // namespace

} // namespace whm
