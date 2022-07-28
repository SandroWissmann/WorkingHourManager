#include "Backend.hpp"

#include <whm/types/Date.hpp>
#include <whm/types/Day.hpp>

#include "Controller/ControllerDay.hpp"
#include "Controller/ControllerMonth.hpp"
#include "Controller/ControllerWeek.hpp"
#include "Controller/ControllerYear.hpp"
#include "FileIO/FileReader.hpp"
#include "FileIO/FileWriter.hpp"

#include <QDebug>

#include <map>

namespace whm {

namespace {

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate);

Backend makeDefaultBackend();

Backend makeBackendFromFile(const FileReader &fileReader);

QVector<QObject *> makeControllerYears(
    QVector<std::shared_ptr<Day>> days,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay);

QVector<QObject *> makeControllerDays(
    QVector<std::shared_ptr<Day>> days,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay);

QVector<QObject *>
makeControllerWeeks(const QVector<QObject *> &controllerDays);

QVector<QObject *>
makeControllerMonths(const QVector<QObject *> &controllerWeeks);

QVector<QObject *>
makeControllerYears(const QVector<QObject *> &controllerMonths);

QVector<std::shared_ptr<Day>>
getDays(const QVector<QObject *> &controllerYears);

} // namespace

Backend::Backend(
    const QVector<QObject *> &controllerYears,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
    : QObject{parent}, m_defaultWorkTimePerDay{defaultWorkTimePerDay},
      m_pauseTimesPerDay{pauseTimesPerDay}, m_controllerYears{controllerYears}
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

    auto writeOk = fileWriter.writeToFile(
        m_defaultWorkTimePerDay, m_pauseTimesPerDay, days);

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
        if (weekday != "Saturday" && weekday != "Sunday") {
            auto day = std::make_shared<Day>(date);
            days.emplace_back(day);
        }
        date.addDays(1);
    }

    // to fill up whole working week.
    while (date.weekday() != "Saturday") {
        auto weekday = date.weekday();
        if (weekday != "Saturday" && weekday != "Sunday") {
            auto day = std::make_shared<Day>(date);
            days.emplace_back(day);
        }
        date.addDays(1);
    }
    return days;
}

Backend makeDefaultBackend()
{
    Date firstDate{2021, 01, 01};
    Time defaultWorkTimePerDay{7, 45};
    Time pauseTimeMondayToThursday{0, 45};
    Time pauseTimeFriday{0, 30};

    std::array<Time, 5> pauseTimesPerDay{
        pauseTimeMondayToThursday,
        pauseTimeMondayToThursday,
        pauseTimeMondayToThursday,
        pauseTimeMondayToThursday,
        pauseTimeFriday};

    auto endDate = Date::currentDate();

    auto days = makeWorkDays(firstDate, endDate);

    auto controllerYears =
        makeControllerYears(days, defaultWorkTimePerDay, pauseTimesPerDay);

    return Backend{controllerYears, defaultWorkTimePerDay, pauseTimesPerDay};
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

    auto defaultWorkTimePerDay = fileReader.defaultWorkTimePerDay();
    auto pauseTimesPerDay = fileReader.pauseTimesPerDay();

    auto controllerYears =
        makeControllerYears(days, defaultWorkTimePerDay, pauseTimesPerDay);

    return Backend{controllerYears, defaultWorkTimePerDay, pauseTimesPerDay};
}

QVector<QObject *> makeControllerYears(
    QVector<std::shared_ptr<Day>> days,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay)
{
    auto controllerDays =
        makeControllerDays(days, defaultWorkTimePerDay, pauseTimesPerDay);
    auto controllerWeeks = makeControllerWeeks(controllerDays);
    auto controllerMonths = makeControllerMonths(controllerWeeks);
    auto controllerYears = makeControllerYears(controllerMonths);
    return controllerYears;
}

QVector<QObject *> makeControllerDays(
    QVector<std::shared_ptr<Day>> days,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay)
{
    std::map<QString, Time> weekdayToPauseTime{
        {"Monday", pauseTimesPerDay[0]},
        {"Tuesday", pauseTimesPerDay[1]},
        {"Wednesday", pauseTimesPerDay[2]},
        {"Thursday", pauseTimesPerDay[3]},
        {"Friday", pauseTimesPerDay[4]},
    };

    QVector<QObject *> controllerDays;
    controllerDays.reserve(days.size());

    for (const auto &day : days) {

        auto weekday = day->date().weekday();
        auto pauseTime = weekdayToPauseTime[weekday];
        auto controllerDay =
            new ControllerDay{day, defaultWorkTimePerDay, pauseTime};
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
makeControllerMonths(const QVector<QObject *> &controllerWeeks)
{
    // Put weeks into years. Some weeks can show up twice because they are
    // sliced between december / january
    std::map<int, QVector<QObject *>> yearsToControllerWeeks;
    for (const auto &controllerWeekAsQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        auto years = controllerWeek->years();

        for (const auto &year : years) {
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

    // transform into flat vector of weeks in years
    QVector<QVector<QObject *>> controllerWeeksInYears;
    controllerWeeksInYears.reserve(yearsToControllerWeeks.size());
    for (const auto &yearToControllerWeeks : yearsToControllerWeeks) {
        controllerWeeksInYears.emplace_back(yearToControllerWeeks.second);
    }

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

            auto outputMonth = qobject_cast<ControllerMonth *>(controllerMonth);

            qDebug() << "controller month. year: [" << outputMonth->year()
                     << "] month: [" << outputMonth->month()
                     << "] weeksCount : ["
                     << outputMonth->controllerWeeks().size();

            controllerMonths.emplace_back(controllerMonth);
        }
    }
    return controllerMonths;
}

QVector<QObject *>
makeControllerYears(const QVector<QObject *> &controllerMonths)
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
        auto controllerYear = new ControllerYear{yearToControllerMonths.second};
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
            qobject_cast<ControllerWeek *>(controllerYearAsQObject);

        auto yearDays = controllerYear->days();

        days.append(yearDays);
    }

    days.erase(std::unique(days.begin(), days.end()), days.end());
    return days;
}

} // namespace

} // namespace whm
