#include "Backend.hpp"

#include <whm/types/Date.hpp>

#include "Backend/ControllerYear.hpp"
#include "Backend/FileReader.hpp"
#include "Backend/FileWriter.hpp"
#include "Common/ControllerWeek.hpp"
#include "Common/ControllerWeek/ControllerDay.hpp"

#include <QDebug>

namespace whm {

namespace {
QVector<QObject *> makeControllerWeeksUntilCurrentWeek(
    const Date &firstDate,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent);

Backend makeDefaultBackend();

Backend makeBackendFromFile(const FileReader &fileReader);

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

Backend::Backend(
    const QVector<QObject *> &controllerWeeks,
    const Time &defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
    : QObject{parent}, m_defaultWorkTimePerDay{defaultWorkTimePerDay},
      m_pauseTimesPerDay{pauseTimesPerDay}, m_controllerWeeks{controllerWeeks}
{
    for (auto &controllerWeek : m_controllerWeeks) {
        controllerWeek->setParent(this);
    }
}

Backend Backend::fromFile()
{
    FileReader fileReader{"save.json"};

    if (!fileReader.isValidFile()) {
        return makeDefaultBackend();
    }

    return makeBackendFromFile(fileReader);
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

Backend makeDefaultBackend()
{
    Date firstDate{2021, 01, 01};
    Time defaultWorkTimePerDay{7, 45};
    Time pauseTimeMondayToThursday{0, 45};
    Time pauseTimeFriday{0, 30};

    std::array<Time, 5> pauseTimesPerDay{
        pauseTimeMondayToThursday,
        pauseTimeMondayToThursday,
        pauseTimeMondayToThursday,
        pauseTimeMondayToThursday,
        pauseTimeFriday};

    return Backend{firstDate, defaultWorkTimePerDay, pauseTimesPerDay};
}

Backend makeBackendFromFile(const FileReader &fileReader)
{
    auto defaultWorkTimesPerDay = fileReader.defaultWorkTimePerDay();
    auto pauseTimesPerDay = fileReader.pauseTimesPerDay();
    auto days = fileReader.days();

    if (days.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "days are empty.";
        return makeDefaultBackend();
    }

    if (days.size() % 5 != 0) {
        qWarning() << Q_FUNC_INFO << "days are not multiplication of 5.";
        return makeDefaultBackend();
    }

    QVector<QObject *> controllerWeeks;
    for (auto it = days.begin(); it < days.end(); it += 5) {
        std::array<Day, 5> weekdays{
            *it, *(it + 1), *(it + 2), *(it + 3), *(it + 4)};

        auto controllerWeek = new ControllerWeek{
            defaultWorkTimesPerDay, pauseTimesPerDay, weekdays};
        controllerWeeks.emplace_back(controllerWeek);
    }

    return Backend{controllerWeeks, defaultWorkTimesPerDay, pauseTimesPerDay};
}

} // namespace

} // namespace whm
