#include <QtTest>
#include "../../src/InsulinDelivery.h"
#include "../../src/DeliveryEvent.h"
#include "../../src/PumpSystem.h"
#include "../../src/Profile.h"
#include "../../src/DataLogger.h"
#include "../../src/AlertSystem.h"

// Fake alert system that inherits from AlertSystem
class FakeAlertSystem : public AlertSystem {
public:
    using AlertSystem::AlertSystem; // inherit constructors

    bool occlusion = false;
    bool lowInsulin = false;

    void triggerOcclusionAlarm() override { occlusion = true; }
    void triggerLowInsulinAlert() override { lowInsulin = true; }
    void triggerLowBatteryWarning() override {}
};

// Logger that tracks if delivery was recorded
class MockLogger : public DataLogger {
public:
    MockLogger(QObject* parent = nullptr) : DataLogger(parent), logged(false) {}
    bool logged;
    float lastUnits = 0.0f;
    DeliveryEvent::EventType lastType;

    void logDeliveryEvent(const DeliveryEvent& event) override {
        logged = true;
        lastUnits = event.getUnits();
        lastType = event.getType();
    }
};

// Minimal PumpSystem replacement for unit testing
class MockPump : public PumpSystem {
public:
    MockPump(Profile* p)
        : PumpSystem(nullptr),
          profile(p),
          logger(new MockLogger()),
          alerts(new FakeAlertSystem()) {}

    // Override accessors to point to our mocks
    Profile* getCurrentProfile() const override { return profile; }
    DataLogger* getLogger() const override { return logger; }
    float getInsulinLevel() const override { return insulin; }
    void updateInsulinLevel(float u) override { insulin -= u; }
    bool isLocked() const override { return locked; }
    
    AlertSystem* getAlerts() const override { return alerts; }

    // Custom accessors for test assertions
    FakeAlertSystem* getFakeAlerts() const { return alerts; }
    MockLogger* getMockLogger() const { return logger; }

private:
    Profile* profile;
    MockLogger* logger;
    FakeAlertSystem* alerts;

    float insulin = 100.0f;
    bool locked = false;
};

class TestInsulinDelivery : public QObject {
    Q_OBJECT

private slots:
    // Test that starting basal delivery logs a BASAL event.
    void testStartBasalLogsEvent() {
        auto* profile = new Profile("T1", 1.0f, 15.0f, 50.0f);
        auto* pump = new MockPump(profile);
        InsulinDelivery delivery(pump);

        delivery.startBasal(1.2f);

        QVERIFY(pump->getMockLogger()->logged);
        QCOMPARE(pump->getMockLogger()->lastType, DeliveryEvent::BASAL);
        QCOMPARE(pump->getMockLogger()->lastUnits, 1.2f);
    }

    // Test that stopping basal delivery logs a SUSPEND event.
    void testStopBasalLogsSuspend() {
        auto* profile = new Profile("T2", 1.0f, 15.0f, 50.0f);
        auto* pump = new MockPump(profile);
        InsulinDelivery delivery(pump);

        delivery.stopBasal();

        QVERIFY(pump->getMockLogger()->logged);
        QCOMPARE(pump->getMockLogger()->lastType, DeliveryEvent::SUSPEND);
        QCOMPARE(pump->getMockLogger()->lastUnits, 0.0f);
    }

    // Test that delivering a bolus with zero units triggers occlusion alert.
    void testDeliverBolusTriggersOcclusionForZeroUnits() {
        auto* profile = new Profile("T3", 1.0f, 15.0f, 50.0f);
        auto* pump = new MockPump(profile);
        InsulinDelivery delivery(pump);

        delivery.deliverBolus(0.0f);
        QVERIFY(pump->getFakeAlerts()->occlusion);
    }

    // Test that delivering a bolus when insulin is low triggers low insulin alert.
    void testDeliverBolusTriggersLowInsulin() {
        auto* profile = new Profile("T4", 1.0f, 15.0f, 50.0f);
        auto* pump = new MockPump(profile);
        pump->updateInsulinLevel(99.0f); // Only 1.0f left
        InsulinDelivery delivery(pump);

        delivery.deliverBolus(5.0f);
        QVERIFY(pump->getFakeAlerts()->lowInsulin);
    }

    // Test the bolus calculation logic.
    void testCalculateBolusLogic() {
        auto* profile = new Profile("T5", 1.0f, 10.0f, 30.0f); // carbRatio = 10, correctionFactor = 30
        auto* pump = new MockPump(profile);
        InsulinDelivery delivery(pump);

        float result = delivery.calculateBolus(190.0f, 50.0f);
        float expected = (50.0f / 10.0f) + ((190.0f - 100.0f) / 30.0f);
        QVERIFY(qAbs(result - expected) < 0.01f);
    }
};

QTEST_MAIN(TestInsulinDelivery)
#include "test_insulindelivery.moc"

