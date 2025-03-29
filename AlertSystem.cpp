#include "AlertSystem.h"

AlertSystem* AlertSystem::m_instance = nullptr;

AlertSystem* AlertSystem::instance() {
    if (!m_instance) m_instance = new AlertSystem();
    return m_instance;
}

void AlertSystem::triggerOcclusionAlarm() {
    occlusionAlarmActive = true;
    emit alertTriggered("Occlusion detected, check tubing...");
}

void AlertSystem::triggerLowInsulinAlert() {
    lowInsulinAlarmActive = true;
    emit alertTriggered("Insulin low");
}

void AlertSystem::triggerLowBatteryWarning() {
    lowBatteryAlarmActive = true;
    emit alertTriggered("Battery low, charge");
}

bool AlertSystem::isOcclusionAlarmActive() const {return occlusionAlarmActive;}
bool AlertSystem::isLowInsulinAlarmActive() const {return lowInsulinAlarmActive;}
bool AlertSystem::isLowBatteryAlarmActive() const {return lowBatteryAlarmActive;}
