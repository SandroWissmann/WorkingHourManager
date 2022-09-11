#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_IGNORE_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_IGNORE_HPP

#include "ControllerDayState.hpp"

namespace whm {

class ControllerDayStateIgnore : public ControllerDayState {
public:
    ControllerDayStateIgnore() = default;
    virtual ~ControllerDayStateIgnore() override = default;

    ControllerDayStateIgnore(const ControllerDayStateIgnore &) = default;
    ControllerDayStateIgnore(ControllerDayStateIgnore &&) = default;
    ControllerDayStateIgnore &
    operator=(const ControllerDayStateIgnore &) = default;
    ControllerDayStateIgnore &operator=(ControllerDayStateIgnore &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
