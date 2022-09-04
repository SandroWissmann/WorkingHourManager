#ifndef WORKING_HOUR_MANAGER_FILE_READER_HPP
#define WORKING_HOUR_MANAGER_FILE_READER_HPP

#include <QJsonDocument>
#include <QObject>
#include <QString>
#include <QVector>

#include <array>

namespace whm {

class Time;
class Day;
class SettingsYear;

class FileReader {
public:
    FileReader(const QString &filename);

    FileReader(const FileReader &) = delete;
    FileReader(FileReader &&) = delete;
    FileReader &operator=(const FileReader &) = delete;
    FileReader &operator=(FileReader &&) = delete;

    ~FileReader() = default;

    bool isValidFile() const;

    QVector<std::shared_ptr<Day>> days() const;

    QVector<SettingsYear> settingsYears() const;

private:
    QJsonDocument m_jsonDocument;
    QVector<std::shared_ptr<Day>> m_days;
    QVector<SettingsYear> m_settingsYears;
};

} // namespace whm

#endif
