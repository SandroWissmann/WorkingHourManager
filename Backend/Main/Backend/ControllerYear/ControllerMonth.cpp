#include "ControllerMonth.hpp"

#include "../../Common/ControllerWeek.hpp"

namespace whm {

namespace {
int calculateMonth(ControllerWeek *controllerWeeks);
}

ControllerMonth::ControllerMonth(QObject *parent) : QObject{parent}
{
}

QVector<QObject *> ControllerMonth::controllerWeeks() const
{
    return m_controllerWeeks;
}

void ControllerMonth::insertControllerWeek(ControllerWeek *controllerWeek)
{
    // precondition weeks are ordered consecutively

    m_controllerWeeks.emplace_back(controllerWeek);

    // check only first week to determine month
    if (m_controllerWeeks.size() == 1) {
        m_month = calculateMonth(controllerWeek);
    }
}

int ControllerMonth::month() const
{
    return m_month;
}

namespace {
int calculateMonth(ControllerWeek *controllerWeek)
{
    auto months = controllerWeek->months();

    if (months.size() == 2) {
        // take second because first value is previous month
        return months.at(1);
    }
    if (months.size() == 1) {
        return months.at(0);
    }
    // invalid size for month. weeks can be only in one or two months.
    return -1;
}
} // namespace

} // namespace whm
