#ifndef SWAP_H_
#define SWAP_H_

#include <vector>
#include "Enums.h"
#include "Leg.h"

class VanillaSwap{
private:
    double xmaturity, xnotional;
    Leg xLeg1, xLeg2;
    TradeType xType;
public:
    VanillaSwap();     // default constructor
    // assuming Leg1 is fixed rate (input cashflow freq/year + fixed rate)
    // assuming Leg2 is floating rate (input cashflow freq/year + spread)
    // at trade level, input maturity and notional (+ive is receive fixed/leg1 and -ive is pay fixed/leg1)
    VanillaSwap(TradeType type, double maturity, double notional, Leg& Leg1, Leg& Leg2);
    Leg getLeg(int legNum);
    TradeType getTradeType();
    double getMaturity();
    double getNotional();
    double getAdjNotional(); // for RWA calc
    double getRiskHorizon(); // for RWA calc
};

#endif