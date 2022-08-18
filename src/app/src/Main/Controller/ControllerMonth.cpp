#include "ControllerMonth.hpp"

#include "ControllerWeek.hpp"

#include <whm/types/Day.hpp>

#include <algorithm>
#include <map>

namespace whm {

namespace {

QVector<std::shared_ptr<Day>>
allDaysInWeeks(const QVector<QObject *> &m_controllerWeeks);

int getCurrentMonth(const QVector<std::shared_ptr<Day>> &days);

int getCurrentYear(const QVector<std::shared_ptr<Day>> &days);
} // namespace

ControllerMonth::ControllerMonth(const QVector<QObject *> &controllerWeeks)
    : m_controllerWeeks{controllerWeeks}
{
    Q_ASSERT(controllerWeeks.size() >= 1 && controllerWeeks.size());

    for (auto &controllerWeek : m_controllerWeeks) {
        controllerWeek->setParent(this);
    }
}

QVector<QObject *> ControllerMonth::controllerWeeks() const
{
    return m_controllerWeeks;
}

QVector<std::shared_ptr<Day>> ControllerMonth::days() const
{
    auto days = allDaysInWeeks(m_controllerWeeks);
    auto month = getCurrentMonth(days);

    // erase all days which are int the previous or next month
    days.erase(
        std::remove_if(
            days.begin(),
            days.end(),
            [month](const auto &day) { return month != day->date().month(); }),
        days.end());
    return days;
}

int ControllerMonth::month() const
{
    auto days = allDaysInWeeks(m_controllerWeeks);
    return getCurrentMonth(days);
}

QString ControllerMonth::nameOfMonth() const
{
    // This is a trick to transform month() int into its name by construct a
    // date with not matter year and day and only output the name of month
    QDate date{2000, month(), 1};
    return date.toString("MMMM");
}

// same code as month except the function call, maybe genralize
int ControllerMonth::year() const
{
    auto days = allDaysInWeeks(m_controllerWeeks);
    return getCurrentYear(days);
}

namespace {

QVector<std::shared_ptr<Day>>
allDaysInWeeks(const QVector<QObject *> &m_controllerWeeks)
{
    QVector<std::shared_ptr<Day>> days;
    // get all days from weeks even the ones from previous and next month
    for (const auto &controllerWeekAsQObject : m_controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        auto weekDays = controllerWeek->days();

        days.append(weekDays);
    }
    return days;
}

int getCurrentMonth(const QVector<std::shared_ptr<Day>> &days)
{
    int count = 0;
    int currentMonth = -1;
    std::map<int, int> monthToCount;
    for (const auto &day : days) {
        auto month = day->date().month();

        monthToCount[month]++;
        if (monthToCount[month] > count) {
            count = monthToCount[month];
            currentMonth = month;
        }
    }
    return currentMonth;
}

// almost same code as month maybe generalize?
int getCurrentYear(const QVector<std::shared_ptr<Day>> &days)
{
    int count = 0;
    int currentYear = -1;
    std::map<int, int> yearToCount;
    for (const auto &day : days) {
        auto year = day->date().year();

        yearToCount[year]++;
        if (yearToCount[year] > count) {
            count = yearToCount[year];
            currentYear = year;
        }
    }
    return currentYear;
}

} // namespace

} // namespace whm
