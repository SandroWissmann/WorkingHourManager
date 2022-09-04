#ifndef WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP

#include "SettingsDay.hpp"

#include <whm/types/Time.hpp>
#include <whm/types/Weekday.hpp>

#include <map>

namespace whm {

class SettingsYear {
public:
    SettingsYear(int year);

    SettingsYear(int year, std::map<Weekday, SettingsDay> weekdayToSettingsDay);

    SettingsYear(const SettingsYear &) = default;
    SettingsYear(SettingsYear &&) = default;
    SettingsYear &operator=(const SettingsYear &) = default;
    SettingsYear &operator=(SettingsYear &&) = default;

    int year() const;

    std::map<Weekday, SettingsDay> weekdayToSettingsDay() const;

    SettingsDay *settingsDay(Weekday weekday);

    std::array<Time, 5> defaultWorkTimesMoToFr() const;

    std::array<Time, 5> pauseTimesMoToFr() const;

    Time defaultWorkTime(Weekday weekday) const;

    bool
    setDefaultWorkTime(Weekday weekday, const QString &defaultWorkTimeAsString);

    Time pauseTime(Weekday weekday) const;

    bool setPauseTime(Weekday weekday, const QString &pauseTimeAsString);

private:
    int m_year{};
    std::map<Weekday, SettingsDay> m_weekdayToSettingsDay;
};

} // namespace whm

#endif
