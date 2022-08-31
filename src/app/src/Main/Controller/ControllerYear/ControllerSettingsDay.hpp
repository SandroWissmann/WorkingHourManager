#ifndef WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_DAY_HPP

#include <whm/types/Time.hpp>

#include "../../Settings/SettingsDay.hpp"

#include <QObject>

#include <vector>

namespace whm {

class ControllerSettingsDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString weekday READ weekdayAsString CONSTANT)

    Q_PROPERTY(QString defaultWorkTime READ defaultWorkTimeAsString WRITE
                   setDefaultWorkTime NOTIFY defaultWorkTimeChanged)

    Q_PROPERTY(QString pauseTime READ pauseTimeAsString WRITE setPauseTime
                   NOTIFY pauseTimeChanged)
public:
    ControllerSettingsDay(SettingsDay *settingsDay);

    ControllerSettingsDay(const ControllerSettingsDay &) = delete;
    ControllerSettingsDay(ControllerSettingsDay &&) = delete;
    ControllerSettingsDay &operator=(const ControllerSettingsDay &) = delete;
    ControllerSettingsDay &operator=(ControllerSettingsDay &&) = delete;

    SettingsDay settingsDay() const;

    Weekday weekday() const;
    QString weekdayAsString() const;

    QString defaultWorkTimeAsString() const;
    void setDefaultWorkTime(const QString &defaultWorkTime);

    QString pauseTimeAsString() const;
    void setPauseTime(const QString &pauseTime);

signals:
    // parameter needed here because we catch signal in ControllerDay.
    void defaultWorkTimeChanged(const Time &defaultWorkTime);
    void pauseTimeChanged(const Time &pauseTime);

private:
    SettingsDay *m_settingsDay;
};

} // namespace whm

#endif
