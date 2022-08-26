#ifndef WORKING_HOUR_MANAGER_BACKEND_HPP
#define WORKING_HOUR_MANAGER_BACKEND_HPP

#include <whm/types/Date.hpp>
#include <whm/types/Time.hpp>

#include <QObject>
#include <QTime>
#include <QVector>

#include <array>

namespace whm {

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerYears READ controllerYears CONSTANT)
public:
    Backend(
        const QVector<QObject *> &controllerYears,
        // TODO: can be removed and taken from year->month->week->day controller
        const Time &defaultWorkedTimePerDay,
        const std::array<Time, 5> &pauseTimesPerDay,
        QObject *parent = nullptr);

    Backend(const Backend &) = delete;
    Backend(Backend &&) = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) = delete;

    ~Backend() = default;

    static Backend fromFile();

    QVector<QObject *> controllerMonths() const;

    QVector<QObject *> controllerWeeks() const;

    QVector<QObject *> controllerYears() const;

    Q_INVOKABLE
    void saveToFile();

private:
    Time m_defaultWorkedTimePerDay;
    std::array<Time, 5> m_pauseTimesPerDay;

    QVector<QObject *> m_controllerYears;
};
} // namespace whm

#endif
