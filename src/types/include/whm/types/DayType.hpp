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
        vacation,
        sick,
        party, // party at company generates the default work houts per day
        flextime,
        ignore,
    };
    Q_ENUM(Value)

private:
    explicit DayTypeQEnum();
};

typedef DayTypeQEnum::Value DayType;

} // namespace whm

#endif
