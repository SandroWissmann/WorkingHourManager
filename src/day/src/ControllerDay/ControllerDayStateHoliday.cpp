#include "ControllerDayStateHoliday.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {
ControllerDayState &ControllerDayStateHoliday::getInstance()
{
    static ControllerDayStateHoliday instance;
    return instance;
}
void ControllerDayStateHoliday::calculate(ControllerDay *controllerDay)
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
