#ifndef WORKING_HOUR_MANAGER_TIME_HPP
#define WORKING_HOUR_MANAGER_TIME_HPP

#include <QTime>

namespace whm {

class Time {
public:
    Time();

    explicit Time(const QString &hhmmStr);
    Time(int hour, int minute);
    explicit Time(int minutes);

    Time(const Time &) = default;
    Time(Time &&) = default;
    Time &operator=(const Time &) = default;
    Time &operator=(Time &&) = default;

    int hour() const;

    int minute() const;

    QString asString() const;

    bool set(const QString &timeAsString);

    bool isValid() const;

    // time only represented with minutes
    int totalMinutes() const;

    Time &operator+=(const Time &rhs);
    Time &operator-=(const Time &rhs);

private:
    QTime m_time;
};

bool operator==(const Time &lhs, const Time &rhs);
bool operator!=(const Time &lhs, const Time &rhs);

Time operator+(Time lhs, const Time &rhs);
Time operator-(Time lhs, const Time &rhs);

bool operator<(const Time &lhs, const Time &rhs);
bool operator>(const Time &lhs, const Time &rhs);
bool operator<=(const Time &lhs, const Time &rhs);
bool operator>=(const Time &lhs, const Time &rhs);

} // namespace whm

#endif
