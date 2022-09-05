#ifndef WORKING_HOUR_MANAGER_BACKEND_HPP
#define WORKING_HOUR_MANAGER_BACKEND_HPP

#include <QObject>
#include <QVector>

#include <array>

namespace whm {

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerYears READ controllerYears NOTIFY
                   controllerYearsChanged)
    Q_PROPERTY(int currentYear READ currentYear CONSTANT)
public:
    Backend(QObject *parent = nullptr);

    Backend(const Backend &) = delete;
    Backend(Backend &&) = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) = delete;

    ~Backend() = default;

    QVector<QObject *> controllerYears() const;

    int currentYear() const;

    Q_INVOKABLE
    bool readControllerYearsFromFile();

    Q_INVOKABLE
    void generateControllerYears(int year);

    Q_INVOKABLE
    void saveToFile();

signals:
    void controllerYearsChanged();

private:
    void setControllerYears(const QVector<QObject *> &controllerYears);

    QVector<QObject *> m_controllerYears;
};
} // namespace whm

#endif
