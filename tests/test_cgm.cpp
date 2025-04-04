#include <QtTest>
#include "../src/CGM.h"  // future: adjust if path changes
#include <QSignalSpy>

class TestCGM : public QObject {
    Q_OBJECT

private slots:
    void testInitialValues() {
        CGM cgm;
        QCOMPARE(cgm.readGlucose(), 0.0f);
        QCOMPARE(cgm.getTrend(), 0.0f);
    }

    void testUpdateAndRead() {
        CGM cgm;
        cgm.updateGlucose(150.0f);
        QCOMPARE(cgm.readGlucose(), 150.0f);
    }
    void testTrendPositive() {
    CGM cgm;
    cgm.updateGlucose(100.0f);
    QTest::qSleep(1100);
    cgm.updateGlucose(110.0f);
    float trend = cgm.getTrend();
    qDebug() << "Positive Trend:" << trend;
    QVERIFY(trend > 0.01f);
}

void testTrendNegative() {
    CGM cgm;
    cgm.updateGlucose(140.0f);
    QTest::qSleep(1100);
    cgm.updateGlucose(130.0f);
    float trend = cgm.getTrend();
    qDebug() << "Negative Trend:" << trend;
    QVERIFY(trend < -0.01f);
}

    void testTrendStable() {
        CGM cgm;
        cgm.updateGlucose(120.0f);
        QTest::qSleep(1000);
        cgm.updateGlucose(120.0f);
        QCOMPARE(cgm.getTrend(), 0.0f);
    }

    void testSignalEmitted() {
        CGM cgm;
        QSignalSpy spy(&cgm, &CGM::glucoseUpdated);

        cgm.updateGlucose(150.0f);
        QCOMPARE(spy.count(), 1);

        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toFloat(), 150.0f);
    }

    void testRapidUpdates() {
        CGM cgm;
        for (int i = 0; i < 10; ++i) {
            cgm.updateGlucose(100.0f + i);
            QTest::qSleep(100); // simulate fast updates
        }
        QVERIFY(cgm.readGlucose() >= 109.0f);
    }
};

QTEST_MAIN(TestCGM)
#include "test_cgm.moc"

