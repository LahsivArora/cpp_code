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
    MarketData * xMktData;
    double xVol, xSimPaths, xMeanRev;
public:
    static int counter;
    SimulateRate(MarketData* mktData, double vol, double meanRev, double simPaths);
    std::vector<RateCurve *> getSimulatedCurves();
    ~SimulateRate(){--counter;}
};


class SimulateFxSpot{
private:
    MarketData * xMktData;
    double xVol, xSimPaths;
public:
    static int counter;
    SimulateFxSpot(MarketData* mktData, double vol, double simPaths);
    std::vector<double *> getSimulatedFxSpots();
    ~SimulateFxSpot(){--counter;}
};

#endif