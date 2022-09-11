#include "ControllerDayStateWork.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {

namespace {
Time calcWorkTime(
    const Time &startTime,
    const Time &endTime,
    const Time &pauseTime)
{
    auto timesAreValid = startTime.isValid() && endTime.isValid();

    if (!timesAreValid) {
        return Time{};
    }
    auto workTime = endTime - startTime - pauseTime;

    // If result would be negative time gets invalid and has empty string so
    // we correct it to 0:00
    if (workTime.asString().isEmpty()) {
        return Time{};
    }
    return workTime;
}

HoursAndMinutes calcOvertime(
    const Time &startTime,
    const Time &endTime,
    const Time &workTime,
    const Time &defaultWorkTime)
{
    if (!startTime.isValid()) {
        return HoursAndMinutes{};
    }
    if (!endTime.isValid()) {
        return HoursAndMinutes{};
    }
    return HoursAndMinutes{workTime} - HoursAndMinutes{defaultWorkTime};
}

} // namespace

ControllerDayState &ControllerDayStateWork::getInstance()
{
    static ControllerDayStateWork instance;
    return instance;
}

void ControllerDayStateWork::calculate(ControllerDay *controllerDay)
{
    auto enteredStartTime = controllerDay->enteredStartTime();
    controllerDay->setStartTime(enteredStartTime);

    auto enteredEndTime = controllerDay->enteredEndTime();
    controllerDay->setEndTime(enteredEndTime);

    auto workTime = calcWorkTime(
        controllerDay->startTime(),
        controllerDay->endTime(),
        controllerDay->pauseTime());
    controllerDay->setWorkTime(workTime);

    auto startTime = controllerDay->startTime();
    auto endTime = controllerDay->endTime();
    auto defaultWorkTime = controllerDay->defaultWorkTime();

    auto overtime = calcOvertime(startTime, endTime, workTime, defaultWorkTime);
    controllerDay->setOvertime(overtime);

    controllerDay->setTimeInputIsEnabled(true);

    controllerDay->setUsedFlextimeDay(0.0);
    controllerDay->setUsedVacationDay(0.0);
}

} // namespace whm
