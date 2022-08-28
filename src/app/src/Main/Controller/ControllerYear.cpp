#include "ControllerYear.hpp"

#include <whm/types/Day.hpp>

#include "ControllerMonth.hpp"

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

ControllerYear::ControllerYear(const QVector<QObject *> &controllerMonths)
    : m_controllerMonths{controllerMonths}, m_overtime{calculateOvertime(
                                                m_controllerMonths)}
{
    makeControllerMonthsToThisConnections();
}

QVector<QObject *> ControllerYear::controllerMonths() const
{
    return m_controllerMonths;
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

void ControllerYear::setOvertime(const HoursAndMinutes &overtime)
{
    if (m_overtime == overtime) {
        return;
    }
    m_overtime = overtime;
    emit overtimeChanged();
}

} // namespace whm
