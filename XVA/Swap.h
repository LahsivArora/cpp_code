#ifndef SWAP_H_
#define SWAP_H_

#include <vector>
#include "Enums.h"

/*
 Step1:
   Create swap object (fixed vs float)

 Step2:
   Make object flexible to define xccy and single ccy swap with same object

*/

class VanillaSwap{
private:
    double xmaturity, xnotional;
    std::vector<double> xLeg1, xLeg2;
    TradeType xType;
public:
    VanillaSwap();     // default constructor
    // assuming Leg1 is fixed rate (input cashflow freq/year + fixed rate)
    // assuming Leg2 is floating rate (input cashflow freq/year + spread)
    // at trade level, input maturity and notional (+ive is receive fixed/leg1 and -ive is pay fixed/leg1)
    VanillaSwap(TradeType type, double maturity, double notional, std::vector<double> Leg1, std::vector<double> Leg2);
    std::vector<double> getLegFlows(int Leg);
    TradeType getTradeType();
    double getMaturity();
    double getNotional();
    double getAdjNotional(); // for RWA calc
    double getFixedRate();
    double getFloatSpread();
    double getFixedFreq();
    double getFloatFreq();
};

#endif