#include "../include/whm/file/FileReader.hpp"

#include <whm/settings/SettingsDay.hpp>
#include <whm/settings/SettingsYear.hpp>

#include <whm/types/Day.hpp>
#include <whm/types/DayType.hpp>
#include <whm/types/Time.hpp>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

#include <optional>

namespace whm {

namespace {

QJsonDocument extractJsonDocumentFromFile(const QString &filename);

QVector<std::shared_ptr<Day>> extractDays(const QJsonDocument &jsonDocument);

std::optional<Date> extractDateFromDay(const QJsonObject &jsonObject);

Time extractStartTimeFromDay(const QJsonObject &jsonObject);

Time extractEndTimeFromDay(const QJsonObject &jsonObject);

Time extractTimeFromDay(const QJsonObject &jsonObject, const QString &key);

DayType extractDayType(const QJsonObject &jsonObject);

int extractIntFromDay(const QJsonObject &jsonObject, const QString &key);

std::map<int, SettingsYear> extractYearsToSettingsYears(
    const QJsonDocument &jsonDocument,
    const QVector<std::shared_ptr<Day>> &days);

QVector<int> extractYears(const QVector<std::shared_ptr<Day>> &days);

std::optional<std::array<Time, 5>>
extractDefaultWorkTimesMoToFr(const QJsonObject &jsonObject);

std::optional<std::array<Time, 5>>
extractPauseTimesMoToFr(const QJsonObject &jsonObject);

std::optional<std::array<Time, 5>>
extractTimeArray(const QJsonObject &jsonObject, const QString &key);

double extractFlextimeDays(const QJsonObject &jsonObject);

double extractVacationDays(const QJsonObject &jsonObject);

std::optional<double>
extractDouble(const QJsonObject &jsonObject, const QString &key);

} // namespace

FileReader::FileReader(const QString &filename)
    : m_jsonDocument{extractJsonDocumentFromFile(filename)},
      m_days{extractDays(m_jsonDocument)},
      m_yearsToSettingsYears{
          extractYearsToSettingsYears(m_jsonDocument, m_days)}
{
}

bool FileReader::isValidFile() const
{
    return !m_jsonDocument.isNull();
}

QVector<std::shared_ptr<Day>> FileReader::days() const
{
    return m_days;
}
std::map<int, SettingsYear> FileReader::yearsToSettingsYears() const
{
    return m_yearsToSettingsYears;
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

QVector<std::shared_ptr<Day>> extractDays(const QJsonDocument &jsonDocument)
{
    if (jsonDocument.isNull()) {
        return {};
    }

    auto jsonObject = jsonDocument.object();
    QString key{"days"};
    if (!jsonObject.contains(key)) {
        return {};
    }
    if (!jsonObject[key].isArray()) {
        return {};
    }

    auto daysArray = jsonObject[key].toArray();

    QVector<std::shared_ptr<Day>> days;
    days.reserve(daysArray.size());
    for (int i = 0; i < daysArray.size(); ++i) {
        auto dayObject = daysArray[i].toObject();
        auto optDate = extractDateFromDay(dayObject);

        if (!optDate) {
            return {};
        }

        auto startTime = extractStartTimeFromDay(dayObject);
        auto endTime = extractEndTimeFromDay(dayObject);
        auto dayType = extractDayType(dayObject);

        auto day = std::make_shared<Day>(*optDate, startTime, endTime, dayType);

        days.emplace_back(day);
    }
    return days;
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

DayType extractDayType(const QJsonObject &jsonObject)
{
    QString key{"dayType"};
    auto dayTypeAsInt = extractIntFromDay(jsonObject, key);

    if (dayTypeAsInt < static_cast<int>(DayType::Work)) {
        return DayType::Work;
    }
    if (dayTypeAsInt > static_cast<int>(DayType::Ignore)) {
        return DayType::Work;
    }
    auto dayType = static_cast<DayType>(dayTypeAsInt);
    return dayType;
}

int extractIntFromDay(const QJsonObject &jsonObject, const QString &key)
{
    if (!jsonObject.contains(key)) {
        return 0;
    }

    auto value = jsonObject.value(key);
    auto variant = value.toVariant();

    auto ok = false;
    auto result = variant.toInt(&ok);

    if (!ok) {
        qDebug() << Q_FUNC_INFO << "Invalid type";
        return 0;
    }
    return result;
}

std::map<int, SettingsYear> extractYearsToSettingsYears(
    const QJsonDocument &jsonDocument,
    const QVector<std::shared_ptr<Day>> &days)
{
    if (jsonDocument.isNull()) {
        return {};
    }

    auto jsonObject = jsonDocument.object();
    QString key{"settingsYears"};
    if (!jsonObject.contains(key)) {
        return {};
    }
    if (!jsonObject[key].isArray()) {
        return {};
    }

    auto years = extractYears(days);
    auto settingsYearArray = jsonObject[key].toArray();

    Q_ASSERT(years.size() == settingsYearArray.size());

    std::map<int, SettingsYear> yearToSettingsYear;
    for (int i = 0; i < settingsYearArray.size(); ++i) {
        auto settingsYearObject = settingsYearArray[i].toObject();
        auto optDefaultWorkTimesMoToFr =
            extractDefaultWorkTimesMoToFr(settingsYearObject);

        if (!optDefaultWorkTimesMoToFr) {
            return {};
        }

        auto optPauseTimesMoToFr = extractPauseTimesMoToFr(settingsYearObject);

        if (!optPauseTimesMoToFr) {
            return {};
        }

        auto defaultWorkTimesMoToFr = optDefaultWorkTimesMoToFr.value();
        auto pauseTimesMoToFr = optPauseTimesMoToFr.value();
        auto flextimeDays = extractFlextimeDays(settingsYearObject);
        auto vacationDays = extractVacationDays(settingsYearObject);

        std::map<Weekday, SettingsDay> weekdayToSettingsDay{
            {Weekday::Monday,
             {Weekday::Monday, defaultWorkTimesMoToFr[0], pauseTimesMoToFr[0]}},
            {Weekday::Tuesday,
             {Weekday::Tuesday,
              defaultWorkTimesMoToFr[1],
              pauseTimesMoToFr[1]}},
            {Weekday::Wednesday,
             {Weekday::Wednesday,
              defaultWorkTimesMoToFr[2],
              pauseTimesMoToFr[2]}},
            {Weekday::Thursday,
             {Weekday::Thursday,
              defaultWorkTimesMoToFr[3],
              pauseTimesMoToFr[3]}},
            {Weekday::Friday,
             {Weekday::Friday, defaultWorkTimesMoToFr[4], pauseTimesMoToFr[4]}},
        };

        SettingsYear settingsYear{
            flextimeDays, vacationDays, weekdayToSettingsDay};

        yearToSettingsYear.insert({years[i], settingsYear});
    }
    return yearToSettingsYear;
}

QVector<int> extractYears(const QVector<std::shared_ptr<Day>> &days)
{
    std::map<int, int> yearsToOccurence;

    for (const auto &day : days) {
        auto year = day->date().year();

        std::map<int, int>::iterator it(yearsToOccurence.find(year));
        if (it != yearsToOccurence.end()) {
            yearsToOccurence[year] = ++yearsToOccurence[year];
        }
        else {
            yearsToOccurence[year] = 1;
        }
    }

    // Filter out year which only comes from days in first week of january
    // which has fill up days from year before
    QVector<int> years;
    for (const auto &[year, occurence] : yearsToOccurence) {
        constexpr int daysFromLastYearInFirstWeekCount = 4;
        if (occurence <= daysFromLastYearInFirstWeekCount) {
            continue;
        }
        years.emplace_back(year);
    }
    return years;
}

std::optional<std::array<Time, 5>>
extractDefaultWorkTimesMoToFr(const QJsonObject &jsonObject)
{
    QString key{"defaultWorkTimesMoToFr"};
    return extractTimeArray(jsonObject, key);
}

std::optional<std::array<Time, 5>>
extractPauseTimesMoToFr(const QJsonObject &jsonObject)
{
    QString key{"pauseTimesMoToFr"};
    return extractTimeArray(jsonObject, key);
}

std::optional<std::array<Time, 5>>
extractTimeArray(const QJsonObject &jsonObject, const QString &key)
{
    if (!jsonObject.contains(key)) {
        return {};
    }
    if (!jsonObject[key].isArray()) {
        return {};
    }

    auto jsonArray = jsonObject[key].toArray();
    if (jsonArray.size() != 5) {
        return {};
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

double extractFlextimeDays(const QJsonObject &jsonObject)
{
    QString key{"flextimeDays"};
    auto optFlextimeDays = extractDouble(jsonObject, key);
    if (optFlextimeDays.has_value()) {
        return optFlextimeDays.value();
    }
    // Default values do not get saved so we get them from SettingsYear
    return SettingsYear::defaultFlextimneDays();
}

double extractVacationDays(const QJsonObject &jsonObject)
{
    QString key{"vacationDays"};
    auto optVacationDays = extractDouble(jsonObject, key);
    if (optVacationDays.has_value()) {
        return optVacationDays.value();
    }
    // Default values do not get saved so we get them from SettingsYear
    return SettingsYear::defaultVacationDays();
}

std::optional<double>
extractDouble(const QJsonObject &jsonObject, const QString &key)
{
    if (!jsonObject.contains(key)) {
        return {};
    }
    if (!jsonObject[key].isDouble()) {
        return {};
    }
    auto value = jsonObject[key].toDouble();
    return {value};
}

} // namespace

} // namespace whm
