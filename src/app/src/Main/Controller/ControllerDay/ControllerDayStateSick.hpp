#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_SICK_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_SICK_HPP

#include "ControllerDayState.hpp"

namespace whm {

class ControllerDayStateSick : public ControllerDayState {
public:
    ControllerDayStateSick() = default;
    virtual ~ControllerDayStateSick() override = default;

    ControllerDayStateSick(const ControllerDayStateSick &) = default;
    ControllerDayStateSick(ControllerDayStateSick &&) = default;
    ControllerDayStateSick &operator=(const ControllerDayStateSick &) = default;
    ControllerDayStateSick &operator=(ControllerDayStateSick &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
