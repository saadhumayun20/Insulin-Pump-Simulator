#include <QtTest>
#include "../src/ControlIQ.h"
#include "../src/DeliveryEvent.h"
#include "../src/Profile.h"
#include "../src/InsulinDelivery.h"
#include "../src/DataLogger.h"
#include "../src/CGM.h"

// Mock CGM that returns custom glucose and trend values
class MockCGM : public CGM {
    Q_OBJECT
public:
    MockCGM(QObject* parent = nullptr) : CGM(parent) {}

    float glucose = 100.0f;
    float trend = 0.0f;

    float readGlucose() const override { return glucose; }
    float getTrend() const override { return trend; }
};

// Minimal mock InsulinDelivery
class MockDelivery : public InsulinDelivery {
public:
    MockDelivery(QObject* parent = nullptr) : InsulinDelivery(nullptr) {}

    float lastBasal = -1.0f;
    float lastBolus = -1.0f;
    bool stopCalled = false;

    void startBasal(float rate) { lastBasal = rate; }
    void deliverBolus(float units) { lastBolus = units; }
    void stopBasal() { stopCalled = true; }
};

// Mock logger that sets a flag when a delivery is logged
class MockLogger : public DataLogger {
public:
    MockLogger(QObject* parent = nullptr) : DataLogger(parent) {}
    bool logged = false;

    void logDeliveryEvent(const DeliveryEvent&) { logged = true; }
};

// Mock PumpSystem that connects all dependencies (no inheritance from PumpSystem)
class MockPumpSystem : public QObject {
    Q_OBJECT
public:
    MockPumpSystem(Profile* p, MockDelivery* d, MockLogger* l)
        : profile(p), delivery(d), logger(l) {}

    Profile* getCurrentProfile() const { return profile; }
    InsulinDelivery* getDeliverySystem() const { return delivery; }
    DataLogger* getLogger() const { return logger; }

private:
    Profile* profile;
    MockDelivery* delivery;
    MockLogger* logger;
};

// Test class for ControlIQ
class TestControlIQ : public QObject {
    Q_OBJECT

private slots:

    // Should not adjust basal or bolus if glucose prediction is within normal range
    void testNoActionWithinTarget() {
        auto* cgm = new MockCGM();
        cgm->glucose = 130.0f;
        cgm->trend = 0.0f;

        auto* profile = new Profile("Mock", 1.0f, 15.0f, 50.0f);
        auto* delivery = new MockDelivery();
        auto* logger = new MockLogger();
        auto* pump = new MockPumpSystem(profile, delivery, logger);

        ControlIQ control(cgm, pump);
        control.adjustBasalRate();

        QVERIFY(!delivery->stopCalled);
        QCOMPARE(delivery->lastBasal, -1.0f);
        QCOMPARE(delivery->lastBolus, -1.0f);
    }

    // Should stop basal if predicted glucose is low (hypoglycemia)
    void testSuspendOnHypo() {
        auto* cgm = new MockCGM();
        cgm->glucose = 70.0f;
        cgm->trend = -1.0f;

        auto* profile = new Profile("Mock", 1.0f, 15.0f, 50.0f);
        auto* delivery = new MockDelivery();
        auto* logger = new MockLogger();
        auto* pump = new MockPumpSystem(profile, delivery, logger);

        ControlIQ control(cgm, pump);
        control.adjustBasalRate();

        QVERIFY(delivery->stopCalled);
    }

    // Should increase basal rate if glucose is high and rising
    void testIncreasedBasal() {
        auto* cgm = new MockCGM();
        cgm->glucose = 170.0f;
        cgm->trend = 1.0f;

        auto* profile = new Profile("Mock", 1.0f, 15.0f, 50.0f);
        auto* delivery = new MockDelivery();
        auto* logger = new MockLogger();
        auto* pump = new MockPumpSystem(profile, delivery, logger);

        ControlIQ control(cgm, pump);
        control.adjustBasalRate();

        QVERIFY(delivery->lastBasal > 1.0f);
    }

    // Should decrease basal rate if glucose is low and falling
    void testDecreasedBasal() {
        auto* cgm = new MockCGM();
        cgm->glucose = 100.0f;
        cgm->trend = -1.0f;

        auto* profile = new Profile("Mock", 1.0f, 15.0f, 50.0f);
        auto* delivery = new MockDelivery();
        auto* logger = new MockLogger();
        auto* pump = new MockPumpSystem(profile, delivery, logger);

        ControlIQ control(cgm, pump);
        control.adjustBasalRate();

        QVERIFY(delivery->lastBasal < 1.0f);
    }

    // Should deliver auto bolus if predicted glucose exceeds hyperglycemia threshold
    void testCorrectionOnHyper() {
        auto* cgm = new MockCGM();
        cgm->glucose = 200.0f;
        cgm->trend = 1.0f;

        auto* profile = new Profile("Mock", 1.0f, 15.0f, 50.0f);
        auto* delivery = new MockDelivery();
        auto* logger = new MockLogger();
        auto* pump = new MockPumpSystem(profile, delivery, logger);

        ControlIQ control(cgm, pump);
        control.adjustBasalRate();

        QVERIFY(delivery->lastBolus > 0.0f);
    }

    // Should cap auto bolus at 6.0 units even if correction factor suggests more
    void testMaxBolusLimit() {
        auto* cgm = new MockCGM();
        cgm->glucose = 300.0f;
        cgm->trend = 0.0f;

        auto* profile = new Profile("Mock", 1.0f, 15.0f, 10.0f);  // low correction factor = high dose
        auto* delivery = new MockDelivery();
        auto* logger = new MockLogger();
        auto* pump = new MockPumpSystem(profile, delivery, logger);

        ControlIQ control(cgm, pump);
        control.adjustBasalRate();

        QCOMPARE(delivery->lastBolus, 6.0f);
    }
};

QTEST_MAIN(TestControlIQ)
#include "test_controliq.moc"

