#ifndef WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP

#include <QDate>
#include <QObject>
#include <QVector>

#include <whm/types/HoursAndMinutes.hpp>

namespace whm {

class Day;

class ControllerMonth : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
    Q_PROPERTY(QString nameOfMonth READ nameOfMonth CONSTANT)
    Q_PROPERTY(QString overtime READ overtimeAsString NOTIFY overtimeChanged)
public:
    ControllerMonth(const QVector<QObject *> &controllerWeeks);

    ControllerMonth(const ControllerMonth &) = delete;
    ControllerMonth(ControllerMonth &&) = delete;
    ControllerMonth &operator=(const ControllerMonth &) = delete;
    ControllerMonth &operator=(ControllerMonth &&) = delete;

    ~ControllerMonth() = default;

    QVector<QObject *> controllerWeeks() const;

    QVector<QObject *> controllerDays() const;

    QString nameOfMonth() const;

    HoursAndMinutes overtime() const;
    QString overtimeAsString() const;

    double usedFlextimeDays() const;
    double usedVacationDays() const;

    QVector<std::shared_ptr<Day>> days() const;

    int month() const;
    int year() const;

signals:
    void overtimeChanged();
    void usedFlextimeDaysChanged();
    void usedVacationDaysChanged();

private slots:
    void onOvertimeOfWeekChanged();
    void onUsedFlextimeDaysOfWeekChanged();
    void onUsedVacationDaysOfWeekChanged();

private:
    void makeControllerWeeksToThisConnections() const;

    void setOvertime(const HoursAndMinutes &overtime);
    void setUsedFlextimeDays(double usedFlextimeDays);
    void setUsedVacationDays(double usedVacationDays);

    QVector<QObject *> m_controllerWeeks;

    HoursAndMinutes m_overtime;
    double m_usedFlextimeDays;
    double m_usedVacationDays;
};
} // namespace whm

#endif
