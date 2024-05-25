#ifndef XVACALC_H_
#define XVACALC_H_

#include "Exposure.h"
#include "CDSCurve.h"

class XVACalc{
private:
    std::map<double,double> xExposure;
    CDSCurve xCurve;
    double xLGD;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    XVACalc(std::map<double,double> exposure, CDSCurve curve, double LGD);
    double calcXVA();
    double calcRWA();
    double calcInitialMargin();
};

#endif