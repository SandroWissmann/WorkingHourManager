#include "ControllerDayStateIgnore.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {
ControllerDayState &ControllerDayStateIgnore::getInstance()
{
    static ControllerDayStateIgnore instance;
    return instance;
}
void ControllerDayStateIgnore::calculate(ControllerDay *controllerDay)
{
    controllerDay->setStartTime("0:00");
    controllerDay->setEndTime("0:00");
    controllerDay->setWorkTime(Time{});
    controllerDay->setOvertime(HoursAndMinutes{});
    controllerDay->setTimeInputIsEnabled(false);
    controllerDay->setUsedFlextimeDay(0.0);
    controllerDay->setUsedVacationDay(0.0);
}

} // namespace whm
