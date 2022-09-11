#include "ControllerDayStateSick.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {
ControllerDayState &ControllerDayStateSick::getInstance()
{
    static ControllerDayStateSick instance;
    return instance;
}
void ControllerDayStateSick::calculate(ControllerDay *controllerDay)
{
    controllerDay->setStartTime("0:00");
    controllerDay->setEndTime("0:00");

    auto defaultWorkTime = controllerDay->defaultWorkTime();
    controllerDay->setWorkTime(defaultWorkTime);

    controllerDay->setOvertime(HoursAndMinutes{});
    controllerDay->setTimeInputIsEnabled(false);
    controllerDay->setUsedFlextimeDay(0.0);
    controllerDay->setUsedVacationDay(0.0);
}

} // namespace whm
