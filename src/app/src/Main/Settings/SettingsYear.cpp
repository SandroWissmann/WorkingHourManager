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
    : m_defaultWorkTimesMoToFr{{makeDefaultWorkTime(), makeDefaultWorkTime(), makeDefaultWorkTime(), makeDefaultWorkTime(), makeDefaultWorkTime()}},
      m_pauseTimesMoToFr{
          {makePauseTime(),
           makePauseTime(),
           makePauseTime(),
           makePauseTime(),
           makePauseTime()}}

{
}

SettingsYear::SettingsYear(
    const std::array<Time, 5> &defaultWorkTimesMoToFr,
    const std::array<Time, 5> &pauseTimesMoToFr)
    : m_defaultWorkTimesMoToFr{defaultWorkTimesMoToFr}, m_pauseTimesMoToFr{
                                                            pauseTimesMoToFr}
{
}

std::array<Time, 5> SettingsYear::defaultWorkTimesMoToFr() const
{
    return m_defaultWorkTimesMoToFr;
}

std::array<Time, 5> SettingsYear::pauseTimesMoToFr() const
{
    return m_pauseTimesMoToFr;
}

Time SettingsYear::defaultWorkTimeMonday() const
{
    return m_defaultWorkTimesMoToFr.at(0);
}

Time SettingsYear::defaultWorkTimeTuesday() const
{
    return m_defaultWorkTimesMoToFr.at(1);
}

Time SettingsYear::defaultWorkTimeWednesday() const
{
    return m_defaultWorkTimesMoToFr.at(2);
}
Time SettingsYear::defaultWorkTimeThursday() const
{
    return m_defaultWorkTimesMoToFr.at(3);
}

Time SettingsYear::defaultWorkTimeFriday() const
{
    return m_defaultWorkTimesMoToFr.at(4);
}

bool SettingsYear::setDefaultWorkTimeMonday(const QString &defaultWorkTime)
{
    return setTime(m_defaultWorkTimesMoToFr.at(0), defaultWorkTime);
}
bool SettingsYear::setDefaultWorkTimeTuesday(const QString &defaultWorkTime)
{
    return setTime(m_defaultWorkTimesMoToFr.at(1), defaultWorkTime);
}
bool SettingsYear::setDefaultWorkTimeWednesday(const QString &defaultWorkTime)
{
    return setTime(m_defaultWorkTimesMoToFr.at(2), defaultWorkTime);
}
bool SettingsYear::setDefaultWorkTimeThursday(const QString &defaultWorkTime)
{
    return setTime(m_defaultWorkTimesMoToFr.at(3), defaultWorkTime);
}
bool SettingsYear::setDefaultWorkTimeFriday(const QString &defaultWorkTime)
{
    return setTime(m_defaultWorkTimesMoToFr.at(4), defaultWorkTime);
}

Time SettingsYear::pauseTimeMonday() const
{
    return m_pauseTimesMoToFr.at(0);
}
Time SettingsYear::pauseTimeTuesday() const
{
    return m_pauseTimesMoToFr.at(1);
}
Time SettingsYear::pauseTimeWednesday() const
{
    return m_pauseTimesMoToFr.at(2);
}
Time SettingsYear::pauseTimeThursday() const
{
    return m_pauseTimesMoToFr.at(3);
}
Time SettingsYear::pauseTimeFriday() const
{
    return m_pauseTimesMoToFr.at(4);
}

bool SettingsYear::setPauseTimeMonday(const QString &pauseTime)
{
    return setTime(m_pauseTimesMoToFr.at(0), pauseTime);
}
bool SettingsYear::setPauseTimeTuesday(const QString &pauseTime)
{
    return setTime(m_pauseTimesMoToFr.at(1), pauseTime);
}
bool SettingsYear::setPauseTimeWednesday(const QString &pauseTime)
{
    return setTime(m_pauseTimesMoToFr.at(2), pauseTime);
}
bool SettingsYear::setPauseTimeThursday(const QString &pauseTime)
{
    return setTime(m_pauseTimesMoToFr.at(3), pauseTime);
}
bool SettingsYear::setPauseTimeFriday(const QString &pauseTime)
{
    return setTime(m_pauseTimesMoToFr.at(4), pauseTime);
}

} // namespace whm
