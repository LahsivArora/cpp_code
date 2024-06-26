#ifndef EXPOSURE_H_
#define EXPOSURE_H_

#include <vector>
#include "MarketData.h"
#include "NettingSet.h"
#include "Enums.h"

/* Calculate exposure given:
   1. Time steps (Quarterly first and then monthly)
   2. Trade instrument (Vanilla swap first and then currency swap)
   3. Simulated market data (rate curve first and then rate curve+FX)
   4. Probability of Default (PD) (constant marginal PD first and then calc from CDS curve)
   5. Loss Given Default (LGD) (constant)
   6. CSA details (no-CSA first, then CSA with $500k threshold)
   7. Netting set (first 1 trade, then multiple vanilla swaps, then mix of vanilla and xccy swaps)
*/

class ExposureCalc{
private:
    NettingSet * xNetSet;
    std::vector<RateCurve *> * xSimCurves;
    std::vector<double *>* xSimSpots;
    MarketData * xMktData;
public:
    static int counter;
    std::vector<std::map<double,double>> calc();
    ExposureCalc(NettingSet* netSet = new NettingSet, std::vector<RateCurve *>* simCurves = new std::vector<RateCurve *>,
                 std::vector<double *>* simSpots = new std::vector<double *>, MarketData* mktData = new MarketData);
    std::map<double,double> calcEEProfile(RiskType type); // for CVA/DVA calc
    double calcEAD(); // for RWA calc
    NettingSet* getNettingSet();
    RateCurve* getBaseCurve();
    ~ExposureCalc(){--counter;}
};
  
#endif