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
public:
    ControllerSettingsYear(const SettingsYear &settingsYear);

    ControllerSettingsYear(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear(ControllerSettingsYear &&) = delete;
    ControllerSettingsYear &operator=(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear &operator=(ControllerSettingsYear &&) = delete;

    SettingsYear settingsYear() const;

    QVector<QObject *> controllerSettingsDays() const;

private:
    SettingsYear m_settingsYear;
    QVector<QObject *> m_controllerSettingsDays;
};

} // namespace whm

#endif
