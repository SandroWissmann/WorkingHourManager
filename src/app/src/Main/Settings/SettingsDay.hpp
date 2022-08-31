#ifndef WORKING_HOUR_MANAGER_SETTINGS_DAY_HPP
#define WORKING_HOUR_MANAGER_SETTINGS_DAY_HPP

#include <whm/types/Time.hpp>
#include <whm/types/Weekday.hpp>

#include <map>

namespace whm {

class SettingsDay {
public:
    SettingsDay(
        Weekday weekday,
        Time defaultWorkTime,
        Time pauseTime);

    SettingsDay(const SettingsDay &) = default;
    SettingsDay(SettingsDay &&) = default;
    SettingsDay &operator=(const SettingsDay &) = default;
    SettingsDay &operator=(SettingsDay &&) = default;

    Weekday weekday() const;
    QString weekdayAsString() const;

    Time defaultWorkTime() const;
    bool setDefaultWorkTime(const QString &defaultWorkTime);

    Time pauseTime() const;
    bool setPauseTime(const QString &pauseTime);

private:
    Weekday m_weekday;
    Time m_defaultWorkTime;
    Time m_pauseTime;
};

} // namespace whm

#endif

