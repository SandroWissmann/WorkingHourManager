#include "FileWriter.hpp"

#include <whm/types/Day.hpp>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringLiteral>

namespace whm {

namespace {

QJsonObject makeApplicationDataJsonObject(
    const Time &defaultWorkedTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    const QVector<std::shared_ptr<Day>> &days);

QJsonArray makeHolidaysPerYearJsonArray();

QJsonArray
makePauseTimesPerDayJsonArray(const std::array<Time, 5> &pauseTimesPerDay);

QJsonArray makeDaysJsonArray(const QVector<std::shared_ptr<Day>> &days);

QJsonObject makeDayJsonObject(std::shared_ptr<Day> day);

} // namespace

FileWriter::FileWriter(const QString &filename) : m_filename{filename}
{
}

bool FileWriter::writeToFile(
    const Time &defaultWorkedTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    const QVector<std::shared_ptr<Day>> &days)
{
    QFile saveFile{m_filename};

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << Q_FUNC_INFO << "Couldn't open save file.";
        return false;
    }

    auto jsonObject = makeApplicationDataJsonObject(
        defaultWorkedTimePerDay, pauseTimesPerDay, days);

    saveFile.write(QJsonDocument(jsonObject).toJson());
    return true;
}

namespace {

QJsonObject makeApplicationDataJsonObject(
    const Time &defaultWorkedTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    const QVector<std::shared_ptr<Day>> &days)
{
    QJsonObject jsonObject;
    jsonObject["defaultWorkedTimePerDay"] = defaultWorkedTimePerDay.asString();
    jsonObject["holidaysPerYear"] = makeHolidaysPerYearJsonArray();
    jsonObject["pauseTimesPerDay"] =
        makePauseTimesPerDayJsonArray(pauseTimesPerDay);
    jsonObject["days"] = makeDaysJsonArray(days);
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

QJsonArray makeDaysJsonArray(const QVector<std::shared_ptr<Day>> &days)
{
    QJsonArray jsonArray;
    for (const auto &day : days) {
        QJsonObject jsonObject = makeDayJsonObject(day);
        jsonArray.append(jsonObject);
    }
    return jsonArray;
}

QJsonObject makeDayJsonObject(std::shared_ptr<Day> day)
{
    QJsonObject jsonObject;
    jsonObject["date"] = day->date().asString();

    // To save space we only save if values are different from the default
    // values
    if (auto startTime = day->startTime(); startTime != Time{}) {
        jsonObject["startTime"] = startTime.asString();
    }
    if (auto endTime = day->startTime(); endTime != Time{}) {
        jsonObject["endTime"] = endTime.asString();
    }
    if (auto dayType = day->dayType(); dayType != DayType::work) {
        jsonObject["dayType"] = static_cast<int>(dayType);
    }
    return jsonObject;
}

} // namespace

} // namespace whm
