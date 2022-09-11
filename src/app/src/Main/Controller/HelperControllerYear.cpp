#include "HelperControllerYear.hpp"

#include "ControllerYear.hpp"
#include "ControllerYear/ControllerSettingsYear.hpp"

#include <whm/day/ControllerDay.hpp>
#include <whm/day/Day.hpp>
#include <whm/month/ControllerMonth.hpp>
#include <whm/settings/SettingsYear.hpp>
#include <whm/week/ControllerWeek.hpp>

#include <QDebug>

namespace whm {
void debugPrint(const QVector<QObject *> &controllerYears)
{
    for (const auto &controllerYearAsQObject : controllerYears) {

        auto controllerYear =
            qobject_cast<ControllerYear *>(controllerYearAsQObject);

        qDebug() << "year: " << controllerYear->year();

        auto controllerMonths = controllerYear->controllerMonths();

        for (const auto &controllerMonthAsQObject : controllerMonths) {
            auto controllerMonth =
                qobject_cast<ControllerMonth *>(controllerMonthAsQObject);

            qDebug() << "#Month: " << controllerMonth->month();

            auto controllerWeeks = controllerMonth->controllerWeeks();

            for (const auto &controllerWeekAsQObject : controllerWeeks) {
                auto controllerWeek =
                    qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

                qDebug() << "##Week:";

                auto controllerDays = controllerWeek->controllerDays();

                for (const auto &controllerDaysAsQObject : controllerDays) {
                    auto controllerDay =
                        qobject_cast<ControllerDay *>(controllerDaysAsQObject);

                    qDebug()
                        << "###Day" << controllerDay->day()->date().asString();
                }
            }
        }
    }
}
} // namespace whm

namespace whm {

namespace {
QVector<QObject *> makeControllerDays(
    const QVector<std::shared_ptr<Day>> &days,
    const std::map<int, SettingsYear> &yearsToSettingsYears);

QObject *
makeControllerDay(const SettingsYear &settingsYear, std::shared_ptr<Day> day);

QVector<QObject *>
makeControllerWeeks(const QVector<QObject *> &controllerDays);

int extractFirstYear(const QVector<std::shared_ptr<Day>> &days);

Date getLastDayOfFirstWeekDate(const QVector<std::shared_ptr<Day>> &days);

QVector<QObject *>
makeControllerMonths(int firstYear, const QVector<QObject *> &controllerWeeks);

std::map<int, QVector<QObject *>> controllerWeeksToYearsControllerWeekMap(
    int firstYear,
    const QVector<QObject *> &controllerWeeks);

QVector<QObject *> makeControllerMonths(
    int firstYear,
    const QVector<QVector<QObject *>> &controllerWeeksInYears);

std::map<int, QVector<QObject *>> makeMonthsToControllerWeeks(
    int firstYear,
    const QVector<QObject *> &controllerWeeks);

QVector<QObject *> makeControllerYears(
    const QVector<QObject *> &controllerMonths,
    const std::map<int, QObject *> &yearsToControllerSettingsYears);

} // namespace

QVector<QObject *> makeControllerYears(
    const QVector<std::shared_ptr<Day>> &days,
    const std::map<int, SettingsYear> &yearsToSettingsYears)
{
    auto controllerDays = makeControllerDays(days, yearsToSettingsYears);
    auto controllerWeeks = makeControllerWeeks(controllerDays);

    auto firstYear = extractFirstYear(days);

    auto controllerMonths = makeControllerMonths(firstYear, controllerWeeks);

    std::map<int, QObject *> yearsToControllerSettingsYears;
    for (const auto &[year, settingsYear] : yearsToSettingsYears) {
        auto controllerSettingsYear = new ControllerSettingsYear{settingsYear};
        yearsToControllerSettingsYears.insert({year, controllerSettingsYear});
    }

    auto controllerYears =
        makeControllerYears(controllerMonths, yearsToControllerSettingsYears);

    return controllerYears;
}

namespace {

/*

  - transform vector of SettingsYears into map [Year, SettingsYear]

  - Loop through days and asign each day a SettingYear from the map

  - First days can have year which is not in map so we asign settingYear + 1.
  This is not an issue because we necer use these days for any serious
  calculations.

  - Last days have the same issue and currently use settingYear - 1. This should
  be fixed.

*/
QVector<QObject *> makeControllerDays(
    const QVector<std::shared_ptr<Day>> &days,
    const std::map<int, SettingsYear> &yearsToSettingsYears)
{
    QVector<QObject *> controllerDays;
    controllerDays.reserve(days.size());
    for (const auto &day : days) {
        auto year = day->date().year();

        if (yearsToSettingsYears.find(year) != yearsToSettingsYears.end()) {
            auto controllerDay =
                makeControllerDay(yearsToSettingsYears.at(year), day);
            controllerDays.emplace_back(controllerDay);
        }
        // If first year is 2021 and we have some days from 2020 because of the
        // first week  take defaultWorkTime from 2021. This days wont be used
        // for calculations anyway.
        else if (
            yearsToSettingsYears.find(year + 1) != yearsToSettingsYears.end()) {

            auto controllerDay =
                makeControllerDay(yearsToSettingsYears.at(year + 1), day);
            controllerDays.emplace_back(controllerDay);
        }
        else { // year - 1
            // only years before should have missing setting thats why we fail
            // here
            Q_ASSERT(false == true);
        }
    }
    return controllerDays;
}

/*
  - Get defaultWorkWime from settingsYear

  - Create ControllerDay from day and defaultWorkTime
*/
QObject *
makeControllerDay(const SettingsYear &settingsYear, std::shared_ptr<Day> day)
{
    auto weekday = day->date().weekday();

    auto defaultWorkTime = settingsYear.defaultWorkTime(weekday);
    auto pauseTime = settingsYear.pauseTime(weekday);
    return new ControllerDay{day, defaultWorkTime, pauseTime};
}

/*
    - Construct ControllerWeek from first 5 days

    - Repeat until end of days.
*/
QVector<QObject *> makeControllerWeeks(const QVector<QObject *> &controllerDays)
{
    Q_ASSERT(controllerDays.size() % 5 == 0);

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

/*
    - Create map [year, controllerWeeks] from controllerWeeks of all years.
    We skip here a year < firstYear. Which are the days from first week in
    january which is december of previous year.

    - Turn this into a stupid vector of vector controllerWeeks. So outer
    vector represents year (but we dont know which year anymore). Inner
    vector reprents the controllerWeeks in that year.

    - Create ControllerMonths (Without year information) from it.
*/
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
        makeControllerMonths(firstYear, controllerWeeksInYears);
    return controllerMonths;
}

/*
    - Create map [year, controllerWeeks] from controllerWeeks of all years.
    We skip here a year < firstYear. Which are the days from first week in
    january which is december of previous year.
*/
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
                QVector<QObject *> weeks{controllerWeekAsQObject};
                yearsToControllerWeeks.insert({year, weeks});
            }
            else {
                auto weeks = yearsToControllerWeeks.at(year);
                weeks.emplace_back(controllerWeekAsQObject);
                yearsToControllerWeeks.at(year) = weeks;
            }
        }
    }
    return yearsToControllerWeeks;
}

/*
 - Input is vector of years with  vector of weeks in years. e.g.:

    {week1, week2, week3} // year 1
    {week3, week4, week5} // year 2

    one week can be overlap in two years in january and december

 - Output is flat list of controllerMonths like this

    {jan1999, feb1999 ..... dec1999, jan200 ....}


 - Be aware that controllerWeeks can be in two months at the same time
*/
QVector<QObject *> makeControllerMonths(
    int firstYear,
    const QVector<QVector<QObject *>> &controllerWeeksInYears)
{
    QVector<QObject *> controllerMonths;
    // go over years and order weeks into months
    for (const auto &controllerWeeksInYear : controllerWeeksInYears) {
        auto monthsToControllerWeeks =
            makeMonthsToControllerWeeks(firstYear, controllerWeeksInYear);

        for (const auto &monthToControllerWeeks : monthsToControllerWeeks) {
            auto controllerMonth =
                new ControllerMonth{monthToControllerWeeks.second};
            controllerMonths.emplace_back(controllerMonth);
        }
    }
    return controllerMonths;
}

std::map<int, QVector<QObject *>> makeMonthsToControllerWeeks(
    int firstYear,
    const QVector<QObject *> &controllerWeeks)
{
    std::map<int, QVector<QObject *>> monthsToControllerWeeks;
    for (const auto &controllerWeekAsQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        auto months = controllerWeek->months();
        auto years = controllerWeek->years();

        for (const auto &month : months) {

            // prevent that we add december of year before first year to
            // next year december.
            if (years.size() == 2) {
                if (years[0] < firstYear) {
                    continue;
                }
            }

            if (monthsToControllerWeeks.find(month) ==
                monthsToControllerWeeks.end()) {
                QVector<QObject *> weeks{controllerWeekAsQObject};
                monthsToControllerWeeks.insert({month, weeks});
            }
            else {
                auto weeks = monthsToControllerWeeks.at(month);
                weeks.emplace_back(controllerWeekAsQObject);
                monthsToControllerWeeks.at(month) = weeks;
            }
        }
    }
    return monthsToControllerWeeks;
}

QVector<QObject *> makeControllerYears(
    const QVector<QObject *> &controllerMonths,
    const std::map<int, QObject *> &yearsToControllerSettingsYears)
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

    std::map<int, QObject *> yearToControllerYears;
    for (const auto &[year, controllerSettingsYearAsQObject] :
         yearsToControllerSettingsYears) {
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

} // namespace

} // namespace whm

namespace whm {

namespace {
QVector<std::shared_ptr<Day>>
getDaysFromFirstWeekFirstYearNotInYear(QObject *controllerYearAsQObject)
{
    auto controllerYear =
        qobject_cast<ControllerYear *>(controllerYearAsQObject);
    return controllerYear->daysFromPreviousYearOfFirstWeek();
}

} // namespace

QVector<std::shared_ptr<Day>> getDays(const QVector<QObject *> &controllerYears)
{
    QVector<std::shared_ptr<Day>> days;
    constexpr auto weeksInYear = 52;
    days.reserve(controllerYears.size() * weeksInYear);

    auto daysFromFirstWeekFirstYearNotInYear =
        getDaysFromFirstWeekFirstYearNotInYear(controllerYears.at(0));

    days.append(daysFromFirstWeekFirstYearNotInYear);

    for (const auto &controllerYearAsQObject : controllerYears) {
        auto controllerYear =
            qobject_cast<ControllerYear *>(controllerYearAsQObject);

        auto yearDays = controllerYear->days();
        days.append(yearDays);
    }
    return days;
}
} // namespace whm

namespace whm {
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
} // namespace whm
