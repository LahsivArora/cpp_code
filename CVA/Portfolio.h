#ifndef PORTFOLIO_H_
#define PORTFOLIO_H_

#include <vector>
#include "Swap.h"
#include "RateCurve.h"

class Portfolio{
private:
    std::vector<VanillaSwap> xSwaps;
public:
    Portfolio();  // default constructor
    Portfolio(std::vector<VanillaSwap> swaps);
    double getMaxMaturity();
    std::vector<VanillaSwap> getTrades();
    double getNoOfTrades();
    double getTradesNPV(RateCurve curve);
};

#endif