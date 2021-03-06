#include "FileReader.hpp"

#include <whm/types/Day.hpp>
#include <whm/types/Time.hpp>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

#include <optional>

namespace whm {

namespace {
QJsonDocument extractJsonDocumentFromFile(const QString &filename);

std::optional<Date> extractDateFromDay(const QJsonObject &jsonObject);

Time extractStartTimeFromDay(const QJsonObject &jsonObject);

Time extractEndTimeFromDay(const QJsonObject &jsonObject);

Time extractTimeFromDay(const QJsonObject &jsonObject, const QString &key);

bool extractIsHolidayFromDay(const QJsonObject &jsonObject);

bool extractIsVacationFromDay(const QJsonObject &jsonObject);

bool extractIsIgnoreFromDay(const QJsonObject &jsonObject);

bool extractBoolFromDay(const QJsonObject &jsonObject, const QString &key);

} // namespace

FileReader::FileReader(const QString &filename)
    : m_jsonDocument{extractJsonDocumentFromFile(filename)}
{
}

bool FileReader::isValidFile() const
{
    return !m_jsonDocument.isNull();
}

Time FileReader::defaultWorkTimePerDay() const
{
    if (!isValidFile()) {
        return Time{};
    }

    auto jsonObject = m_jsonDocument.object();

    QString key{"defaultWorkTimePerDay"};
    if (!jsonObject.contains(key)) {
        return Time{};
    }
    if (!jsonObject[key].isString()) {
        return Time{};
    }

    auto timeString = jsonObject[key].toString();
    return Time{timeString};
}

QVector<int> FileReader::holidaysPerYear() const
{
    if (!isValidFile()) {
        return QVector<int>{};
    }

    auto jsonObject = m_jsonDocument.object();

    QString key{"holidaysPerYear"};
    if (!jsonObject.contains(key)) {
        return QVector<int>{};
    }
    if (!jsonObject[key].isArray()) {
        return QVector<int>{};
    }

    auto jsonArray = jsonObject[key].toArray();
    QVector<int> holidaysPerYear;
    holidaysPerYear.reserve(jsonArray.size());

    for (int i = 0; i < jsonArray.size(); ++i) {
        if (!jsonArray[i].isDouble()) {
            return QVector<int>{};
        }
        auto value = jsonArray[i].toInt();
        holidaysPerYear.emplace_back(value);
    }
    return holidaysPerYear;
}

std::array<Time, 5> FileReader::pauseTimesPerDay() const
{
    if (!isValidFile()) {
        return std::array<Time, 5>{};
    }

    auto jsonObject = m_jsonDocument.object();

    QString key{"pauseTimesPerDay"};
    if (!jsonObject.contains(key)) {
        return std::array<Time, 5>{};
    }
    if (!jsonObject[key].isArray()) {
        return std::array<Time, 5>{};
    }

    auto jsonArray = jsonObject[key].toArray();
    if (jsonArray.size() != 5) {
        return std::array<Time, 5>{};
    }

    for (int i = 0; i < jsonArray.size(); ++i) {
        if (!jsonArray[i].isString()) {
            return std::array<Time, 5>{};
        }
    }
    return std::array<Time, 5>{
        Time{jsonArray[0].toString()},
        Time{jsonArray[1].toString()},
        Time{jsonArray[2].toString()},
        Time{jsonArray[3].toString()},
        Time{jsonArray[4].toString()}};
}

QVector<std::shared_ptr<Day>> FileReader::days() const
{
    if (!isValidFile()) {
        return {};
    }

    auto jsonObject = m_jsonDocument.object();
    QString dayKey{"days"};
    if (!jsonObject.contains(dayKey)) {
        return {};
    }
    if (!jsonObject[dayKey].isArray()) {
        return {};
    }

    auto daysArray = jsonObject[dayKey].toArray();

    QVector<std::shared_ptr<Day>> days;
    days.reserve(daysArray.size());
    for (int j = 0; j < daysArray.size(); ++j) {
        auto dayObject = daysArray[j].toObject();
        auto optDate = extractDateFromDay(dayObject);

        if (!optDate) {
            return {};
        }

        auto startTime = extractStartTimeFromDay(dayObject);
        auto endTime = extractEndTimeFromDay(dayObject);
        auto isHoliday = extractIsHolidayFromDay(dayObject);
        auto isVaccation = extractIsVacationFromDay(dayObject);

        auto day = std::make_shared<Day>(
            *optDate, startTime, endTime, isHoliday, isVaccation);

        days.emplace_back(day);
    }
    return days;
}

namespace {

QJsonDocument extractJsonDocumentFromFile(const QString &filename)
{
    QFile loadFile{filename};

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return QJsonDocument{};
    }

    QByteArray fileData = loadFile.readAll();

    return QJsonDocument::fromJson(fileData);
}

std::optional<Date> extractDateFromDay(const QJsonObject &jsonObject)
{
    QString key{"date"};

    if (!jsonObject.contains(key)) {
        return {};
    }
    if (!jsonObject[key].isString()) {
        return {};
    }

    auto dateString = jsonObject[key].toString();

    return {Date{dateString}};
}

Time extractStartTimeFromDay(const QJsonObject &jsonObject)
{
    QString key{"startTime"};
    return extractTimeFromDay(jsonObject, key);
}

Time extractEndTimeFromDay(const QJsonObject &jsonObject)
{
    QString key{"endTime"};
    return extractTimeFromDay(jsonObject, key);
}

Time extractTimeFromDay(const QJsonObject &jsonObject, const QString &key)
{
    if (!jsonObject.contains(key)) {
        return Time{};
    }
    if (!jsonObject[key].isString()) {
        qDebug() << Q_FUNC_INFO << "Invalid type";
        return Time{};
    }

    auto timeString = jsonObject[key].toString();
    return Time{timeString};
}

bool extractIsHolidayFromDay(const QJsonObject &jsonObject)
{
    QString key{"isHoliday"};
    return extractBoolFromDay(jsonObject, key);
}

bool extractIsVacationFromDay(const QJsonObject &jsonObject)
{
    QString key{"isVacation"};
    return extractBoolFromDay(jsonObject, key);
}

bool extractIsIgnoreFromDay(const QJsonObject &jsonObject)
{
    QString key{"isIgnore"};
    return extractBoolFromDay(jsonObject, key);
}

bool extractBoolFromDay(const QJsonObject &jsonObject, const QString &key)
{
    if (!jsonObject.contains(key)) {
        return false;
    }
    if (!jsonObject[key].isBool()) {
        qDebug() << Q_FUNC_INFO << "Invalid type";
        return false;
    }

    return jsonObject[key].toBool();
}

} // namespace

} // namespace whm
