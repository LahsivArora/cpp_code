#ifndef RISKENGINE_H_
#define RISKENGINE_H_

#include <vector>
#include "NettingSet.h"
#include "RateCurve.h"
#include "Pricer.h"

class RiskEngine{
private:
    NettingSet xNetSet;
    RateCurve xCurve;
public:
    RiskEngine(NettingSet &netSet, RateCurve &curve);
    std::map<double,double> calcIRDelta();
};

#endif