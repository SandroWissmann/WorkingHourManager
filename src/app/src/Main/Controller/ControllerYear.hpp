#ifndef WORKING_HOUR_MANAGER_CONTROLLER_YEAR_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_YEAR_HPP

#include <QObject>
#include <QVector>

#include <whm/types/HoursAndMinutes.hpp>

namespace whm {

class Day;

class ControllerYear : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QVector<QObject *> controllerMonths READ controllerMonths CONSTANT)
    Q_PROPERTY(
        QObject *controllerSettinsYear READ controllerSettinsYear CONSTANT)
    Q_PROPERTY(int year READ year CONSTANT)
    Q_PROPERTY(QString overtime READ overtime NOTIFY overtimeChanged)
public:
    ControllerYear(
        const QVector<QObject *> &controllerMonths,
        QObject *controllerSettingsYear);

    ControllerYear(const ControllerYear &) = delete;
    ControllerYear(ControllerYear &&) = delete;
    ControllerYear &operator=(const ControllerYear &) = delete;
    ControllerYear &operator=(ControllerYear &&) = delete;

    ~ControllerYear() = default;

    QVector<QObject *> controllerMonths() const;
    QObject *controllerSettinsYear() const;
    int year() const;
    QString overtime() const;

    QVector<std::shared_ptr<Day>> days() const;

signals:
    void overtimeChanged();

private slots:
    void onOvertimeOfMonthChanged();

private:
    QVector<QObject *> controllerDays() const;

    void makeControllerMonthsToThisConnections() const;

    void makeControllerSettingsYearToControllerDaysConnections() const;

    void setOvertime(const HoursAndMinutes &overtime);

    QVector<QObject *> m_controllerMonths;
    QObject *m_controllerSettingsYear;

    HoursAndMinutes m_overtime;
};
} // namespace whm

#endif
