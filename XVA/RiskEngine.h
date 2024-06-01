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
    RiskEngine(NettingSet& netSet = *(new NettingSet), RateCurve& curve = *(new RateCurve));
    RiskEngine(Swap& swap = *(new Swap), RateCurve& curve = *(new RateCurve));
    std::map<double,double> calcIRDelta();
    double calcRWADelta();
};

#endif