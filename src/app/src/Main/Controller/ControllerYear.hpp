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
    Q_PROPERTY(QString overtime READ overtimeAsString NOTIFY overtimeChanged)
    Q_PROPERTY(QString usedFlextimeDays READ usedFlextimeDaysAsString()
                   NOTIFY usedFlextimeDaysChanged)
    Q_PROPERTY(QString usedVacationDays READ usedVacationDaysAsString NOTIFY
                   usedVacationDaysChanged)
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
    QString overtimeAsString() const;
    QString usedFlextimeDaysAsString() const;
    QString usedVacationDaysAsString() const;

    QVector<std::shared_ptr<Day>> days() const;

signals:
    void overtimeChanged();
    void usedFlextimeDaysChanged();
    void usedVacationDaysChanged();

private slots:
    void onOvertimeOfMonthChanged();
    void onUsedFlextimeDaysOfMonthChanged();
    void onUsedVacationDaysOfMonthChanged();

private:
    QVector<QObject *> controllerDays() const;

    void makeControllerMonthsToThisConnections() const;

    void makeControllerSettingsYearToControllerDaysConnections() const;

    void setOvertime(const HoursAndMinutes &overtime);
    void setUsedFlextimeDays(double usedFlextimeDays);
    void setUsedVacationDays(double usedVacationDays);

    QVector<QObject *> m_controllerMonths;
    QObject *m_controllerSettingsYear;

    HoursAndMinutes m_overtime;
    double m_usedFlextimeDays;
    double m_usedVacationDays;
};
} // namespace whm

#endif
