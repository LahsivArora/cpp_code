#ifndef RISKCALC_H_
#define RISKCALC_H_

#include "Exposure.h"
#include "CDSCurve.h"

class RiskCalc{
private:
    std::map<double,double> xExposure;
    CDSCurve xCurve;
    double xLGD;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    RiskCalc(std::map<double,double> exposure, CDSCurve curve, double LGD);
    double CalcXVA();
    double CalcRWA();
};

#endif