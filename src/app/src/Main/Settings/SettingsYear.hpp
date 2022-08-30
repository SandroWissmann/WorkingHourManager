#ifndef WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_SETTINGS_YEAR_HPP

#include <whm/types/Time.hpp>

#include <vector>

namespace whm {

class SettingsYear {
public:
    SettingsYear();

    SettingsYear(
        const std::array<Time, 5> &defaultWorkTimesMoToFr,
        const std::array<Time, 5> &pauseTimesMoToFr);

    SettingsYear(const SettingsYear &) = default;
    SettingsYear(SettingsYear &&) = default;
    SettingsYear &operator=(const SettingsYear &) = default;
    SettingsYear &operator=(SettingsYear &&) = default;

    std::array<Time, 5> defaultWorkTimesMoToFr() const;
    std::array<Time, 5> pauseTimesMoToFr() const;

    Time defaultWorkTimeMonday() const;
    Time defaultWorkTimeTuesday() const;
    Time defaultWorkTimeWednesday() const;
    Time defaultWorkTimeThursday() const;
    Time defaultWorkTimeFriday() const;

    bool setDefaultWorkTimeMonday(const QString &defaultWorkTime);
    bool setDefaultWorkTimeTuesday(const QString &defaultWorkTime);
    bool setDefaultWorkTimeWednesday(const QString &defaultWorkTime);
    bool setDefaultWorkTimeThursday(const QString &defaultWorkTime);
    bool setDefaultWorkTimeFriday(const QString &defaultWorkTime);

    Time pauseTimeMonday() const;
    Time pauseTimeTuesday() const;
    Time pauseTimeWednesday() const;
    Time pauseTimeThursday() const;
    Time pauseTimeFriday() const;

    bool setPauseTimeMonday(const QString &pauseTime);
    bool setPauseTimeTuesday(const QString &pauseTime);
    bool setPauseTimeWednesday(const QString &pauseTime);
    bool setPauseTimeThursday(const QString &pauseTime);
    bool setPauseTimeFriday(const QString &pauseTime);

private:
    std::array<Time, 5> m_defaultWorkTimesMoToFr;
    std::array<Time, 5> m_pauseTimesMoToFr;
};

} // namespace whm

#endif
