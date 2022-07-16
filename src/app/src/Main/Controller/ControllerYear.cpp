#include "ControllerYear.hpp"

#include "ControllerWeek.hpp"
#include "ControllerMonth.hpp"

#include <algorithm>

namespace whm {

namespace {
QVector<QObject *> makeControllerMonths(
    const QVector<QObject *> &controllerWeeks,
    QObject *parent);

void addControllerWeekToMonths(
    ControllerWeek *controllerWeek,
    QVector<QObject *> &controllerMonths,
    QObject *parent);

void removeMonth(QVector<int> &months, int month);
} // namespace

ControllerYear::ControllerYear(
    const QVector<QObject *> &controllerWeeks,
    QObject *parent)
    : QObject{parent}, m_controllerMonths{
                           makeControllerMonths(controllerWeeks, parent)}
{
}

QVector<QObject *> ControllerYear::controllerMonths() const
{
    return m_controllerMonths;
}

namespace {
QVector<QObject *>
makeControllerMonths(const QVector<QObject *> &controllerWeeks, QObject *parent)
{
    QVector<QObject *> controllerMonths;

    for (const auto &controllerWeekAsQObject : controllerWeeks) {
        auto controllerWeek =
            qobject_cast<ControllerWeek *>(controllerWeekAsQObject);

        addControllerWeekToMonths(controllerWeek, controllerMonths, parent);
    }
    return controllerMonths;
}

void addControllerWeekToMonths(
    ControllerWeek *controllerWeek,
    QVector<QObject *> &controllerMonths,
    QObject *parent)
{
    auto weekMonths = controllerWeek->months();

    // check if week can be added to months with weekMonths
    for (const auto &controllerMonthsAsQObject : controllerMonths) {

        auto controllerMonth =
            qobject_cast<ControllerMonth *>(controllerMonthsAsQObject);

        auto month = controllerMonth->month();

        for (const auto &weekMonth : weekMonths) {
            if (weekMonth == month) {
                controllerMonth->insertControllerWeek(controllerWeek);

                removeMonth(weekMonths, month);
                break;
            }
        }
    }

    // it can happen once that we have weekMonths == 2 but then we still
    // only insert one more month with the seond value. The first value
    // is always to be dscarded because it will be in a month before the
    // first one we want in the system.
    // e.g. If we start on 01.01.2021 and the first week gives {11,0} we
    // only keep the 0 = january because we are not interested in december.

    if (!weekMonths.isEmpty()) {
        auto controllerMonth = new ControllerMonth{parent};
        controllerMonth->insertControllerWeek(controllerWeek);
        controllerMonths.emplaceBack(controllerMonth);
    }
}

void removeMonth(QVector<int> &months, int month)
{
    months.erase(
        std::remove(months.begin(), months.end(), month), months.end());
}

} // namespace

} // namespace whm
