/* Working Hour Manager
 * Copyright (C) 2022 Sandro Wißmann
 *
 * Working Hour Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Working Hour Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quiz If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/WorkingHourManager
 */
#include "../include/whm/file/FileWriter.hpp"

#include <whm/day/Day.hpp>
#include <whm/year/SettingsYear.hpp>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringLiteral>

namespace whm {

namespace {

QJsonObject makeApplicationDataJsonObject(
    const QVector<std::shared_ptr<Day>> &days,
    const QVector<SettingsYear> &settingsYears);

QJsonArray makeJsonArray(const QVector<std::shared_ptr<Day>> &days);

QJsonObject makeJsonObject(std::shared_ptr<Day> day);

QJsonArray makeJsonArray(const std::array<Time, 5> timeArray);

QJsonArray makeJsonArray(const QVector<SettingsYear> &settingsYears);

QJsonObject makeJsonObject(const SettingsYear &settingYear);

} // namespace

FileWriter::FileWriter(const QString &filename) : m_filename{filename}
{
}

bool FileWriter::writeToFile(
    const QVector<std::shared_ptr<Day>> &days,
    const QVector<SettingsYear> &settingsYears)
{
    QFile saveFile{m_filename};

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << Q_FUNC_INFO << "Couldn't open save file.";
        return false;
    }

    auto jsonObject = makeApplicationDataJsonObject(days, settingsYears);

    saveFile.write(QJsonDocument(jsonObject).toJson());
    return true;
}

namespace {

QJsonObject makeApplicationDataJsonObject(
    const QVector<std::shared_ptr<Day>> &days,
    const QVector<SettingsYear> &settingsYears)
{
    QJsonObject jsonObject;
    jsonObject["days"] = makeJsonArray(days);
    jsonObject["settingsYears"] = makeJsonArray(settingsYears);
    return jsonObject;
}

QJsonArray makeJsonArray(const QVector<std::shared_ptr<Day>> &days)
{
    QJsonArray jsonArray;
    for (const auto &day : days) {
        QJsonObject jsonObject = makeJsonObject(day);
        jsonArray.append(jsonObject);
    }
    return jsonArray;
}

QJsonObject makeJsonObject(std::shared_ptr<Day> day)
{
    QJsonObject jsonObject;
    jsonObject["date"] = day->date().asString();

    // To save space we only save if values are different from the default
    // values
    if (auto startTime = day->startTime(); startTime != Time{}) {
        jsonObject["startTime"] = startTime.asString();
    }
    if (auto endTime = day->endTime(); endTime != Time{}) {
        jsonObject["endTime"] = endTime.asString();
    }
    if (auto dayType = day->dayType(); dayType != DayType::Work) {
        jsonObject["dayType"] = static_cast<int>(dayType);
    }
    return jsonObject;
}

QJsonArray makeJsonArray(const std::array<Time, 5> array)
{
    QJsonArray jsonArray;
    for (auto i = 0; i < array.size(); ++i) {
        QJsonValue jsonValue = array[i].asString();
        jsonArray.insert(i, jsonValue);
    }
    return jsonArray;
}

QJsonArray makeJsonArray(const QVector<SettingsYear> &settingsYears)
{
    QJsonArray jsonArray;
    for (const auto &settingsYear : settingsYears) {
        QJsonObject jsonObject = makeJsonObject(settingsYear);
        jsonArray.append(jsonObject);
    }
    return jsonArray;
}

QJsonObject makeJsonObject(const SettingsYear &settingYear)
{
    QJsonObject jsonObject;
    jsonObject["defaultWorkTimesMoToFr"] =
        makeJsonArray(settingYear.defaultWorkTimesMoToFr());
    jsonObject["pauseTimesMoToFr"] =
        makeJsonArray(settingYear.pauseTimesMoToFr());

    if (!qFuzzyCompare(
            settingYear.flextimeDays(), SettingsYear::defaultFlextimneDays())) {
        jsonObject["flextimeDays"] = settingYear.flextimeDays();
    }
    if (!qFuzzyCompare(
            settingYear.flextimeDays(), SettingsYear::defaultVacationDays())) {
        jsonObject["vacationDays"] = settingYear.vacationDays();
    }
    return jsonObject;
}

} // namespace

} // namespace whm
