#ifndef WORKING_HOUR_MANAGER_BACKEND_HPP
#define WORKING_HOUR_MANAGER_BACKEND_HPP

#include <QObject>
#include <QTime>
#include <QVector>

namespace whm {

class Backend : public QObject {
    Q_OBJECT
    // tmp remove when frontend is rdy to handle years / months / etc
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
    Q_PROPERTY(QVector<QObject *> controllerYears READ controllerYears CONSTANT)
public:
    // Expect weeks contain the right years
    Backend(
        const QDate &firstDate,
        QTime defaultWorkTimePerDay,
        QTime pauseTimeMonday,
        QTime pauseTimeTuesday,
        QTime pauseTimeWednesday,
        QTime pauseTimeThursday,
        QTime pauseTimeFriday,
        QObject *parent = nullptr);

    QVector<QObject *> controllerMonths() const;

    Backend(const Backend &) = delete;
    Backend(Backend &&) = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) = delete;

    ~Backend() = default;

    QVector<QObject *> controllerWeeks() const;

    QVector<QObject *> controllerYears() const;

private:
    QVector<QObject *> m_controllerWeeks;
    QVector<QObject *> m_controllerYears;
};
} // namespace whm

#endif
