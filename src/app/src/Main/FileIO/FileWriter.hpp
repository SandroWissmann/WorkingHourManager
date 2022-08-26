#ifndef WORKING_HOUR_MANAGER_FILE_WRITER_HPP
#define WORKING_HOUR_MANAGER_FILE_WRITER_HPP

#include <QObject>
#include <QString>

#include <array>
#include <memory>

namespace whm {

class Time;
class Day;

class FileWriter {
public:
    FileWriter(const QString &filename);

    FileWriter(const FileWriter &) = delete;
    FileWriter(FileWriter &&) = delete;
    FileWriter &operator=(const FileWriter &) = delete;
    FileWriter &operator=(FileWriter &&) = delete;

    ~FileWriter() = default;

    bool writeToFile(
        const Time &defaultWorkedTimePerDay,
        const std::array<Time, 5> &pauseTimesPerDay,
        const QVector<std::shared_ptr<Day>> &days);

private:
    QString m_filename;
};

} // namespace whm

#endif
