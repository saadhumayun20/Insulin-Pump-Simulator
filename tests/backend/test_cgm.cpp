#include <QtTest>
#include "../src/CGM.h"  // future: adjust if path changes
#include <QSignalSpy>

class TestCGM : public QObject {
    Q_OBJECT

private slots:

    // Tests that a new CGM starts with 0 glucose and 0 trend
    void testInitialValues() {
        CGM cgm;
        QCOMPARE(cgm.readGlucose(), 0.0f);
        QCOMPARE(cgm.getTrend(), 0.0f);
    }

    // Verifies that glucose updates are correctly stored and read
    void testUpdateAndRead() {
        CGM cgm;
        cgm.updateGlucose(150.0f);
        QCOMPARE(cgm.readGlucose(), 150.0f);
    }

    // Ensures that a rising glucose level produces a positive trend
    void testTrendPositive() {
        CGM cgm;
        cgm.updateGlucose(100.0f);
        QTest::qSleep(1100);
        cgm.updateGlucose(110.0f);
        float trend = cgm.getTrend();
        qDebug() << "Positive Trend:" << trend;
        QVERIFY(trend > 0.01f);
    }

    // Ensures that a falling glucose level produces a negative trend
    void testTrendNegative() {
        CGM cgm;
        cgm.updateGlucose(140.0f);
        QTest::qSleep(1100);
        cgm.updateGlucose(130.0f);
        float trend = cgm.getTrend();
        qDebug() << "Negative Trend:" << trend;
        QVERIFY(trend < -0.01f);
    }

    // Ensures that glucose stability results in zero trend
    void testTrendStable() {
        CGM cgm;
        cgm.updateGlucose(120.0f);
        QTest::qSleep(1000);
        cgm.updateGlucose(120.0f);
        QCOMPARE(cgm.getTrend(), 0.0f);
    }

    // Verifies the glucoseUpdated signal is emitted when glucose changes
    void testSignalEmitted() {
        CGM cgm;
        QSignalSpy spy(&cgm, &CGM::glucoseUpdated);

        cgm.updateGlucose(150.0f);
        QCOMPARE(spy.count(), 1);

        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toFloat(), 150.0f);
    }

    // Simulates rapid successive glucose updates to test stability
    void testRapidUpdates() {
        CGM cgm;
        for (int i = 0; i < 10; ++i) {
            cgm.updateGlucose(100.0f + i);
            QTest::qSleep(100); // simulate fast updates
        }
        QVERIFY(cgm.readGlucose() >= 109.0f);
    }

    // Tests trend behavior with a very small glucose delta (precision test)
    void testPrecisionTrend() {
        CGM cgm;
        cgm.updateGlucose(100.0f);
        QTest::qSleep(1100);
        cgm.updateGlucose(100.1f);
        float trend = cgm.getTrend();
        QVERIFY(trend > 0.0f && trend < 10.0f); // Should be small but positive
    }

    // Verifies that repeated identical values produce zero trend
    void testIdenticalUpdates() {
        CGM cgm;
        cgm.updateGlucose(115.0f);
        QTest::qSleep(1000);
        cgm.updateGlucose(115.0f);
        float trend = cgm.getTrend();
        QCOMPARE(trend, 0.0f);
    }

    // Simulates 100 rising readings to ensure stable internal state and positive trend
    void testHighFrequencyUpdates() {
        CGM cgm;
        float value = 100.0f;

        for (int i = 0; i < 100; ++i) {
            cgm.updateGlucose(value + i);  // rising glucose
            QTest::qSleep(50);             // simulate 0.05s delay
        }


	QTest::qSleep(1000);
	cgm.updateGlucose(200.0f);
	QVERIFY(cgm.getTrend() > 0.0f);


    }
};

QTEST_MAIN(TestCGM)
#include "test_cgm.moc"

