#include "ControllerDayStateParty.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {
ControllerDayState &ControllerDayStateParty::getInstance()
{
    static ControllerDayStateParty instance;
    return instance;
}
void ControllerDayStateParty::calculate(ControllerDay *controllerDay)
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
