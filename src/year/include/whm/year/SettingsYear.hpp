/* Working Hour Manager
 * Copyright (C) 2022 Sandro Wißmann
 *
 * Working Hour Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Working Hour Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quiz If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/WorkingHourManager
 */
#ifndef WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP

#include <whm/day/SettingsDay.hpp>
#include <whm/types/Time.hpp>
#include <whm/types/Weekday.hpp>

#include <map>

namespace whm {

class SettingsYear {
public:
    SettingsYear();

    SettingsYear(
        double flextimeDays,
        double vacationDays,
        std::map<Weekday, SettingsDay> weekdayToSettingsDay);

    SettingsYear(const SettingsYear &) = default;
    SettingsYear(SettingsYear &&) = default;
    SettingsYear &operator=(const SettingsYear &) = default;
    SettingsYear &operator=(SettingsYear &&) = default;

    std::map<Weekday, SettingsDay> weekdayToSettingsDay() const;

    SettingsDay *settingsDay(Weekday weekday);

    std::array<Time, 5> defaultWorkTimesMoToFr() const;
    std::array<Time, 5> pauseTimesMoToFr() const;

    Time defaultWorkTime(Weekday weekday) const;
    bool
    setDefaultWorkTime(Weekday weekday, const QString &defaultWorkTimeAsString);

    Time pauseTime(Weekday weekday) const;
    bool setPauseTime(Weekday weekday, const QString &pauseTimeAsString);

    double flextimeDays() const;
    bool setFlextimeDays(const QString &flextimeDaysAsString);

    double vacationDays() const;
    bool setVacationDays(const QString &vacationDaysAsString);

    static constexpr double defaultFlextimneDays()
    {
        return 6.0;
    }

    static constexpr double defaultVacationDays()
    {
        return 30.0;
    }

private:
    std::map<Weekday, SettingsDay> m_weekdayToSettingsDay;
    double m_flextimeDays;
    double m_vacationDays;
};

} // namespace whm

#endif
