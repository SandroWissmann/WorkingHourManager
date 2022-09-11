#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HOLIDAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HOLIDAY_HPP

#include "ControllerDayState.hpp"
namespace whm {

class ControllerDayStateHoliday : public ControllerDayState {
public:
    ControllerDayStateHoliday() = default;
    virtual ~ControllerDayStateHoliday() override = default;

    ControllerDayStateHoliday(const ControllerDayStateHoliday &) = default;
    ControllerDayStateHoliday(ControllerDayStateHoliday &&) = default;
    ControllerDayStateHoliday &
    operator=(const ControllerDayStateHoliday &) = default;
    ControllerDayStateHoliday &
    operator=(ControllerDayStateHoliday &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
