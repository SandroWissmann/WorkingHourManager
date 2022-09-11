#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_FLEXTIME_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_FLEXTIME_HPP

#include "ControllerDayState.hpp"

namespace whm {

class ControllerDayStateFlextime : public ControllerDayState {
public:
    ControllerDayStateFlextime() = default;
    virtual ~ControllerDayStateFlextime() override = default;

    ControllerDayStateFlextime(const ControllerDayStateFlextime &) = default;
    ControllerDayStateFlextime(ControllerDayStateFlextime &&) = default;
    ControllerDayStateFlextime &
    operator=(const ControllerDayStateFlextime &) = default;
    ControllerDayStateFlextime &
    operator=(ControllerDayStateFlextime &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
