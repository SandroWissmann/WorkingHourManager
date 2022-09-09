#ifndef WORKING_HOUR_MANAGER_FILE_READER_HPP
#define WORKING_HOUR_MANAGER_FILE_READER_HPP

#include <QJsonDocument>
#include <QObject>
#include <QString>
#include <QVector>

#include <array>

namespace whm {

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

    std::map<int, SettingsYear> yearsToSettingsYears() const;

private:
    QJsonDocument m_jsonDocument;
    QVector<std::shared_ptr<Day>> m_days;
    std::map<int, SettingsYear> m_yearsToSettingsYears;
};

} // namespace whm

#endif
