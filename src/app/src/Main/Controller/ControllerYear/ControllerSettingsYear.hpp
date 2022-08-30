#ifndef WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_YEAR_HPP

#include <whm/types/Time.hpp>

#include "../../Settings/SettingsYear.hpp"

#include <QObject>

#include <vector>

namespace whm {

class ControllerSettingsYear : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString defaultWorkTimeMonday READ defaultWorkTimeMondayAsString
                   WRITE setDefaultWorkTimeMonday NOTIFY
                       defaultWorkTimeMondayAsStringChanged)
    Q_PROPERTY(
        QString defaultWorkTimeTuesday READ defaultWorkTimeTuesdayAsString WRITE
            setDefaultWorkTimeTuesday NOTIFY
                defaultWorkTimeTuesdayAsStringChanged)
    Q_PROPERTY(
        QString defaultWorkTimeWednesday READ defaultWorkTimeWednesdayAsString
            WRITE setDefaultWorkTimeWednesday NOTIFY
                defaultWorkTimeWednesdayAsStringChanged)
    Q_PROPERTY(
        QString defaultWorkTimeThursday READ defaultWorkTimeThursdayAsString
            WRITE setDefaultWorkTimeThursday NOTIFY
                defaultWorkTimeThursdayAsStringChanged)

    Q_PROPERTY(QString defaultWorkTimeFriday READ defaultWorkTimeFridayAsString
                   WRITE setDefaultWorkTimeFriday NOTIFY
                       defaultWorkTimeFridayAsStringChanged)

    Q_PROPERTY(
        QString pauseTimeMonday READ pauseTimeMondayAsString WRITE
            setPauseTimeMonday NOTIFY
                pauseTimeMondayAsStringChanged)
    Q_PROPERTY(
        QString pauseTimeTuesday READ pauseTimeTuesdayAsString
            WRITE setPauseTimeTuesday NOTIFY
                pauseTimeTuesdayAsStringChanged)
    Q_PROPERTY(
        QString pauseTimeWednesday READ pauseTimeWednesdayAsString
            WRITE setPauseTimeWednesday NOTIFY
                pauseTimeWednesdayAsStringChanged)
    Q_PROPERTY(
        QString pauseTimeThursday READ pauseTimeThursdayAsString
            WRITE setPauseTimeThursday NOTIFY
                pauseTimeThursdayAsStringChanged)

    Q_PROPERTY(QString defaultWorkTimeFriday READ defaultWorkTimeFridayAsString
                   WRITE setDefaultWorkTimeFriday NOTIFY
                       defaultWorkTimeFridayAsStringChanged)
public:
    ControllerSettingsYear(const SettingsYear &settingsYear);

    ControllerSettingsYear(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear(ControllerSettingsYear &&) = delete;
    ControllerSettingsYear &operator=(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear &operator=(ControllerSettingsYear &&) = delete;

    SettingsYear settingsYear() const;

    QString defaultWorkTimeMondayAsString() const;
    QString defaultWorkTimeTuesdayAsString() const;
    QString defaultWorkTimeWednesdayAsString() const;
    QString defaultWorkTimeThursdayAsString() const;
    QString defaultWorkTimeFridayAsString() const;

    QString pauseTimeMondayAsString() const;
    QString pauseTimeTuesdayAsString() const;
    QString pauseTimeWednesdayAsString() const;
    QString pauseTimeThursdayAsString() const;
    QString pauseTimeFridayAsString() const;

    void setDefaultWorkTimeMonday(const QString &defaultWorkTime);
    void setDefaultWorkTimeTuesday(const QString &defaultWorkTime);
    void setDefaultWorkTimeWednesday(const QString &defaultWorkTime);
    void setDefaultWorkTimeThursday(const QString &defaultWorkTime);
    void setDefaultWorkTimeFriday(const QString &defaultWorkTime);

    void setPauseTimeMonday(const QString &pauseTime);
    void setPauseTimeTuesday(const QString &pauseTime);
    void setPauseTimeWednesday(const QString &pauseTime);
    void setPauseTimeThursday(const QString &pauseTime);
    void setPauseTimeFriday(const QString &pauseTime);

signals:
    // signals with parameters here because we want to catch them with days
    void defaultWorkTimeMondayChanged(const whm::Time &defaultWorkTime);
    void defaultWorkTimeTuesdayChanged(const whm::Time &defaultWorkTime);
    void defaultWorkTimeWednesdayChanged(const whm::Time &defaultWorkTime);
    void defaultWorkTimeThursdayChanged(const whm::Time &defaultWorkTime);
    void defaultWorkTimeFridayChanged(const whm::Time &defaultWorkTime);

    void pauseTimeMondayChanged(const whm::Time &pauseTime);
    void pauseTimeTuesdayChanged(const whm::Time &pauseTime);
    void pauseTimeWednesdayChanged(const whm::Time &pauseTime);
    void pauseTimeThursdayChanged(const whm::Time &pauseTime);
    void pauseTimeFridayChanged(const whm::Time &pauseTime);

    void defaultWorkTimeMondayAsStringChanged();
    void defaultWorkTimeTuesdayAsStringChanged();
    void defaultWorkTimeWednesdayAsStringChanged();
    void defaultWorkTimeThursdayAsStringChanged();
    void defaultWorkTimeFridayAsStringChanged();

    void pauseTimeMondayAsStringChanged();
    void pauseTimeTuesdayAsStringChanged();
    void pauseTimeWednesdayAsStringChanged();
    void pauseTimeThursdayAsStringChanged();
    void pauseTimeFridayAsStringChanged();

private:
    SettingsYear m_settingsYear;
};

} // namespace whm

#endif
