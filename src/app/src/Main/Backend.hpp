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
#ifndef WORKING_HOUR_MANAGER_BACKEND_HPP
#define WORKING_HOUR_MANAGER_BACKEND_HPP

#include <QObject>
#include <QVector>

#include <array>

namespace whm {

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerYears READ controllerYears NOTIFY
                   controllerYearsChanged)
    Q_PROPERTY(QObject *controllerSettings READ controllerSettings CONSTANT)
    Q_PROPERTY(int currentYear READ currentYear CONSTANT)
public:
    Backend(QObject *parent = nullptr);

    Backend(const Backend &) = delete;
    Backend(Backend &&) = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) = delete;

    ~Backend() = default;

    QVector<QObject *> controllerYears() const;

    QObject *controllerSettings() const;

    int currentYear() const;

    Q_INVOKABLE
    bool readControllerYearsFromFile();

    Q_INVOKABLE
    void generateControllerYears(int year);

    Q_INVOKABLE
    void saveToFile();

signals:
    void controllerYearsChanged();

private:
    void setControllerYears(const QVector<QObject *> &controllerYears);

    QVector<QObject *> m_controllerYears;
    QObject *m_controllerSettings;
};
} // namespace whm

#endif
