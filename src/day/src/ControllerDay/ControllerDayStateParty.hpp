#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_PARTY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_PARTY_HPP

#include "ControllerDayState.hpp"

namespace whm {

class ControllerDayStateParty : public ControllerDayState {
public:
    ControllerDayStateParty() = default;
    virtual ~ControllerDayStateParty() override = default;

    ControllerDayStateParty(const ControllerDayStateParty &) = default;
    ControllerDayStateParty(ControllerDayStateParty &&) = default;
    ControllerDayStateParty &
    operator=(const ControllerDayStateParty &) = default;
    ControllerDayStateParty &operator=(ControllerDayStateParty &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm
#endif
