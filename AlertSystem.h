#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H

#include <QObject>

class AlertSystem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AlertSystem)

public:
    static AlertSystem* instance();
    void triggerOcclusionAlarm();
    void triggerLowInsulinAlert();
    void triggerLowBatteryWarning();

    bool isOcclusionAlarmActive() const;  // For safety checks

private:
    explicit AlertSystem(QObject *parent = nullptr);
    static AlertSystem* m_instance;

    bool occlusionAlarmActive = false;
};

#endif // ALERTSYSTEM_H
