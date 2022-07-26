#include "ControllerYear.hpp"

#include <whm/types/Day.hpp>

#include "ControllerMonth.hpp"

#include <algorithm>

namespace whm {

ControllerYear::ControllerYear(const QVector<QObject *> &controllerMonths)
    : m_controllerMonths{controllerMonths}
{
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

} // namespace whm
