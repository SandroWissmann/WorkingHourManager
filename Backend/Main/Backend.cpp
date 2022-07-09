#include "Backend.hpp"

#include "Backend/ControllerYear.hpp"
#include "Common/ControllerWeek.hpp"
#include "Common/ControllerWeek/ControllerDay.hpp"

#include "Support/Date.hpp"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringLiteral>

namespace whm {

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const Date &firstDate,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent);

QJsonObject makeApplicationDataJsonObject(
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    const QVector<QObject *> &controllerWeeks);

QJsonArray makeHolidaysPerYearJsonArray();

QJsonArray
makePauseTimesPerDayJsonArray(const std::array<Time, 5> &pauseTimesPerDay);

QJsonArray makeWeeksJsonArray(const QVector<QObject *> &controllerWeeks);

QJsonObject makeWeekJsonObject(const ControllerWeek &controllerWeek);

QJsonArray makeDaysJsonArray(const QVector<QObject *> &controllerDays);

QJsonObject makeDayJsonObject(const ControllerDay &controllerDay);

} // namespace

Backend::Backend(
    const Date &firstDate,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
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
    const Date &firstDate,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
{
    constexpr int mondayIdx = 1;
    auto startDate = firstDate.getPreviouseDateWithDayOfWeek(mondayIdx);

    // auto startDate = firstDate;
    auto endDate = Date::currentDate();

    QVector<QObject *> controllerWeeks;
    while (startDate <= endDate) {
        controllerWeeks.emplace_back(new ControllerWeek{
            startDate, defaultWorkTimePerDay, pauseTimesPerDay, parent});
        startDate = startDate.addDays(7);
    }

    return controllerWeeks;
}

QJsonObject makeApplicationDataJsonObject(
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    const QVector<QObject *> &controllerWeeks)
{
    QJsonObject jsonObject;
    jsonObject["defaultWorkTimePerDay"] = defaultWorkTimePerDay.asString();
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
makePauseTimesPerDayJsonArray(const std::array<Time, 5> &pauseTimesPerDay)
{
    QJsonArray jsonArray;
    for (auto i = 0; i < pauseTimesPerDay.size(); ++i) {
        QJsonValue jsonValue = pauseTimesPerDay[i].asString();
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
    jsonObject["date"] = controllerDay.day().date().asString();
    jsonObject["startTime"] = controllerDay.startTimeAsString();
    jsonObject["endTime"] = controllerDay.endTimeAsString();
    jsonObject["isHoliday"] = controllerDay.isHoliday();
    jsonObject["isVacation"] = controllerDay.isVacation();
    // TODO implement ignore function so weeks are not used for calc the times
    // holidays etc.
    jsonObject["isIgnore"] = false;
    return jsonObject;
}

} // namespace

} // namespace whm
