#ifndef WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP

#include <QDate>
#include <QObject>
#include <QVector>

namespace whm {

class ControllerWeek;

class ControllerMonth : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
public:
    ControllerMonth(QObject *parent = nullptr);

    ControllerMonth(const ControllerMonth &) = delete;
    ControllerMonth(ControllerMonth &&) = delete;
    ControllerMonth &operator=(const ControllerMonth &) = delete;
    ControllerMonth &operator=(ControllerMonth &&) = delete;

    ~ControllerMonth() = default;

    QVector<QObject *> controllerWeeks() const;

    // assume here ControllerWeek memory is managed by someone else
    void insertControllerWeek(ControllerWeek *controllerWeek);

    int month() const;

private:
    QVector<QObject *> m_controllerWeeks;
    int m_month{-1};
};
} // namespace whm

#endif
