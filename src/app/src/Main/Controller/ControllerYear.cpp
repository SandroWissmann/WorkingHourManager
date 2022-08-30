#include "ControllerYear.hpp"

#include <whm/types/Day.hpp>

#include "ControllerDay.hpp"
#include "ControllerMonth.hpp"
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
} // namespace

ControllerYear::ControllerYear(
    const QVector<QObject *> &controllerMonths,
    QObject *controllerSettingsYear)
    : m_controllerMonths{controllerMonths},
      m_controllerSettingsYear{controllerSettingsYear},
      m_overtime{calculateOvertime(m_controllerMonths)}
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

QString ControllerYear::overtime() const
{
    return m_overtime.toString();
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
    }
}

void ControllerYear::makeControllerSettingsYearToControllerDaysConnections()
    const
{
    auto controllerSettingsYear =
        qobject_cast<ControllerSettingsYear *>(m_controllerSettingsYear);
    auto controllerDays = this->controllerDays();

    for (const auto &controllerDayAsQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto weekday = controllerDay->day()->date().weekday();

        if (weekday == "Monday") {
            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::defaultWorkTimeMondayChanged,
                controllerDay,
                &ControllerDay::onDefaultWorkTimeChanged);

            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::pauseTimeMondayChanged,
                controllerDay,
                &ControllerDay::onPauseTimeChanged);
        }
        else if (weekday == "Tuesday") {
            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::defaultWorkTimeTuesdayChanged,
                controllerDay,
                &ControllerDay::onDefaultWorkTimeChanged);

            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::pauseTimeTuesdayChanged,
                controllerDay,
                &ControllerDay::onPauseTimeChanged);
        }
        else if (weekday == "Wednesday") {
            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::defaultWorkTimeWednesdayChanged,
                controllerDay,
                &ControllerDay::onDefaultWorkTimeChanged);

            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::pauseTimeWednesdayChanged,
                controllerDay,
                &ControllerDay::onPauseTimeChanged);
        }
        else if (weekday == "Thursday") {
            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::defaultWorkTimeThursdayChanged,
                controllerDay,
                &ControllerDay::onDefaultWorkTimeChanged);

            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::pauseTimeThursdayChanged,
                controllerDay,
                &ControllerDay::onPauseTimeChanged);
        }
        else if (weekday == "Friday") {
            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::defaultWorkTimeFridayChanged,
                controllerDay,
                &ControllerDay::onDefaultWorkTimeChanged);

            connect(
                controllerSettingsYear,
                &ControllerSettingsYear::pauseTimeFridayChanged,
                controllerDay,
                &ControllerDay::onPauseTimeChanged);
        }
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

} // namespace whm
