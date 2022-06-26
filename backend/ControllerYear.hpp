#ifndef WORKING_HOUR_MANAGER_CONTROLLER_YEAR_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_YEAR_HPP

#include <QObject>
#include <QVector>

namespace whm {

class ControllerMonth;
class ControllerWeek;

class ControllerYear : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QVector<QObject *> controllerMonths READ controllerMonths CONSTANT)
public:
    // Expect weeks contain the right years
    ControllerYear(
        const QVector<QObject *> &controllerWeeks,
        QObject *parent = nullptr);

    ControllerYear(const ControllerMonth &) = delete;
    ControllerYear(ControllerMonth &&) = delete;
    ControllerYear &operator=(const ControllerMonth &) = delete;
    ControllerYear &operator=(ControllerMonth &&) = delete;

    ~ControllerYear() = default;

    QVector<QObject *> controllerMonths() const;

private:
    QVector<QObject *> m_controllerMonths;
};
} // namespace whm

#endif
