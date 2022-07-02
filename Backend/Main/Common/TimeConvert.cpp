#include "TimeConvert.hpp"

namespace whm {

QTime stringToTime(const QString hhmmStr)
{
    return QTime::fromString(hhmmStr, "hh:mm");
}

QString timeToString(const QTime &time)
{
    return time.toString("hh:mm");
}

}
