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

class FileReader {
public:
    FileReader(const QString &filename);

    FileReader(const FileReader &) = delete;
    FileReader(FileReader &&) = delete;
    FileReader &operator=(const FileReader &) = delete;
    FileReader &operator=(FileReader &&) = delete;

    ~FileReader() = default;

    bool isValidFile() const;

    Time defaultWorkedTimePerDay() const;

    QVector<int> holidaysPerYear() const;

    std::array<Time, 5> pauseTimesPerDay() const;

    QVector<std::shared_ptr<Day>> days() const;

private:
    QJsonDocument m_jsonDocument;
};

} // namespace whm

#endif
