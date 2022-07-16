#include "../include/whm/types/HoursAndMinutes.hpp"

namespace whm {

namespace {

QString toStringWith2Digits(int hoursOrMinutes)
{
    return QStringLiteral("%1").arg(hoursOrMinutes, 2, 10, QLatin1Char('0'));
}

} // namespace

HoursAndMinutes::HoursAndMinutes(int hours, int minutes)
    : m_hours{(hours)}, m_minutes{(minutes)}
{
}

HoursAndMinutes::HoursAndMinutes(int minutes)
    : m_hours{minutes / 60}, m_minutes{minutes - m_hours * 60}
{
}

QString HoursAndMinutes::toString() const
{
    auto hourString = toStringWith2Digits(m_hours);
    auto minuteString = toStringWith2Digits(m_minutes);
    QString resultString{hourString + ":" + minuteString};
    resultString.remove('-');
    if (m_minutes < 0 || m_hours < 0) {
        resultString.push_front('-');
    }
    return resultString;
}

int HoursAndMinutes::hours() const
{
    return m_hours;
}

int HoursAndMinutes::minutes() const
{
    return m_minutes;
}

void HoursAndMinutes::setHours(int hours)
{
    if (m_hours == hours) {
        return;
    }
    m_hours = hours;
}

void HoursAndMinutes::setMinutes(int minutes)
{
    if (m_minutes == minutes) {
        return;
    }
    m_minutes = minutes;
}

HoursAndMinutes &HoursAndMinutes::operator+=(const HoursAndMinutes &rhs)
{
    auto minutesLhs = hours() * 60 + minutes();
    auto minutesRhs = rhs.hours() * 60 + rhs.minutes();
    auto minutes = minutesLhs + minutesRhs;
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;
    setHours(hours);
    setMinutes(minutes);
    return *this;
}

HoursAndMinutes &HoursAndMinutes::operator-=(const HoursAndMinutes &rhs)
{
    auto minutesLhs = hours() * 60 + minutes();
    auto minutesRhs = rhs.hours() * 60 + rhs.minutes();
    auto minutes = minutesLhs - minutesRhs;
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;
    setHours(hours);
    setMinutes(minutes);
    return *this;
}

bool operator==(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs)
{
    if (lhs.hours() != rhs.hours()) {
        return false;
    }
    if (lhs.minutes() != rhs.minutes()) {
        return false;
    }
    return true;
}

bool operator!=(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs)
{
    return !(lhs == rhs);
}

HoursAndMinutes operator+(HoursAndMinutes lhs, const HoursAndMinutes &rhs)
{
    lhs += rhs;
    return lhs;
}

HoursAndMinutes operator-(HoursAndMinutes lhs, const HoursAndMinutes &rhs)
{
    lhs -= rhs;
    return lhs;
}

} // namespace whm
