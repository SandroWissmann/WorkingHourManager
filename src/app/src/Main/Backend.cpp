#include "Backend.hpp"

#include <whm/types/Date.hpp>
#include <whm/types/Day.hpp>

#include "Controller/ControllerDay.hpp"
#include "Controller/ControllerMonth.hpp"
#include "Controller/ControllerWeek.hpp"
#include "Controller/ControllerYear.hpp"
#include "Controller/ControllerYear/ControllerSettingsYear.hpp"
#include "FileIO/FileReader.hpp"
#include "FileIO/FileWriter.hpp"

#include <QDebug>

#include <map>

namespace whm {

namespace {

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate);

int extractFirstYear(const QVector<std::shared_ptr<Day>> &days);

Date getLastDayOfFirstWeekDate(const QVector<std::shared_ptr<Day>> &days);

Backend makeDefaultBackend();

Backend makeBackendFromFile(const FileReader &fileReader);

QVector<QObject *> makeControllerYears(
    QVector<std::shared_ptr<Day>> days,
    const SettingsYear &settingsYear);

QVector<QObject *> makeControllerDays(
    QVector<std::shared_ptr<Day>> days,
    const SettingsYear &settingsYear);

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
    QObject *controllerSettingsYear);

QVector<std::shared_ptr<Day>>
getDays(const QVector<QObject *> &controllerYears);

} // namespace

Backend::Backend(const QVector<QObject *> &controllerYears, QObject *parent)
    : QObject{parent}, m_controllerYears{controllerYears}
{
    for (auto &controllerYear : m_controllerYears) {
        controllerYear->setParent(this);
    }
}

Backend Backend::fromFile()
{
    FileReader fileReader{"save.json"};

    if (!fileReader.isValidFile()) {
        return makeDefaultBackend();
    }

    return makeBackendFromFile(fileReader);
}

QVector<QObject *> Backend::controllerYears() const
{
    return m_controllerYears;
}

void Backend::saveToFile()
{
    FileWriter fileWriter{"save.json"};

    auto days = getDays(m_controllerYears);

    // TODO save these values for each year
    auto controllerYear =
        qobject_cast<ControllerYear *>(m_controllerYears.at(0));
    auto controllerSettingsYear = qobject_cast<ControllerSettingsYear *>(
        controllerYear->controllerSettinsYear());
    auto settingsYear = controllerSettingsYear->settingsYear();

    std::array<Time, 5> defaultWorkTimesMoToFr{
        settingsYear.defaultWorkTime(Weekday::monday),
        settingsYear.defaultWorkTime(Weekday::tuesday),
        settingsYear.defaultWorkTime(Weekday::wednesday),
        settingsYear.defaultWorkTime(Weekday::thursday),
        settingsYear.defaultWorkTime(Weekday::friday),
    };

    std::array<Time, 5> pauseTimesMoToFr{
        settingsYear.pauseTime(Weekday::monday),
        settingsYear.pauseTime(Weekday::tuesday),
        settingsYear.pauseTime(Weekday::wednesday),
        settingsYear.pauseTime(Weekday::thursday),
        settingsYear.pauseTime(Weekday::friday),
    };

    auto writeOk =
        fileWriter.writeToFile(defaultWorkTimesMoToFr, pauseTimesMoToFr, days);

    if (!writeOk) {
        qWarning() << Q_FUNC_INFO << "Save to file failed.";
    }
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
        date = date.addDays(1);
    }

    // to fill up whole working week.
    while (date.weekday() != Weekday::saturday) {
        auto weekday = date.weekday();

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
        date = date.addDays(1);
    }
    return days;
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

Backend makeDefaultBackend()
{
    Date firstDate{2021, 01, 01};
    auto endDate = Date::currentDate();

    auto days = makeWorkDays(firstDate, endDate);

    auto settingsYear = SettingsYear{};
    auto controllerYears = makeControllerYears(days, settingsYear);

    return Backend{controllerYears};
}

Backend makeBackendFromFile(const FileReader &fileReader)
{
    auto days = fileReader.days();

    if (days.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "days are empty.";
        return makeDefaultBackend();
    }

    if (days.size() % 5 != 0) {
        qWarning() << Q_FUNC_INFO << "days are not multiplication of 5.";
        return makeDefaultBackend();
    }

    auto defaultWorkTimesMoToFr = fileReader.defaultWorkTimesMoToFr();
    auto pauseTimesMoToFr = fileReader.pauseTimesMoToFr();

    std::map<Weekday, SettingsDay> weekdayToSettingsDay{
        {Weekday::monday,
         SettingsDay{
             Weekday::monday, defaultWorkTimesMoToFr[0], pauseTimesMoToFr[0]}},
        {Weekday::tuesday,
         SettingsDay{
             Weekday::tuesday, defaultWorkTimesMoToFr[1], pauseTimesMoToFr[1]}},
        {Weekday::wednesday,
         SettingsDay{
             Weekday::wednesday,
             defaultWorkTimesMoToFr[2],
             pauseTimesMoToFr[2]}},
        {Weekday::thursday,
         SettingsDay{
             Weekday::thursday,
             defaultWorkTimesMoToFr[3],
             pauseTimesMoToFr[3]}},
        {Weekday::friday,
         SettingsDay{
             Weekday::friday, defaultWorkTimesMoToFr[4], pauseTimesMoToFr[4]}}};

    SettingsYear settingsYear{weekdayToSettingsDay};

    auto controllerYears = makeControllerYears(days, settingsYear);

    return Backend{controllerYears};
}

QVector<QObject *> makeControllerYears(
    QVector<std::shared_ptr<Day>> days,
    const SettingsYear &settingsYear)
{
    auto controllerDays = makeControllerDays(days, settingsYear);
    auto controllerWeeks = makeControllerWeeks(controllerDays);

    auto firstYear = extractFirstYear(days);

    auto controllerMonths = makeControllerMonths(firstYear, controllerWeeks);
    auto controllerSettingsYear = new ControllerSettingsYear{settingsYear};
    auto controllerYears =
        makeControllerYears(controllerMonths, controllerSettingsYear);

    return controllerYears;
}

QVector<QObject *> makeControllerDays(
    QVector<std::shared_ptr<Day>> days,
    const SettingsYear &settingsYear)
{
    QVector<QObject *> controllerDays;
    controllerDays.reserve(days.size());
    for (const auto &day : days) {
        auto weekday = day->date().weekday();
        auto pauseTime = settingsYear.pauseTime(weekday);
        auto defaultWorkTime = settingsYear.defaultWorkTime(weekday);
        auto controllerDay = new ControllerDay{day, defaultWorkTime, pauseTime};
        controllerDays.emplace_back(controllerDay);
    }
    return controllerDays;
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
    QObject *controllerSettingsYear)
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

    QVector<QObject *> controllerYears;
    controllerYears.reserve(yearsToControllerMonths.size());
    for (const auto &yearToControllerMonths : yearsToControllerMonths) {
        auto controllerYear = new ControllerYear{
            yearToControllerMonths.second, controllerSettingsYear};
        controllerYears.emplace_back(controllerYear);
    }
    return controllerYears;
}

QVector<std::shared_ptr<Day>> getDays(const QVector<QObject *> &controllerYears)
{
    QVector<std::shared_ptr<Day>> days;

    // get all days from years even the ones from previous and next years
    for (const auto &controllerYearAsQObject : controllerYears) {
        auto controllerYear =
            qobject_cast<ControllerYear *>(controllerYearAsQObject);

        auto yearDays = controllerYear->days();

        days.append(yearDays);
    }

    days.erase(std::unique(days.begin(), days.end()), days.end());
    return days;
}

} // namespace

} // namespace whm
