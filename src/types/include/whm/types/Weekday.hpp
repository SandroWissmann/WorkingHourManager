#ifndef WORKING_HOUR_MANAGER_WEEKDAY_HPP
#define WORKING_HOUR_MANAGER_WEEKDAY_HPP

#include <QObject>

namespace whm {

class WeekdayQEnum {
    Q_GADGET
public:
    enum class Value {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday,
        Unknown
    };
    Q_ENUM(Value)

private:
    explicit WeekdayQEnum();
};

typedef WeekdayQEnum::Value Weekday;

} // namespace whm

#endif
