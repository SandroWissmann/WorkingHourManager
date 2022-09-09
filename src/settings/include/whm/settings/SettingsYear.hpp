#ifndef WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP

#include "SettingsDay.hpp"

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
