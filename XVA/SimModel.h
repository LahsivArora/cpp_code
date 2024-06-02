#ifndef SIMMODEL_H_
#define SIMMODEL_H_

#include <vector>
#include <string>
#include "MarketData.h"
#include "Swap.h"

/*
 Contain simulation models using monte-carlo
  Step1: sim for RateCurve using Vasicek Model
  Step2: sim for FxAsset using Local Vol model
*/

class SimulateRate{
private:
    RateCurve * xCurve;
    double xVol, xSimPaths, xMeanRev;
public:
    static int counter;
    SimulateRate(RateCurve* curve, double vol, double meanRev, double simPaths);
    std::vector<RateCurve *> getSimulatedCurves();
    std::vector<double> getSimulatedBaseNPVs(Swap* swap, MarketData* mktData);
    ~SimulateRate(){--counter;}
};

#endif