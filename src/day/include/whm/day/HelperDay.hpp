#ifndef WORKING_HOUR_MANAGER_HELPER_DAY_HPP
#define WORKING_HOUR_MANAGER_HELPER_DAY_HPP

#include <QObject>
#include <QVector>

#include <memory>

namespace whm{

class Day;
class Date;

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate);
}

#endif
