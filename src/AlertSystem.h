#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H

#include <QObject>

class AlertSystem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AlertSystem)

public:
    static AlertSystem* instance();
    virtual void triggerOcclusionAlarm();
    virtual void triggerLowInsulinAlert();
    virtual void triggerLowBatteryWarning();

    bool isOcclusionAlarmActive() const;
    bool isLowInsulinAlarmActive() const;
    bool isLowBatteryAlarmActive() const;

signals:
    void alertTriggered(QString message);

public slots:
    void clearAlerts() {
        occlusionAlarmActive = false;
        lowInsulinAlarmActive = false;
        lowBatteryAlarmActive = false;
    }


protected:
    explicit AlertSystem(QObject *parent = nullptr);
    static AlertSystem* m_instance;

    bool occlusionAlarmActive = false;
    bool lowInsulinAlarmActive = false;
    bool lowBatteryAlarmActive = false;
};

#endif // ALERTSYSTEM_H
