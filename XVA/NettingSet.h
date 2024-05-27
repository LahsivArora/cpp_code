#ifndef NETTINGSET_H_
#define NETTINGSET_H_

#include <vector>
#include "Swap.h"
#include "RateCurve.h"

class NettingSet{
private:
    std::vector<Swap> xSwaps;
public:
    NettingSet();  // default constructor
    NettingSet(std::vector<Swap>& swaps);
    double getMaxMaturity();
    std::vector<Swap> getTrades();
    unsigned int getNoOfTrades();
    double getTradesNPV(RateCurve curve1, RateCurve curve2, double FxSpot);
};

#endif