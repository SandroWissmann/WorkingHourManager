#ifndef WORKING_HOUR_MANAGER_BACKEND_HPP
#define WORKING_HOUR_MANAGER_BACKEND_HPP

#include "Support/Date.hpp"
#include "Support/Time.hpp"

#include <QObject>
#include <QTime>
#include <QVector>

#include <array>

namespace whm {

class Backend : public QObject {
    Q_OBJECT
    // tmp remove when frontend is rdy to handle years / months / etc
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
    Q_PROPERTY(QVector<QObject *> controllerYears READ controllerYears CONSTANT)
public:
    // Expect weeks contain the right years
    Backend(
        const Date &firstDate,
        const Time &defaultWorkTimePerDay,
        const std::array<Time, 5> &pauseTimesPerDay,
        QObject *parent = nullptr);

    Backend(
        const QVector<QObject *> &controllerWeeks,
        const Time &defaultWorkTimePerDay,
        const std::array<Time, 5> &pauseTimesPerDay,
        QObject *parent = nullptr);

    static Backend fromFile();

    QVector<QObject *> controllerMonths() const;

    Backend(const Backend &) = delete;
    Backend(Backend &&) = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) = delete;

    ~Backend() = default;

    QVector<QObject *> controllerWeeks() const;

    QVector<QObject *> controllerYears() const;

    Q_INVOKABLE
    void saveToFile();

private:
    Time m_defaultWorkTimePerDay;
    std::array<Time, 5> m_pauseTimesPerDay;

    QVector<QObject *> m_controllerWeeks;
    QVector<QObject *> m_controllerYears;
};
} // namespace whm

#endif
