#include "Backend.hpp"

#include "Backend/ControllerYear.hpp"
#include "Backend/FileWriter.hpp"
#include "Common/ControllerWeek.hpp"
#include "Common/ControllerWeek/ControllerDay.hpp"

#include "Support/Date.hpp"

#include <QDebug>

namespace whm {

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const Date &firstDate,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent);

} // namespace

Backend::Backend(
    const Date &firstDate,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
    : QObject{parent}, m_defaultWorkTimePerDay{defaultWorkTimePerDay},
      m_pauseTimesPerDay{pauseTimesPerDay},
      m_controllerWeeks{makeControllerWeeksUntilCurrentWeek(
          firstDate,
          defaultWorkTimePerDay,
          pauseTimesPerDay,
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

void Backend::saveToFile()
{
    FileWriter fileWriter{"save.json"};
    auto writeOk = fileWriter.writeToFile(
        m_defaultWorkTimePerDay, m_pauseTimesPerDay, m_controllerWeeks);

    if (!writeOk) {
        qWarning() << Q_FUNC_INFO << "Save to file failed.";
    }
}

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const Date &firstDate,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
{
    constexpr int mondayIdx = 1;
    auto startDate = firstDate.getPreviouseDateWithDayOfWeek(mondayIdx);

    // auto startDate = firstDate;
    auto endDate = Date::currentDate();

    QVector<QObject *> controllerWeeks;
    while (startDate <= endDate) {
        controllerWeeks.emplace_back(new ControllerWeek{
            startDate, defaultWorkTimePerDay, pauseTimesPerDay, parent});
        startDate = startDate.addDays(7);
    }

    return controllerWeeks;
}
} // namespace

} // namespace whm
