#ifndef RISKENGINE_H_
#define RISKENGINE_H_

#include <vector>
#include "NettingSet.h"
#include "Swap.h"
#include "RateCurve.h"
#include "Pricer.h"

class RiskEngine{
private:
    NettingSet xNetSet;
    RateCurve xCurve;
    VanillaSwap xSwap;
public:
    RiskEngine(NettingSet &netSet, RateCurve &curve);
    RiskEngine(VanillaSwap &swap, RateCurve &curve);
    std::map<double,double> calcIRDelta();
    double calcRWADelta();
};

#endif