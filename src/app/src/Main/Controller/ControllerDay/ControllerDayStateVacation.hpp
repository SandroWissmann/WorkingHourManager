#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_VACATION_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_VACATION_HPP

#include "ControllerDayState.hpp"

namespace whm {

class ControllerDayStateVacation : public ControllerDayState {
public:
    ControllerDayStateVacation() = default;
    virtual ~ControllerDayStateVacation() override = default;

    ControllerDayStateVacation(const ControllerDayStateVacation &) = default;
    ControllerDayStateVacation(ControllerDayStateVacation &&) = default;
    ControllerDayStateVacation &
    operator=(const ControllerDayStateVacation &) = default;
    ControllerDayStateVacation &
    operator=(ControllerDayStateVacation &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
