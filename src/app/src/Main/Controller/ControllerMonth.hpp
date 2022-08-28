#ifndef WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP

#include <QDate>
#include <QObject>
#include <QVector>

#include <memory>

#include <whm/types/HoursAndMinutes.hpp>

namespace whm {

class Day;

class ControllerMonth : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
    Q_PROPERTY(QString nameOfMonth READ nameOfMonth CONSTANT)
    Q_PROPERTY(QString overtime READ overtime NOTIFY overtimeChanged)
public:
    ControllerMonth(const QVector<QObject *> &controllerWeeks);

    ControllerMonth(const ControllerMonth &) = delete;
    ControllerMonth(ControllerMonth &&) = delete;
    ControllerMonth &operator=(const ControllerMonth &) = delete;
    ControllerMonth &operator=(ControllerMonth &&) = delete;

    ~ControllerMonth() = default;

    QVector<QObject *> controllerWeeks() const;
    QString nameOfMonth() const;
    QString overtime() const;

    QVector<std::shared_ptr<Day>> days() const;

    int month() const;
    int year() const;

signals:
    void overtimeChanged();

private slots:
    void onOvertimeOfWeekChanged();

private:
    void makeControllerWeeksToThisConnections() const;

    void setOvertime(const HoursAndMinutes &overtime);

    QVector<QObject *> m_controllerWeeks;

    HoursAndMinutes m_overtime;
};
} // namespace whm

#endif
