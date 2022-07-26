#ifndef WORKING_HOUR_MANAGER_CONTROLLER_YEAR_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_YEAR_HPP

#include <QObject>
#include <QVector>

namespace whm {

class Day;

class ControllerYear : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QVector<QObject *> controllerMonths READ controllerMonths CONSTANT)
    Q_PROPERTY(int year READ year CONSTANT)
public:
    ControllerYear(const QVector<QObject *> &controllerMonths);

    ControllerYear(const ControllerYear &) = delete;
    ControllerYear(ControllerYear &&) = delete;
    ControllerYear &operator=(const ControllerYear &) = delete;
    ControllerYear &operator=(ControllerYear &&) = delete;

    ~ControllerYear() = default;

    QVector<QObject *> controllerMonths() const;

    int year() const;

    QVector<std::shared_ptr<Day>> days() const;

private:
    QVector<QObject *> m_controllerMonths;
};
} // namespace whm

#endif
