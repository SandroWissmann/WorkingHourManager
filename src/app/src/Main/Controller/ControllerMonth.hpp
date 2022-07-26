#ifndef WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP

#include <QDate>
#include <QObject>
#include <QVector>

#include <memory>

namespace whm {

class Day;

class ControllerMonth : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
    Q_PROPERTY(int month READ month CONSTANT)
public:
    ControllerMonth(const QVector<QObject *> &controllerWeeks);

    ControllerMonth(const ControllerMonth &) = delete;
    ControllerMonth(ControllerMonth &&) = delete;
    ControllerMonth &operator=(const ControllerMonth &) = delete;
    ControllerMonth &operator=(ControllerMonth &&) = delete;

    ~ControllerMonth() = default;

    QVector<QObject *> controllerWeeks() const;

    QVector<std::shared_ptr<Day>> days() const;

    int month() const;

    int year() const;

private:
    QVector<QObject *> m_controllerWeeks;
};
} // namespace whm

#endif
