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
#ifndef WORKING_HOUR_MANAGER_FILE_WRITER_HPP
#define WORKING_HOUR_MANAGER_FILE_WRITER_HPP

#include <QObject>
#include <QString>

#include <array>
#include <memory>

namespace whm {

class Day;
class SettingsYear;

class FileWriter {
public:
    FileWriter(const QString &filename);

    FileWriter(const FileWriter &) = delete;
    FileWriter(FileWriter &&) = delete;
    FileWriter &operator=(const FileWriter &) = delete;
    FileWriter &operator=(FileWriter &&) = delete;

    ~FileWriter() = default;

    bool writeToFile(
        const QVector<std::shared_ptr<Day>> &days,
        const QVector<SettingsYear> &settingsYears);

private:
    QString m_filename;
};

} // namespace whm

#endif
