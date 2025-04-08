#include <QtTest>
#include "../../src/Profile.h"

class TestProfile : public QObject {
    Q_OBJECT

private slots:
    void testConstructorSetsValues() {
        Profile profile("TestUser", 1.5f, 12.0f, 45.0f);
        QCOMPARE(profile.getName(), QString("TestUser"));
        QCOMPARE(profile.getBasalRate(), 1.5f);
        QCOMPARE(profile.getCarbRatio(), 12.0f);
        QCOMPARE(profile.getCorrectionFactor(), 45.0f);
        QCOMPARE(profile.getTargetGlucose(), 100.0f);
    }
};

QTEST_MAIN(TestProfile)
#include "test_profile.moc"

