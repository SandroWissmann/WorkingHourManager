#include "SettingsYear.hpp"

namespace whm {

namespace {

Time makeDefaultWorkTime()
{
    return Time{7, 45};
}

Time makePauseTime()
{
    return Time{0, 45};
}

SettingsDay makeDefaultSettingsDay(Weekday weekday)
{
    return SettingsDay{weekday, makeDefaultWorkTime(), makePauseTime()};
}

std::map<Weekday, SettingsDay> makeDefaultWeekdayToSettingsDay()
{
    return {
        {Weekday::monday, makeDefaultSettingsDay(Weekday::monday)},
        {Weekday::tuesday, makeDefaultSettingsDay(Weekday::tuesday)},
        {Weekday::wednesday, makeDefaultSettingsDay(Weekday::wednesday)},
        {Weekday::thursday, makeDefaultSettingsDay(Weekday::thursday)},
        {Weekday::friday, makeDefaultSettingsDay(Weekday::friday)}};
}

bool setTime(Time &value, const QString &newValue)
{
    if (value.asString() == newValue) {
        return false;
    }
    value.set(newValue);
    return true;
}

} // namespace

SettingsYear::SettingsYear(int year)
    : m_year{year}, m_flextimeDays{defaultFlextimneDays()},
      m_vacationDays{defaultVacationDays()},
      m_weekdayToSettingsDay{makeDefaultWeekdayToSettingsDay()}
{
}

SettingsYear::SettingsYear(
    int year,
    double flextimeDays,
    double vacationDays,
    std::map<Weekday, SettingsDay> weekdayToSettingsDay)
    : m_weekdayToSettingsDay{weekdayToSettingsDay},
      m_flextimeDays{flextimeDays}, m_vacationDays{vacationDays}, m_year{year}
{
    Q_ASSERT(m_flextimeDays > 0);
    Q_ASSERT(m_vacationDays > 0);

    Q_ASSERT(m_weekdayToSettingsDay.size() == 5);
    Q_ASSERT(
        m_weekdayToSettingsDay.find(Weekday::monday) !=
        m_weekdayToSettingsDay.end());
    Q_ASSERT(
        m_weekdayToSettingsDay.find(Weekday::tuesday) !=
        m_weekdayToSettingsDay.end());
    Q_ASSERT(
        m_weekdayToSettingsDay.find(Weekday::wednesday) !=
        m_weekdayToSettingsDay.end());
    Q_ASSERT(
        m_weekdayToSettingsDay.find(Weekday::thursday) !=
        m_weekdayToSettingsDay.end());
    Q_ASSERT(
        m_weekdayToSettingsDay.find(Weekday::friday) !=
        m_weekdayToSettingsDay.end());
}

int SettingsYear::year() const
{
    return m_year;
}

std::map<Weekday, SettingsDay> SettingsYear::weekdayToSettingsDay() const
{
    return m_weekdayToSettingsDay;
}

SettingsDay *SettingsYear::settingsDay(Weekday weekday)
{
    Q_ASSERT(
        m_weekdayToSettingsDay.find(weekday) != m_weekdayToSettingsDay.end());
    return &m_weekdayToSettingsDay.at(weekday);
}

std::array<Time, 5> SettingsYear::defaultWorkTimesMoToFr() const
{
    return {
        m_weekdayToSettingsDay.at(Weekday::monday).defaultWorkTime(),
        m_weekdayToSettingsDay.at(Weekday::tuesday).defaultWorkTime(),
        m_weekdayToSettingsDay.at(Weekday::wednesday).defaultWorkTime(),
        m_weekdayToSettingsDay.at(Weekday::thursday).defaultWorkTime(),
        m_weekdayToSettingsDay.at(Weekday::friday).defaultWorkTime()};
}

std::array<Time, 5> SettingsYear::pauseTimesMoToFr() const
{
    return {
        m_weekdayToSettingsDay.at(Weekday::monday).pauseTime(),
        m_weekdayToSettingsDay.at(Weekday::tuesday).pauseTime(),
        m_weekdayToSettingsDay.at(Weekday::wednesday).pauseTime(),
        m_weekdayToSettingsDay.at(Weekday::thursday).pauseTime(),
        m_weekdayToSettingsDay.at(Weekday::friday).pauseTime()};
}

Time SettingsYear::defaultWorkTime(Weekday weekday) const
{
    return m_weekdayToSettingsDay.at(weekday).defaultWorkTime();
}

bool SettingsYear::setDefaultWorkTime(
    Weekday weekday,
    const QString &defaultWorkTimeAsString)
{
    if (m_weekdayToSettingsDay.find(weekday) == m_weekdayToSettingsDay.end()) {
        return false;
    }
    auto settingsYear = m_weekdayToSettingsDay.at(weekday);

    if (settingsYear.defaultWorkTime().asString() == defaultWorkTimeAsString) {
        return false;
    }
    settingsYear.setDefaultWorkTime(defaultWorkTimeAsString);
    m_weekdayToSettingsDay.at(weekday) = settingsYear;
    return true;
}

Time SettingsYear::pauseTime(Weekday weekday) const
{
    return m_weekdayToSettingsDay.at(weekday).pauseTime();
}

bool SettingsYear::setPauseTime(
    Weekday weekday,
    const QString &pauseTimeAsString)
{
    if (m_weekdayToSettingsDay.find(weekday) == m_weekdayToSettingsDay.end()) {
        return false;
    }
    auto settingsYear = m_weekdayToSettingsDay.at(weekday);

    if (settingsYear.pauseTime().asString() == pauseTimeAsString) {
        return false;
    }
    settingsYear.setPauseTime(pauseTimeAsString);
    m_weekdayToSettingsDay.at(weekday) = settingsYear;
    return true;
}

double SettingsYear::flextimeDays() const
{
    return m_flextimeDays;
}

bool SettingsYear::setFlextimeDays(const QString &flextimeDaysAsString)
{
    bool ok;
    auto flextimeDays = flextimeDaysAsString.toDouble(&ok);
    Q_ASSERT(ok);

    if (qFuzzyCompare(m_flextimeDays, flextimeDays)) {
        return false;
    }
    m_flextimeDays = flextimeDays;
    return true;
}

double SettingsYear::vacationDays() const
{
    return m_vacationDays;
}

bool SettingsYear::setVacationDays(const QString &vacationDaysAsString)
{
    bool ok;
    auto vacationDays = vacationDaysAsString.toDouble(&ok);
    Q_ASSERT(ok);

    if (qFuzzyCompare(m_vacationDays, vacationDays)) {
        return false;
    }
    m_vacationDays = vacationDays;
    return true;
}
} // namespace whm
