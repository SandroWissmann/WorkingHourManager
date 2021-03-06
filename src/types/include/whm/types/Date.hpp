#ifndef WORKING_HOUR_MANAGER_DATE_HPP
#define WORKING_HOUR_MANAGER_DATE_HPP

#include <QDate>

namespace whm {

class Date {
public:
    static Date currentDate();

    Date(int year, int month, int day);

    // dd.MM.yyyy
    Date(const QString date);

    Date(const Date &) = default;
    Date(Date &&) = default;
    Date &operator=(const Date &) = default;
    Date &operator=(Date &&) = default;

    // format dd.MM.yyyy
    QString asString() const;

    // Monday / Tuesday etc.
    QString weekday() const;

    int month() const;

    int year() const;

    Date addDays(int days) const;

    Date removeDays(int days) const;

    Date getPreviouseDateWithDayOfWeek(int dayOfWeek) const;

private:
    QDate m_date;

    friend bool operator<(const Date &lhs, const Date &rhs);
};

bool operator==(const Date &lhs, const Date &rhs);
bool operator!=(const Date &lhs, const Date &rhs);

bool operator<(const Date &lhs, const Date &rhs);
bool operator>(const Date &lhs, const Date &rhs);
bool operator<=(const Date &lhs, const Date &rhs);
bool operator>=(const Date &lhs, const Date &rhs);

} // namespace whm

#endif
