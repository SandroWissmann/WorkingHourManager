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

SettingsYear::SettingsYear()
    : m_weekdayToSettingsDay{makeDefaultWeekdayToSettingsDay()}
{
}

SettingsYear::SettingsYear(std::map<Weekday, SettingsDay> weekdayToSettingsDay)
    : m_weekdayToSettingsDay{weekdayToSettingsDay}
{
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

} // namespace whm
