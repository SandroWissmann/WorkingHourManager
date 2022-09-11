#ifndef WORKING_HOUR_MANAGER_DAYTYPE_HPP
#define WORKING_HOUR_MANAGER_DAYTYPE_HPP

#include <QObject>
#include <QStringList>

namespace whm {

class DayTypeQEnum {
    Q_GADGET
public:
    enum class Value {
        Work,
        Holiday,
        Vacation,
        Sick,
        Party, // party at company generates the default work houts per day
        Flextime,
        Ignore,
    };
    Q_ENUM(Value)

private:
    explicit DayTypeQEnum();
};

typedef DayTypeQEnum::Value DayType;

QStringList dayTypeAsStringList();

} // namespace whm

#endif
