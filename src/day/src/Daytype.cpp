#include "../include/whm/day/DayType.hpp"

#include <QMetaEnum>

namespace whm {

QStringList dayTypeAsStringList()
{
    auto metaEnum = QMetaEnum::fromType<DayType>();
    QStringList dayTypesAsStrings;

    int count = metaEnum.keyCount();

    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        auto dayType = static_cast<DayType>(metaEnum.value(i));

        auto dayTypeAsString = QVariant::fromValue(dayType).toString();
        dayTypesAsStrings.emplace_back(dayTypeAsString);
    }
    return dayTypesAsStrings;
}
} // namespace whm
