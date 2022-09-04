#include "ControllerMonth.hpp"

#include "ControllerDay.hpp"
#include "ControllerWeek.hpp"

#include <whm/types/Day.hpp>

#include <algorithm>
#include <map>

namespace whm {

namespace {

QVector<std::shared_ptr<Day>>
allDaysInControllerWeeks(const QVector<QObject *> &m_controllerWeeks);

int getCurrentMonth(const QVector<std::shared_ptr<Day>> &days);

int getCurrentYear(const QVector<std::shared_ptr<Day>> &days);

HoursAndMinutes
calculateOvertime(const QVector<QObject *> controllerWeeks, int month);

double
calculateUsedFlextimeDays(const QVector<QObject *> controllerWeeks, int month);
double
calculateUsedVacationDays(const QVector<QObject *> controllerWeeks, int month);
} // namespace

ControllerMonth::ControllerMonth(const QVector<QObject *> &controllerWeeks)
    : m_controllerWeeks{controllerWeeks},
      m_overtime{calculateOvertime(m_controllerWeeks, month())},
      m_usedFlextimeDays{calculateUsedFlextimeDays(m_controllerWeeks, month())},
      m_usedVacationDays{calculateUsedVacationDays(m_controllerWeeks, month())}
{
    Q_ASSERT(controllerWeeks.size() >= 1 && controllerWeeks.size());

    for (auto &controllerWeek : m_controllerWeeks) {
        controllerWeek->setParent(this);
    }

    makeControllerWeeksToThisConnections();
}

QVector<QObject *> ControllerMonth::controllerWeeks() const
{
    return m_controllerWeeks;
}

QVector<QObject *> ControllerMonth::controllerDays() const
{
    QVector<QObject *> controllerDaysInMonth;
    constexpr int workdaysInWeek = 5;
    controllerDaysInMonth.reserve(m_controllerWeeks.size() * workdaysInWeek);

    for (const auto &controllerWeekAsQObject : m_controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        auto controllerDays = controllerWeek->controllerDays();
        controllerDaysInMonth.append(controllerDays);
    }

    auto month = this->month();

    // erase all controller days which are in the previous or next month
    controllerDaysInMonth.erase(
        std::remove_if(
            controllerDaysInMonth.begin(),
            controllerDaysInMonth.end(),
            [month](const auto &controllerDayInMonth) {
                auto controllerDay =
                    qobject_cast<ControllerDay *>(controllerDayInMonth);

                return month != controllerDay->day()->date().month();
            }),
        controllerDaysInMonth.end());
    return controllerDaysInMonth;
}

QString ControllerMonth::nameOfMonth() const
{
    // This is a trick to transform month() int into its name by construct a
    // date with not matter year and day and only output the name of month
    QDate date{2000, month(), 1};
    return date.toString("MMMM");
}

HoursAndMinutes ControllerMonth::overtime() const
{
    return m_overtime;
}

QString ControllerMonth::overtimeAsString() const
{
    return m_overtime.toString();
}

double ControllerMonth::usedFlextimeDays() const
{
    return m_usedFlextimeDays;
}

double ControllerMonth::usedVacationDays() const
{
    return m_usedVacationDays;
}

QVector<std::shared_ptr<Day>> ControllerMonth::days() const
{
    auto days = allDaysInControllerWeeks(m_controllerWeeks);
    auto month = getCurrentMonth(days);

    // erase all days which are in the previous or next month
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
    auto days = allDaysInControllerWeeks(m_controllerWeeks);
    return getCurrentMonth(days);
}

// same code as month except the function call, maybe genralize
int ControllerMonth::year() const
{
    auto days = allDaysInControllerWeeks(m_controllerWeeks);
    return getCurrentYear(days);
}

void ControllerMonth::onOvertimeOfWeekChanged()
{
    auto overtime = calculateOvertime(m_controllerWeeks, month());
    setOvertime(overtime);
}

void ControllerMonth::onUsedFlextimeDaysOfWeekChanged()
{
    auto usedFlextimeDays =
        calculateUsedFlextimeDays(m_controllerWeeks, month());
    setUsedFlextimeDays(usedFlextimeDays);
}

void ControllerMonth::onUsedVacationDaysOfWeekChanged()
{
    auto usedVacationDays =
        calculateUsedVacationDays(m_controllerWeeks, month());
    setUsedVacationDays(usedVacationDays);
}

void ControllerMonth::makeControllerWeeksToThisConnections() const
{
    for (const auto &controllerWeekQObject : m_controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekQObject);
        connect(
            controllerWeek,
            &ControllerWeek::overtimeChanged,
            this,
            &ControllerMonth::onOvertimeOfWeekChanged);
        connect(
            controllerWeek,
            &ControllerWeek::usedFlextimeDaysChanged,
            this,
            &ControllerMonth::onUsedFlextimeDaysOfWeekChanged);
        connect(
            controllerWeek,
            &ControllerWeek::usedVacationDaysChanged,
            this,
            &ControllerMonth::onUsedVacationDaysOfWeekChanged);
    }
}

void ControllerMonth::setOvertime(const HoursAndMinutes &overtime)
{
    if (m_overtime == overtime) {
        return;
    }
    m_overtime = overtime;
    emit overtimeChanged();
}

void ControllerMonth::setUsedFlextimeDays(double usedFlextimeDays)
{
    if (m_usedFlextimeDays == usedFlextimeDays) {
        return;
    }
    m_usedFlextimeDays = usedFlextimeDays;
    emit usedFlextimeDaysChanged();
}

void ControllerMonth::setUsedVacationDays(double usedVacationDays)
{
    if (m_usedVacationDays == usedVacationDays) {
        return;
    }
    m_usedVacationDays = usedVacationDays;
    emit usedVacationDaysChanged();
}

namespace {

QVector<std::shared_ptr<Day>>
allDaysInControllerWeeks(const QVector<QObject *> &m_controllerWeeks)
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

HoursAndMinutes
calculateOvertime(const QVector<QObject *> controllerWeeks, int month)
{
    HoursAndMinutes overtimeOfMonth;
    for (const auto &controllerWeekQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekQObject);

        auto monthsToOvertime = controllerWeek->monthsToOvertime();

        Q_ASSERT(monthsToOvertime.find(month) != monthsToOvertime.end());

        overtimeOfMonth += monthsToOvertime[month];
    }
    return overtimeOfMonth;
}

double
calculateUsedFlextimeDays(const QVector<QObject *> controllerWeeks, int month)
{
    double usedFlextimeDaysInMonth;
    for (const auto &controllerWeekQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekQObject);

        auto monthsToUsedFlextimeDays =
            controllerWeek->monthsToUsedFlextimeDays();

        Q_ASSERT(
            monthsToUsedFlextimeDays.find(month) !=
            monthsToUsedFlextimeDays.end());

        usedFlextimeDaysInMonth += monthsToUsedFlextimeDays[month];
    }
    return usedFlextimeDaysInMonth;
}

double
calculateUsedVacationDays(const QVector<QObject *> controllerWeeks, int month)
{
    double usedVacationDaysInMonth;
    for (const auto &controllerWeekQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekQObject);

        auto monthsToUsedVacationDays =
            controllerWeek->monthsToUsedVacationDays();

        Q_ASSERT(
            monthsToUsedVacationDays.find(month) !=
            monthsToUsedVacationDays.end());

        usedVacationDaysInMonth += monthsToUsedVacationDays[month];
    }
    return usedVacationDaysInMonth;
}

} // namespace

} // namespace whm
