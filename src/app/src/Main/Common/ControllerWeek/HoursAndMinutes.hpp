#ifndef WORKING_HOUR_MANAGER_HOURSANDMINUTES_HPP
#define WORKING_HOUR_MANAGER_HOURSANDMINUTES_HPP

#include <QDateTime>

namespace whm {

class HoursAndMinutes {
public:
    HoursAndMinutes() = default;
    explicit HoursAndMinutes(int minutes);
    HoursAndMinutes(int hours, int minutes);

    HoursAndMinutes(const HoursAndMinutes &) = default;
    HoursAndMinutes(HoursAndMinutes &&) = default;
    HoursAndMinutes &operator=(const HoursAndMinutes &) = default;
    HoursAndMinutes &operator=(HoursAndMinutes &&) = default;

    // format mm:hh
    QString toString() const;

    int hours() const;
    int minutes() const;

    HoursAndMinutes &operator+=(const HoursAndMinutes &rhs);
    HoursAndMinutes &operator-=(const HoursAndMinutes &rhs);

private:
    void setHours(int hours);
    void setMinutes(int minutes);

    int m_hours{};
    int m_minutes{};

    friend bool
    operator==(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);
    friend bool
    operator!=(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);

    friend HoursAndMinutes
    operator+(HoursAndMinutes lhs, const HoursAndMinutes &rhs);
    friend HoursAndMinutes
    operator-(HoursAndMinutes lhs, const HoursAndMinutes &rhs);
};

bool operator==(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);
bool operator!=(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);

HoursAndMinutes operator+(HoursAndMinutes lhs, const HoursAndMinutes &rhs);
HoursAndMinutes operator-(HoursAndMinutes lhs, const HoursAndMinutes &rhs);

} // namespace whm

#endif
