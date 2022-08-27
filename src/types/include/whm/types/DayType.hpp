#ifndef WORKING_HOUR_MANAGER_DAYTYPE_HPP
#define WORKING_HOUR_MANAGER_DAYTYPE_HPP

#include <QObject>

namespace whm {

class DayTypeQEnum {
    Q_GADGET
public:
    enum class Value {
        work,
        holiday,
        vaccation,
        sick,
        ignore,
    };
    Q_ENUM(Value)

private:
    explicit DayTypeQEnum();
};

typedef DayTypeQEnum::Value DayType;

} // namespace whm

#endif
