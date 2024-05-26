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
    NettingSet xNetSet;
    RateCurve xBaseCurve;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    XVACalc(ExposureCalc& exposureObj, CDSCurve curve, double LGD, RiskType type);
    double calcXVA();
    double calcRWA();
    double calcInitialMargin();
};

#endif