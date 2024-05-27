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
    Swap xSwap;
public:
    RiskEngine();
    RiskEngine(NettingSet& netSet, RateCurve& curve);
    RiskEngine(Swap& swap, RateCurve& curve);
    std::map<double,double> calcIRDelta();
    double calcRWADelta();
};

#endif