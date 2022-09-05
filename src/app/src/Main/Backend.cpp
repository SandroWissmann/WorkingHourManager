#include "Backend.hpp"

#include <whm/file/FileReader.hpp>
#include <whm/file/FileWriter.hpp>
#include <whm/types/Date.hpp>
#include <whm/types/Day.hpp>

#include "Controller/ControllerDay.hpp"
#include "Controller/ControllerMonth.hpp"
#include "Controller/ControllerWeek.hpp"
#include "Controller/ControllerYear.hpp"
#include "Controller/ControllerYear/ControllerSettingsYear.hpp"

#include <QDebug>

#include <map>

namespace whm {

namespace {

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate);

void addDayOnWorkday(
    QVector<std::shared_ptr<Day>> &days,
    Weekday weekday,
    const Date &date);

int extractFirstYear(const QVector<std::shared_ptr<Day>> &days);

Date getLastDayOfFirstWeekDate(const QVector<std::shared_ptr<Day>> &days);

QVector<QObject *> makeDefaultControllerYears();

QVector<QObject *> makeControllerYearsFromFile(const FileReader &fileReader);

QVector<QObject *> makeControllerYears(
    QVector<std::shared_ptr<Day>> days,
    const QVector<SettingsYear> &settingsYear);

QObject *
makeControllerDay(const SettingsYear &settingsYear, std::shared_ptr<Day> day);

QVector<QObject *> makeControllerDays(
    QVector<std::shared_ptr<Day>> days,
    const QVector<SettingsYear> &settingsYear);

QVector<QObject *>
makeControllerWeeks(const QVector<QObject *> &controllerDays);

QVector<QObject *>
makeControllerMonths(int firstYear, const QVector<QObject *> &controllerWeeks);

std::map<int, QVector<QObject *>> controllerWeeksToYearsControllerWeekMap(
    int firstYear,
    const QVector<QObject *> &controllerWeeks);

QVector<QObject *>
makeControllerMonths(const QVector<QVector<QObject *>> &controllerWeeksInYears);

QVector<QObject *> makeControllerYears(
    const QVector<QObject *> &controllerMonths,
    const QVector<QObject *> &controllerSettingsYears);

QVector<std::shared_ptr<Day>>
getDays(const QVector<QObject *> &controllerYears);

QVector<SettingsYear>
getSettingsYears(const QVector<QObject *> &controllerYears);

} // namespace

Backend::Backend(QObject *parent) : QObject{parent}
{
}

QVector<QObject *> Backend::controllerYears() const
{
    return m_controllerYears;
}

bool Backend::readControllerYearsFromFile()
{
    FileReader fileReader{"save.json"};

    if (!fileReader.isValidFile()) {
        return false;
    }

    auto controllerYears = makeControllerYearsFromFile(fileReader);
    setControllerYears(controllerYears);
    return true;
}

void Backend::generateControllerYears()
{
    auto controllerYears = makeDefaultControllerYears();
    setControllerYears(controllerYears);
}

void Backend::saveToFile()
{
    if (m_controllerYears.isEmpty()) {
        return;
    }

    FileWriter fileWriter{"save.json"};

    auto days = getDays(m_controllerYears);
    auto settingsYears = getSettingsYears(m_controllerYears);

    auto writeOk = fileWriter.writeToFile(days, settingsYears);

    if (!writeOk) {
        qWarning() << Q_FUNC_INFO << "Save to file failed.";
    }
}

void Backend::setControllerYears(const QVector<QObject *> &controllerYears)
{
    if (m_controllerYears == controllerYears) {
        return;
    }

    for (auto &controllerYears : m_controllerYears) {
        controllerYears->deleteLater();
    }
    m_controllerYears.clear();
    m_controllerYears = controllerYears;
    for (auto &controllerYears : m_controllerYears) {
        controllerYears->setParent(this);
    }
    emit controllerYearsChanged();
}

namespace {

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate)
{
    constexpr int mondayIdx = 1;
    auto date = firstDate.getPreviouseDateWithDayOfWeek(mondayIdx);

    // TODO this way we can end up in the middle of a week. Move forward
    // until friday here. instead of second while loop.
    auto endDate = lastDate;

    QVector<std::shared_ptr<Day>> days;
    while (date <= lastDate) {
        auto weekday = date.weekday();
        addDayOnWorkday(days, weekday, date);
        date = date.addDays(1);
    }

    // to fill up whole working week.
    while (date.weekday() != Weekday::saturday) {
        auto weekday = date.weekday();
        addDayOnWorkday(days, weekday, date);
        date = date.addDays(1);
    }
    return days;
}

void addDayOnWorkday(
    QVector<std::shared_ptr<Day>> &days,
    Weekday weekday,
    const Date &date)
{
    switch (weekday) {
        case Weekday::monday:
        case Weekday::tuesday:
        case Weekday::wednesday:
        case Weekday::thursday:
        case Weekday::friday: {
            auto day = std::make_shared<Day>(date);
            days.emplace_back(day);
        } break;
        case Weekday::saturday:
            break;
        case Weekday::sunday:
            break;
        case Weekday::unknown:
            break;
    }
}

int extractFirstYear(const QVector<std::shared_ptr<Day>> &days)
{
    auto date = getLastDayOfFirstWeekDate(days);
    return date.year();
}

Date getLastDayOfFirstWeekDate(const QVector<std::shared_ptr<Day>> &days)
{
    constexpr auto workDaysCount = 5;
    Q_ASSERT(days.size() >= workDaysCount);

    return days.at(workDaysCount - 1)->date();
}

QVector<QObject *> makeDefaultControllerYears()
{
    Date startDate{2021, 01, 01};
    auto endDate = Date::currentDate();

    auto startYear = startDate.year();
    auto endYear = endDate.year();
    auto countOfYears = endYear - startYear + 1;

    QVector<int> years{countOfYears};
    std::iota(years.begin(), years.end(), startYear);

    auto days = makeWorkDays(startDate, endDate);

    QVector<SettingsYear> settingsYears;
    settingsYears.reserve(years.size());
    for (const auto &year : years) {
        SettingsYear settingsYear{year};
        settingsYears.emplace_back(settingsYear);
    }
    auto controllerYears = makeControllerYears(days, settingsYears);
    return controllerYears;
}

QVector<QObject *> makeControllerYearsFromFile(const FileReader &fileReader)
{
    auto days = fileReader.days();

    Q_ASSERT(!days.isEmpty());
    Q_ASSERT(days.size() % 5 == 0);

    // TODO: If these days have a new year we should generate a new default
    // settingsYear
    auto firstDateNotInFile = days.back()->date();
    firstDateNotInFile.addDays(1);
    auto currentDate = Date::currentDate();
    auto newDaysNotInFile = makeWorkDays(firstDateNotInFile, currentDate);
    days.append(newDaysNotInFile);

    auto settingsYears = fileReader.settingsYears();

    Q_ASSERT(!settingsYears.isEmpty());

    auto controllerYears = makeControllerYears(days, settingsYears);
    return controllerYears;
}

QVector<QObject *> makeControllerYears(
    QVector<std::shared_ptr<Day>> days,
    const QVector<SettingsYear> &settingsYears)
{
    auto controllerDays = makeControllerDays(days, settingsYears);
    auto controllerWeeks = makeControllerWeeks(controllerDays);

    auto firstYear = extractFirstYear(days);

    auto controllerMonths = makeControllerMonths(firstYear, controllerWeeks);

    QVector<QObject *> controllerSettingsYears;
    controllerSettingsYears.reserve(settingsYears.size());
    for (const auto &settingsYear : settingsYears) {
        auto controllerSettingsYear = new ControllerSettingsYear{settingsYear};
        controllerSettingsYears.emplace_back(controllerSettingsYear);
    }

    auto controllerYears =
        makeControllerYears(controllerMonths, controllerSettingsYears);

    return controllerYears;
}

QVector<QObject *> makeControllerDays(
    QVector<std::shared_ptr<Day>> days,
    const QVector<SettingsYear> &settingsYears)
{
    // TODO Maybe we should give this settings directly out as a map
    // Then we can also get rid of year in SettingsYear
    std::map<int, SettingsYear> yearToSettingsYear;
    for (const auto &settingsYear : settingsYears) {
        Q_ASSERT(
            yearToSettingsYear.find(settingsYear.year()) ==
            yearToSettingsYear.end());

        yearToSettingsYear.insert({settingsYear.year(), settingsYear});
    }

    QVector<QObject *> controllerDays;
    controllerDays.reserve(days.size());
    for (const auto &day : days) {
        auto year = day->date().year();

        if (yearToSettingsYear.find(year) != yearToSettingsYear.end()) {
            auto controllerDay =
                makeControllerDay(yearToSettingsYear.at(year), day);
            controllerDays.emplace_back(controllerDay);
        }
        // TODO: This is a hack. If first year is 2021 and we have some days
        // from 2020 because of the first week we try to just taje
        // defaultWorkTime from there.
        else if (
            yearToSettingsYear.find(year - 1) != yearToSettingsYear.end()) {

            auto controllerDay =
                makeControllerDay(yearToSettingsYear.at(year - 1), day);
            controllerDays.emplace_back(controllerDay);
        }
        // TODO Same as before but for last december week
        else if (
            yearToSettingsYear.find(year + 1) != yearToSettingsYear.end()) {

            auto controllerDay =
                makeControllerDay(yearToSettingsYear.at(year + 1), day);
            controllerDays.emplace_back(controllerDay);
        }
    }
    return controllerDays;
}

QObject *
makeControllerDay(const SettingsYear &settingsYear, std::shared_ptr<Day> day)
{
    auto weekday = day->date().weekday();

    auto defaultWorkTime = settingsYear.defaultWorkTime(weekday);
    auto pauseTime = settingsYear.pauseTime(weekday);
    return new ControllerDay{day, defaultWorkTime, pauseTime};
}

QVector<QObject *> makeControllerWeeks(const QVector<QObject *> &controllerDays)
{
    QVector<QObject *> controllerDaysOfWeek;
    constexpr int countOfWeekdays = 5;
    controllerDaysOfWeek.reserve(countOfWeekdays);

    QVector<QObject *> controllerWeeks;
    controllerWeeks.reserve(controllerDays.size() / countOfWeekdays);
    for (const auto &controllerDay : controllerDays) {
        controllerDaysOfWeek.emplace_back(controllerDay);

        if (controllerDaysOfWeek.size() == countOfWeekdays) {
            auto controllerWeek = new ControllerWeek{controllerDaysOfWeek};
            controllerWeeks.emplace_back(controllerWeek);
            controllerDaysOfWeek.clear();
        }
    }
    return controllerWeeks;
}

QVector<QObject *>
makeControllerMonths(int firstYear, const QVector<QObject *> &controllerWeeks)
{
    // Put weeks into years. Some weeks can show up twice because they are
    // sliced between december / january
    std::map<int, QVector<QObject *>> yearsToControllerWeeks =
        controllerWeeksToYearsControllerWeekMap(firstYear, controllerWeeks);

    // transform into flat vector of weeks in years
    QVector<QVector<QObject *>> controllerWeeksInYears;
    controllerWeeksInYears.reserve(yearsToControllerWeeks.size());
    for (const auto &yearToControllerWeeks : yearsToControllerWeeks) {
        controllerWeeksInYears.emplace_back(yearToControllerWeeks.second);
    }

    QVector<QObject *> controllerMonths =
        makeControllerMonths(controllerWeeksInYears);
    return controllerMonths;
}

std::map<int, QVector<QObject *>> controllerWeeksToYearsControllerWeekMap(
    int firstYear,
    const QVector<QObject *> &controllerWeeks)
{
    std::map<int, QVector<QObject *>> yearsToControllerWeeks;
    for (const auto &controllerWeekAsQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        auto years = controllerWeek->years();

        for (const auto &year : years) {

            // do not store a year which is smaller for example some days
            // from december could cause this
            if (year < firstYear) {
                continue;
            }

            if (yearsToControllerWeeks.find(year) ==
                yearsToControllerWeeks.end()) {
                QVector<QObject *> weeks;
                weeks.emplace_back(controllerWeekAsQObject);
                yearsToControllerWeeks[year] = weeks;
            }
            else {
                auto weeks = yearsToControllerWeeks[year];
                weeks.emplace_back(controllerWeekAsQObject);
                yearsToControllerWeeks[year] = weeks;
            }
        }
    }
    return yearsToControllerWeeks;
}

QVector<QObject *>
makeControllerMonths(const QVector<QVector<QObject *>> &controllerWeeksInYears)
{
    QVector<QObject *> controllerMonths;
    // go over years and order weeks into months
    for (const auto &controllerWeeksInYear : controllerWeeksInYears) {
        std::map<int, QVector<QObject *>> monthsToControllerWeeks;
        for (const auto &controllerWeekAsQObject : controllerWeeksInYear) {
            auto controllerWeek =
                qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

            auto months = controllerWeek->months();

            for (const auto &month : months) {
                if (monthsToControllerWeeks.find(month) ==
                    monthsToControllerWeeks.end()) {
                    QVector<QObject *> weeks;
                    weeks.emplace_back(controllerWeekAsQObject);
                    monthsToControllerWeeks[month] = weeks;
                }
                else {
                    auto weeks = monthsToControllerWeeks[month];
                    weeks.emplace_back(controllerWeekAsQObject);
                    monthsToControllerWeeks[month] = weeks;
                }
            }
        }

        for (const auto &monthToControllerWeeks : monthsToControllerWeeks) {
            auto controllerMonth =
                new ControllerMonth{monthToControllerWeeks.second};
            controllerMonths.emplace_back(controllerMonth);
        }
    }
    return controllerMonths;
}

QVector<QObject *> makeControllerYears(
    const QVector<QObject *> &controllerMonths,
    const QVector<QObject *> &controllerSettingsYears)
{
    std::map<int, QVector<QObject *>> yearsToControllerMonths;

    for (const auto &controllerMonthsAsQObject : controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthsAsQObject);

        auto year = controllerMonth->year();

        if (yearsToControllerMonths.find(year) ==
            yearsToControllerMonths.end()) {
            QVector<QObject *> weeks;
            weeks.emplace_back(controllerMonthsAsQObject);
            yearsToControllerMonths[year] = weeks;
        }
        else {
            auto weeks = yearsToControllerMonths[year];
            weeks.emplace_back(controllerMonthsAsQObject);
            yearsToControllerMonths[year] = weeks;
        }
    }

    // TODO maybe we should story controllerSettingsYears directly in a
    // map <year, controllerSettingsYears>
    std::map<int, QObject *> yearToControllerYears;
    for (const auto &controllerSettingsYearAsQObject :
         controllerSettingsYears) {
        auto controllerSettingsYear = qobject_cast<ControllerSettingsYear *>(
            controllerSettingsYearAsQObject);

        auto year = controllerSettingsYear->settingsYear().year();
        Q_ASSERT(
            yearToControllerYears.find(year) == yearToControllerYears.end());
        yearToControllerYears.insert({year, controllerSettingsYearAsQObject});
    }

    QVector<QObject *> controllerYears;
    controllerYears.reserve(yearsToControllerMonths.size());
    for (const auto &yearToControllerMonths : yearsToControllerMonths) {

        auto controllerMonths = yearToControllerMonths.second;
        auto year = yearToControllerMonths.first;
        auto controllerSettingsYear = yearToControllerYears.at(year);

        auto controllerYear =
            new ControllerYear{controllerMonths, controllerSettingsYear};
        controllerYears.emplace_back(controllerYear);
    }
    return controllerYears;
}

QVector<std::shared_ptr<Day>> getDays(const QVector<QObject *> &controllerYears)
{
    QVector<std::shared_ptr<Day>> days;

    for (const auto &controllerYearAsQObject : controllerYears) {
        auto controllerYear =
            qobject_cast<ControllerYear *>(controllerYearAsQObject);

        auto yearDays = controllerYear->days();
        days.append(yearDays);
    }
    return days;
}

QVector<SettingsYear>
getSettingsYears(const QVector<QObject *> &controllerYears)
{
    QVector<SettingsYear> settingsYears;

    for (const auto &controllerYearAsQObject : controllerYears) {
        auto controllerYear =
            qobject_cast<ControllerYear *>(controllerYearAsQObject);

        auto controllerSettinsYearAsQObject =
            controllerYear->controllerSettinsYear();
        auto controllerSetingsYear = qobject_cast<ControllerSettingsYear *>(
            controllerSettinsYearAsQObject);
        auto settingsYear = controllerSetingsYear->settingsYear();

        settingsYears.append(settingsYear);
    }
    return settingsYears;
}

} // namespace

} // namespace whm
