#include "ControllerYear.hpp"

#include <whm/day/ControllerDay.hpp>
#include <whm/day/ControllerSettingsDay.hpp>
#include <whm/day/Day.hpp>
#include <whm/month/ControllerMonth.hpp>
#include <whm/week/ControllerWeek.hpp>

#include "ControllerYear/ControllerSettingsYear.hpp"

#include <algorithm>

namespace whm {

namespace {
HoursAndMinutes calculateOvertime(const QVector<QObject *> controllerMonths)
{
    HoursAndMinutes overtimeOfYear;
    for (const auto &controllerMonthQObject : controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthQObject);

        auto overtime = controllerMonth->overtime();
        overtimeOfYear += overtime;
    }
    return overtimeOfYear;
}

double calculateUsedFlextimeDays(const QVector<QObject *> controllerMonths)
{
    double usedFlextimeDaysInYear{};
    for (const auto &controllerMonthQObject : controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthQObject);

        auto usedFlextimeDays = controllerMonth->usedFlextimeDays();
        usedFlextimeDaysInYear += usedFlextimeDays;
    }
    return usedFlextimeDaysInYear;
}

double calculateUsedVacationDays(const QVector<QObject *> controllerMonths)
{
    double usedVacationDaysInYear{};
    for (const auto &controllerMonthQObject : controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthQObject);

        auto usedVacationDays = controllerMonth->usedVacationDays();
        usedVacationDaysInYear += usedVacationDays;
    }
    return usedVacationDaysInYear;
}

} // namespace

ControllerYear::ControllerYear(
    const QVector<QObject *> &controllerMonths,
    QObject *controllerSettingsYear)
    : m_controllerMonths{controllerMonths},
      m_controllerSettingsYear{controllerSettingsYear},
      m_overtime{calculateOvertime(m_controllerMonths)},
      m_usedFlextimeDays{calculateUsedFlextimeDays(m_controllerMonths)},
      m_usedVacationDays{calculateUsedVacationDays(m_controllerMonths)}
{
    m_controllerSettingsYear->setParent(this);
    for (auto &controllerMonth : m_controllerMonths) {
        controllerMonth->setParent(this);
    }
    makeControllerMonthsToThisConnections();
    makeControllerSettingsYearToControllerDaysConnections();
}

QVector<QObject *> ControllerYear::controllerMonths() const
{
    return m_controllerMonths;
}

QObject *ControllerYear::controllerSettinsYear() const
{
    return m_controllerSettingsYear;
}

int ControllerYear::year() const
{
    Q_ASSERT(!m_controllerMonths.isEmpty());

    auto controllerMonth =
        qobject_cast<ControllerMonth *>(m_controllerMonths[0]);

    return controllerMonth->year();
}

QString ControllerYear::overtimeAsString() const
{
    return m_overtime.toString();
}

QString ControllerYear::usedFlextimeDaysAsString() const
{
    return QString::number(m_usedFlextimeDays, 'f', 1);
}

QString ControllerYear::usedVacationDaysAsString() const
{
    return QString::number(m_usedVacationDays, 'f', 1);
}

QVector<std::shared_ptr<Day>>
ControllerYear::daysFromPreviousYearOfFirstWeek() const
{
    auto controllerMonthAsQObject = m_controllerMonths.at(0);
    auto controllerMonth =
        qobject_cast<ControllerMonth *>(controllerMonthAsQObject);
    auto controllerWeekAsQObject = controllerMonth->controllerWeeks().at(0);
    auto controllerWeek =
        qobject_cast<ControllerWeek *>(controllerWeekAsQObject);
    auto yearsToDays = controllerWeek->yearsToDays();

    Q_ASSERT(yearsToDays.find(year()) != yearsToDays.end());
    Q_ASSERT(yearsToDays.size() > 0);
    Q_ASSERT(yearsToDays.size() <= 2);

    // first week is still only in this year so no days from previous year
    if (yearsToDays.size() == 1) {
        return QVector<std::shared_ptr<Day>>{};
    }

    for (const auto &[year, days] : yearsToDays) {
        if (year != this->year()) {
            return days;
        }
    }
    // Case should never be hit. Q_ASSERTS on top should catch this
    return QVector<std::shared_ptr<Day>>{};
}

QVector<std::shared_ptr<Day>> ControllerYear::days() const
{
    QVector<std::shared_ptr<Day>> days;
    constexpr int averageDaysPerMont = 30;
    days.reserve(m_controllerMonths.size() * averageDaysPerMont);

    for (const auto &controllerMonthAsQObject : m_controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthAsQObject);

        auto daysInMonth = controllerMonth->days();
        days.append(daysInMonth);
    }
    return days;
}

void ControllerYear::onOvertimeOfMonthChanged()
{
    auto overtime = calculateOvertime(m_controllerMonths);
    setOvertime(overtime);
}

void ControllerYear::onUsedFlextimeDaysOfMonthChanged()
{
    auto usedFlextimeDays = calculateUsedFlextimeDays(m_controllerMonths);
    setUsedFlextimeDays(usedFlextimeDays);
}

void ControllerYear::onUsedVacationDaysOfMonthChanged()
{
    auto usedVacationDays = calculateUsedVacationDays(m_controllerMonths);
    setUsedVacationDays(usedVacationDays);
}

QVector<QObject *> ControllerYear::controllerDays() const
{
    QVector<QObject *> controllerDaysInYear;
    constexpr int daysInMonth = 31; // approx
    controllerDaysInYear.reserve(m_controllerMonths.size() * daysInMonth);

    for (const auto &controllerMonthAsQObject : m_controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthAsQObject);

        auto controllerDays = controllerMonth->controllerDays();
        controllerDaysInYear.append(controllerDays);
    }
    return controllerDaysInYear;
}

void ControllerYear::makeControllerMonthsToThisConnections() const
{
    for (const auto &controllerMonthQObject : m_controllerMonths) {
        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthQObject);
        connect(
            controllerMonth,
            &ControllerMonth::overtimeChanged,
            this,
            &ControllerYear::onOvertimeOfMonthChanged);

        connect(
            controllerMonth,
            &ControllerMonth::usedFlextimeDaysChanged,
            this,
            &ControllerYear::onUsedFlextimeDaysOfMonthChanged);

        connect(
            controllerMonth,
            &ControllerMonth::usedVacationDaysChanged,
            this,
            &ControllerYear::onUsedVacationDaysOfMonthChanged);
    }
}

void ControllerYear::makeControllerSettingsYearToControllerDaysConnections()
    const
{
    auto controllerSettingsYear =
        qobject_cast<ControllerSettingsYear *>(m_controllerSettingsYear);

    auto controllerSettingsDaysAsQObject =
        controllerSettingsYear->controllerSettingsDays();

    std::map<Weekday, ControllerSettingsDay *> controllerSettingsDays;
    for (const auto &controllerSettingsDayAsQObject :
         controllerSettingsDaysAsQObject) {
        auto controllerSettingDay = qobject_cast<ControllerSettingsDay *>(
            controllerSettingsDayAsQObject);

        auto weekday = controllerSettingDay->weekday();
        Q_ASSERT(
            controllerSettingsDays.find(weekday) ==
            controllerSettingsDays.end());
        controllerSettingsDays.insert({weekday, controllerSettingDay});
    }

    auto controllerDays = this->controllerDays();

    for (const auto &controllerDayAsQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto weekday = controllerDay->day()->date().weekday();

        connect(
            controllerSettingsDays.at(weekday),
            &ControllerSettingsDay::defaultWorkTimeChanged,
            controllerDay,
            &ControllerDay::onDefaultWorkTimeChanged);

        connect(
            controllerSettingsDays.at(weekday),
            &ControllerSettingsDay::pauseTimeChanged,
            controllerDay,
            &ControllerDay::onPauseTimeChanged);
    }
}

void ControllerYear::setOvertime(const HoursAndMinutes &overtime)
{
    if (m_overtime == overtime) {
        return;
    }
    m_overtime = overtime;
    emit overtimeChanged();
}

void ControllerYear::setUsedFlextimeDays(double usedFlextimeDays)
{
    if (m_usedFlextimeDays == usedFlextimeDays) {
        return;
    }
    m_usedFlextimeDays = usedFlextimeDays;
    emit usedFlextimeDaysChanged();
}

void ControllerYear::setUsedVacationDays(double usedVacationDays)
{
    if (m_usedVacationDays == usedVacationDays) {
        return;
    }
    m_usedVacationDays = usedVacationDays;
    emit usedVacationDaysChanged();
}

} // namespace whm
