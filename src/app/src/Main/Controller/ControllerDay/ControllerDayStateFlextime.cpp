#include "ControllerDayStateFlextime.hpp"

#include "../ControllerDay.hpp"

namespace whm {

ControllerDayState &ControllerDayStateFlextime::getInstance()
{
    static ControllerDayStateFlextime instance;
    return instance;
}

void ControllerDayStateFlextime::calculate(ControllerDay *controllerDay)
{
    controllerDay->setStartTime("0:00");
    controllerDay->setEndTime("0:00");

    auto defaultWorkTime = controllerDay->defaultWorkTime();
    controllerDay->setWorkTime(defaultWorkTime);

    auto overtime = HoursAndMinutes{-defaultWorkTime.totalMinutes()};
    controllerDay->setOvertime(overtime);

    controllerDay->setTimeInputIsEnabled(false);
    controllerDay->setUsedFlextimeDay(1.0);
    controllerDay->setUsedVacationDay(0.0);
}

} // namespace whm
