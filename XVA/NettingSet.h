#ifndef NETTINGSET_H_
#define NETTINGSET_H_

#include <vector>
#include "Swap.h"
#include "RateCurve.h"

class NettingSet{
private:
    std::vector<Swap> xSwaps;
public:
    NettingSet(std::vector<Swap>& swaps = *(new std::vector<Swap>));
    double getMaxMaturity();
    std::vector<Swap> getTrades();
    unsigned int getNoOfTrades();
};

#endif