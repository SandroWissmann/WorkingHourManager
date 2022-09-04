#ifndef WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_YEAR_HPP

#include <whm/types/Time.hpp>

#include "../../Settings/SettingsYear.hpp"

#include <QObject>

#include <vector>

namespace whm {

class ControllerSettingsYear : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerSettingsDays READ
                   controllerSettingsDays CONSTANT)
    Q_PROPERTY(QString flextimeDays READ flextimeDaysAsString WRITE
                   setFlextimeDays NOTIFY flextimeDaysChanged)
    Q_PROPERTY(QString vacationDays READ vacationDaysAsString WRITE
                   setVacationDays NOTIFY vacationDaysChanged)
public:
    ControllerSettingsYear(const SettingsYear &settingsYear);

    ControllerSettingsYear(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear(ControllerSettingsYear &&) = delete;
    ControllerSettingsYear &operator=(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear &operator=(ControllerSettingsYear &&) = delete;

    SettingsYear settingsYear() const;

    QVector<QObject *> controllerSettingsDays() const;

    double flextimeDays();
    QString flextimeDaysAsString();
    void setFlextimeDays(const QString &flextimeDaysAsString);

    double vacationDays();
    QString vacationDaysAsString();
    void setVacationDays(const QString &vacationDaysAsString);

signals:
    void flextimeDaysChanged();
    void vacationDaysChanged();

private:
    SettingsYear m_settingsYear;
    QVector<QObject *> m_controllerSettingsDays;
};

} // namespace whm

#endif
