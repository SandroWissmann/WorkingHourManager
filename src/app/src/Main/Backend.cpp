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
#include "Backend.hpp"

#include <whm/day/ControllerDay.hpp>
#include <whm/day/Day.hpp>
#include <whm/day/HelperDay.hpp>
#include <whm/file/FileReader.hpp>
#include <whm/file/FileWriter.hpp>
#include <whm/month/ControllerMonth.hpp>
#include <whm/types/Date.hpp>
#include <whm/week/ControllerWeek.hpp>
#include <whm/year/ControllerSettingsYear.hpp>
#include <whm/year/ControllerYear.hpp>
#include <whm/year/HelperControllerYear.hpp>

#include <QDebug>

#include <map>

namespace whm {

namespace {

QVector<QObject *> makeDefaultControllerYears(int year)
{
    Date startDate{year, 01, 01};
    auto endDate = Date::currentDate();

    auto startYear = startDate.year();
    auto endYear = endDate.year();
    auto countOfYears = endYear - startYear + 1;

    QVector<int> years(countOfYears);
    std::iota(years.begin(), years.end(), startYear);

    auto days = makeWorkDays(startDate, endDate);

    std::map<int, SettingsYear> yearsToSettingsYears;
    for (const auto &year : years) {
        SettingsYear settingsYear{};
        yearsToSettingsYears.insert({year, settingsYear});
    }
    auto controllerYears = makeControllerYears(days, yearsToSettingsYears);
    return controllerYears;
}

} // namespace

Backend::Backend(QObject *parent) : QObject{parent}
{
}

QVector<QObject *> Backend::controllerYears() const
{
    return m_controllerYears;
}

int Backend::currentYear() const
{
    auto currentDate = Date::currentDate();
    return currentDate.year();
}

bool Backend::readControllerYearsFromFile()
{
    FileReader fileReader{"save.json"};

    if (!fileReader.isValidFile()) {
        return false;
    }

    auto days = fileReader.days();

    Q_ASSERT(!days.isEmpty());
    Q_ASSERT(days.size() % 5 == 0);

    auto yearsToSettingsYears = fileReader.yearsToSettingsYears();
    Q_ASSERT(!yearsToSettingsYears.empty());

    auto firstDateNotInFile = days.back()->date();
    firstDateNotInFile = firstDateNotInFile.addDays(1);
    auto currentDate = Date::currentDate();

    if (firstDateNotInFile < currentDate) {
        auto newDaysNotInFile = makeWorkDays(firstDateNotInFile, currentDate);

        for (const auto &newDay : newDaysNotInFile) {
            auto year = newDay->date().year();
            if (yearsToSettingsYears.find(year) == yearsToSettingsYears.end()) {
                SettingsYear settingsYear{};
                yearsToSettingsYears.insert({year, settingsYear});
            }
        }
        days.append(newDaysNotInFile);
    }

    Q_ASSERT(!days.isEmpty());
    Q_ASSERT(days.size() % 5 == 0);

    auto controllerYears = makeControllerYears(days, yearsToSettingsYears);
    setControllerYears(controllerYears);
    return true;
}

void Backend::generateControllerYears(int year)
{
    auto controllerYears = makeDefaultControllerYears(year);
    setControllerYears(controllerYears);
}

void Backend::saveToFile()
{
    if (m_controllerYears.isEmpty()) {
        return;
    }

    FileWriter fileWriter{"save.json"};

    auto days = getDays(m_controllerYears);
    auto settingsYears = getSettingsYears(m_controllerYears);

    auto writeOk = fileWriter.writeToFile(days, settingsYears);

    if (!writeOk) {
        qWarning() << Q_FUNC_INFO << "Save to file failed.";
    }
}

void Backend::setControllerYears(const QVector<QObject *> &controllerYears)
{
    if (m_controllerYears == controllerYears) {
        return;
    }

    for (auto &controllerYears : m_controllerYears) {
        controllerYears->deleteLater();
    }
    m_controllerYears.clear();
    m_controllerYears = controllerYears;
    for (auto &controllerYears : m_controllerYears) {
        controllerYears->setParent(this);
    }
    emit controllerYearsChanged();
}

} // namespace whm
