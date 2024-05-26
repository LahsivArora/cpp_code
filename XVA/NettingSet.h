#ifndef NETTINGSET_H_
#define NETTINGSET_H_

#include <vector>
#include "Swap.h"
#include "RateCurve.h"

class NettingSet{
private:
    std::vector<VanillaSwap> xSwaps;
public:
    NettingSet();  // default constructor
    NettingSet(std::vector<VanillaSwap>& swaps);
    double getMaxMaturity();
    std::vector<VanillaSwap> getTrades();
    unsigned int getNoOfTrades();
    double getTradesNPV(RateCurve curve);
};

#endif