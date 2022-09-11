#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_STATE_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_STATE_HPP

namespace whm {

class ControllerDay;

class ControllerDayState {
public:
    ControllerDayState() = default;
    virtual ~ControllerDayState() = default;

    ControllerDayState(const ControllerDayState &) = default;
    ControllerDayState(ControllerDayState &&) = default;
    ControllerDayState &operator=(const ControllerDayState &) = default;
    ControllerDayState &operator=(ControllerDayState &&) = default;

    virtual void calculate(ControllerDay *controllerDay) = 0;

private:
};
} // namespace whm

#endif
