#include "Profile.h"

Profile::Profile(const QString& name, float basal, float carbRatio,
                 float correctionFactor, QObject* parent)
            : QObject(parent), name(name), basalRate(basal),
              carbRatio(carbRatio), correctionFactor(correctionFactor) {}

void Profile::editProfile(float newBasalRate, float newCarbRatio, float newCorrectionFactor){
    basalRate = newBasalRate;
    carbRatio = newCarbRatio;
    correctionFactor = newCorrectionFactor;
}

void Profile::deleteProfile(){
    name = "#####";
    basalRate = 0.0f;
    carbRatio = 0.0f;
    correctionFactor = 0.0f;
    delete parent();
}

//getters
QString Profile::getName() const { return name; }
float Profile::getBasalRate() const { return basalRate; }
float Profile::getCarbRatio() const { return carbRatio; }
float Profile::getCorrectionFactor() const { return correctionFactor; }
float Profile::getTargetGlucose() const { return targetGlucose; }
