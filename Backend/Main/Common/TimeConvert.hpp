#ifndef WORKING_HOUR_MANAGER_TIME_CONVERT_HPP
#define WORKING_HOUR_MANAGER_TIME_CONVERT_HPP

#include <QString>
#include <QTime>

namespace whm {

QTime stringToTime(const QString hhmmStr);

QString timeToString(const QTime &time);

}

#endif
