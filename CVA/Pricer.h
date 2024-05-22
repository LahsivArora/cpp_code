#ifndef PRICER_H_
#define PRICER_H_

#include <vector>
#include "Swap.h"

class SwapPricer{
private:
    VanillaSwap xSwap;
    std::vector<double> xLeg1disc, xLeg2disc, xLeg2fwd;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    SwapPricer(VanillaSwap swap, std::vector<double> leg1disc, std::vector<double> leg2disc, std::vector<double> leg2fwd);
    double getLeg1NPV();
    double getLeg2NPV();
    double getTradeNPV();
    double getParRate();
};

#endif