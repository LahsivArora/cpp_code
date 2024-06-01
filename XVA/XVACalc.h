#ifndef XVACALC_H_
#define XVACALC_H_

#include "Exposure.h"
#include "CDSCurve.h"
#include "Enums.h"

class XVACalc{
private:
    ExposureCalc xExposureObj;
    CDSCurve xCurve;
    double xLGD;
    RiskType xType;
    NettingSet* xNetSet;
    RateCurve* xBaseCurve;
public:
    XVACalc(ExposureCalc& exposureObj, CDSCurve curve, double LGD, RiskType type);
    double calcXVA();
    double calcRWA();
    double calcInitialMargin();
};

#endif