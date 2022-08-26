#include "../include/whm/types/Time.hpp"

namespace whm {

namespace {
QTime stringToTime(QString hhmmStr)
{
    return QTime::fromString(hhmmStr, "h:mm");
}

QString timeToString(const QTime &time)
{
    return time.toString("h:mm");
}

QTime minutesToTime(int minutes)
{
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;
    return QTime{hours, minutes};
}

} // namespace

Time::Time() : m_time{0, 0}
{
}

Time::Time(int minutes) : m_time{minutesToTime(minutes)}
{
}

Time::Time(int hour, int minute) : m_time{hour, minute}
{
}

Time::Time(const QString &hhmmStr) : m_time{stringToTime(hhmmStr)}
{
}

int Time::hour() const
{
    return m_time.hour();
}

int Time::minute() const
{
    return m_time.minute();
}

QString Time::asString() const
{
    return timeToString(m_time);
}

bool Time::set(const QString &timeAsString)
{
    if (asString() == timeAsString) {
        return false;
    }
    m_time = stringToTime(timeAsString);
    return true;
}

bool Time::isValid() const
{
    if (asString() == "0:00") {
        return false;
    }
    return true;
}

int Time::totalMinutes() const
{
    if (!isValid()) {
        return 0;
    }
    return m_time.hour() * 60 + m_time.minute();
}

Time &Time::operator+=(const Time &rhs)
{
    auto minutes = totalMinutes() + rhs.totalMinutes();
    *this = Time{minutes};
    return *this;
}

Time &Time::operator-=(const Time &rhs)
{
    auto minutes = totalMinutes() - rhs.totalMinutes();
    *this = Time{minutes};
    return *this;
}

bool operator==(const Time &lhs, const Time &rhs)
{
    return lhs.asString() == rhs.asString();
}

bool operator!=(const Time &lhs, const Time &rhs)
{
    return !(lhs == rhs);
}

Time operator+(Time lhs, const Time &rhs)
{
    lhs += rhs;
    return lhs;
}

Time operator-(Time lhs, const Time &rhs)
{
    lhs -= rhs;
    return lhs;
}

bool operator<(const Time &lhs, const Time &rhs)
{
    return lhs.totalMinutes() < rhs.totalMinutes();
}

bool operator>(const Time &lhs, const Time &rhs)
{
    return rhs < lhs;
}

bool operator<=(const Time &lhs, const Time &rhs)
{
    return !(lhs > rhs);
}
bool operator>=(const Time &lhs, const Time &rhs)
{
    return !(lhs < rhs);
}

} // namespace whm
