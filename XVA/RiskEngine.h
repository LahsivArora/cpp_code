#ifndef RISKENGINE_H_
#define RISKENGINE_H_

#include <vector>
#include "Pricer.h"

class RiskEngine{
private:
    NettingSet * xNetSet;
    std::vector<Swap *>* xSwaps;
    RateCurve * xCurve;
    MarketData * xMktData;
    Swap * xSwap;
    std::string xBumpCurve;
public:
    static int counter;
    RiskEngine(NettingSet* netSet = new NettingSet, MarketData* curve = new MarketData, std::string bumpCurve = "");
    RiskEngine(Swap* swap = new Swap, MarketData* curve = new MarketData, std::string bumpCurve = "");
    std::map<double,double> calcIRDelta();
    double calcRWADelta();
    ~RiskEngine(){--counter;}
};

#endif