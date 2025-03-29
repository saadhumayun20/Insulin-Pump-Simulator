#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QObject>

class Profile : public QObject
{
    Q_OBJECT

public:
    explicit Profile(const QString& name,       //same as createProfile essentially
                    float basalRate,
                    float carbRatio,
                    float correctionFactor,
                    QObject* parent = nullptr);

    void editProfile(float newBasalRate, float newCarbRatio, float newCorrectionFactor);
    void deleteProfile();

    //getter
    QString getName() const;
    float getBasalRate() const;
    float getCarbRatio() const;
    float getCorrectionFactor() const;
    float getTargetGlucose() const;
private:
    QString name;
    float basalRate;
    float carbRatio;
    float correctionFactor;
};

#endif // PROFILE_H
