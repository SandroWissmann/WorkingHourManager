#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_WORK_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_WORK_HPP

#include "ControllerDayState.hpp"

namespace whm {

class ControllerDayStateWork : public ControllerDayState {
public:
    ControllerDayStateWork() = default;
    virtual ~ControllerDayStateWork() override = default;

    ControllerDayStateWork(const ControllerDayStateWork &) = default;
    ControllerDayStateWork(ControllerDayStateWork &&) = default;
    ControllerDayStateWork &operator=(const ControllerDayStateWork &) = default;
    ControllerDayStateWork &operator=(ControllerDayStateWork &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
