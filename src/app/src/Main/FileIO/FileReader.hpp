#ifndef WORKING_HOUR_MANAGER_FILE_READER_HPP
#define WORKING_HOUR_MANAGER_FILE_READER_HPP

#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonDocument>

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

    Time defaultWorkTimePerDay() const;

    QVector<int> holidaysPerYear() const;

    std::array<Time, 5> pauseTimesPerDay() const;

    QVector<Day> days() const;

private:
    QJsonDocument m_jsonDocument;
};

}

#endif
