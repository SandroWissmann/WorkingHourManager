#ifndef WORKING_HOUR_MANAGER_BACKEND_HPP
#define WORKING_HOUR_MANAGER_BACKEND_HPP

#include <QObject>
#include <QVector>

#include <array>

namespace whm {

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerYears READ controllerYears CONSTANT)
public:
    Backend(
        const QVector<QObject *> &controllerYears,
        QObject *parent = nullptr);

    Backend(const Backend &) = delete;
    Backend(Backend &&) = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) = delete;

    ~Backend() = default;

    static Backend fromFile();

    QVector<QObject *> controllerYears() const;

    Q_INVOKABLE
    void saveToFile();

private:
    QVector<QObject *> m_controllerYears;
};
} // namespace whm

#endif
