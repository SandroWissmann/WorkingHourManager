#include "Backend.hpp"

#include "ControllerWeek.hpp"
#include "ControllerYear.hpp"

namespace whm {

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const QDate &firstDate,
    QTime defaultWorkTimePerDay,
    QTime pauseTimeMonday,
    QTime pauseTimeTuesday,
    QTime pauseTimeWednesday,
    QTime pauseTimeThursday,
    QTime pauseTimeFriday,
    QObject *parent);

QDate goBackToPreviousMonday(const QDate &firstDate);

// similar to build in functions QDate::addDays but negative and only for
// one day
void removeDay(QDate &date);

QDate makeCurrentDate();

} // namespace

Backend::Backend(
    const QDate &firstDate,
    QTime defaultWorkTimePerDay,
    QTime pauseTimeMonday,
    QTime pauseTimeTuesday,
    QTime pauseTimeWednesday,
    QTime pauseTimeThursday,
    QTime pauseTimeFriday,
    QObject *parent)
    : QObject{parent}, m_controllerWeeks{makeControllerWeeksUntilCurrentWeek(
                           firstDate,
                           defaultWorkTimePerDay,
                           pauseTimeMonday,
                           pauseTimeTuesday,
                           pauseTimeWednesday,
                           pauseTimeThursday,
                           pauseTimeFriday,
                           this)}
{
}

QVector<QObject *> Backend::controllerWeeks() const
{
    return m_controllerWeeks;
}

QVector<QObject *> Backend::controllerYears() const
{
    return m_controllerYears;
}

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const QDate &firstDate,
    QTime defaultWorkTimePerDay,
    QTime pauseTimeMonday,
    QTime pauseTimeTuesday,
    QTime pauseTimeWednesday,
    QTime pauseTimeThursday,
    QTime pauseTimeFriday,
    QObject *parent)
{
    auto startDate = goBackToPreviousMonday(firstDate);

    // auto startDate = firstDate;
    auto endDate = makeCurrentDate();

    QVector<QObject *> controllerWeeks;
    while (startDate <= endDate) {
        controllerWeeks.emplace_back(new ControllerWeek{
            startDate,
            defaultWorkTimePerDay,
            pauseTimeMonday,
            pauseTimeTuesday,
            pauseTimeWednesday,
            pauseTimeThursday,
            pauseTimeFriday,
            parent});
        startDate = startDate.addDays(7);
    }

    return controllerWeeks;
}

QDate goBackToPreviousMonday(const QDate &firstDate)
{
    QDate date = firstDate;
    constexpr int dayOfWeekMonday = 1;

    while (date.dayOfWeek() != dayOfWeekMonday) {
        removeDay(date);
    }

    return date;
}

void removeDay(QDate &date)
{
    auto year = date.year();
    auto month = date.month();
    auto day = date.day();

    date.setDate(year, month, day - 1);

    // we were at start of the month so go back one month
    if (!date.isValid()) {

        date.setDate(year, month - 1, 1);
        auto daysInMonth = date.daysInMonth();
        date.setDate(year, month - 1, daysInMonth);

        // we were at start of the year so go back one year
        if (!date.isValid()) {
            date.setDate(year - 1, 12, 31);
        }
    }

    if (!date.isValid()) {
        qDebug() << Q_FUNC_INFO << "Date is invalid";
    }
}

QDate makeCurrentDate()
{
    auto currentDateTime = QDateTime::currentDateTime();
    return currentDateTime.date();
}

} // namespace

} // namespace whm
