#ifndef WORKING_HOUR_MANAGER_HELPER_CONTROLLER_YEAR_HPP
#define WORKING_HOUR_MANAGER_HELPER_CONTROLLER_YEAR_HPP

#include <QObject>
#include <QVector>

namespace whm {

class Day;
class SettingsYear;

void debugPrint(const QVector<QObject *> &controllerYears);

/*
  Steps:

  1 Generate ControllerDays from Days and SettingsYears

  2 Generate ControllerWeeks from ControllerDays

  3 Get first year from Days

  4 Generate ControllerMonths from first year + ControllerWeeks

  5 Generate ControllerSettingsYears from SettingsYears

  6 Generate Controller Years from ControllerMonths and ControllerSettingsYears
*/
QVector<QObject *> makeControllerYears(
    const QVector<std::shared_ptr<Day>> &days,
    const std::map<int, SettingsYear> &yearsToSettingsYears);

QVector<std::shared_ptr<Day>>
getDays(const QVector<QObject *> &controllerYears);

QVector<SettingsYear>
getSettingsYears(const QVector<QObject *> &controllerYears);

} // namespace whm

#endif
