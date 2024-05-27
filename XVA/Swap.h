#ifndef SWAP_H_
#define SWAP_H_

#include <vector>
#include "Enums.h"
#include "Leg.h"

class VanillaSwap{
private:
    double xmaturity, xnotional, xEndFxFwd;
    Leg xLeg1, xLeg2;
    TradeType xType;
    NotionalExch xExch;
public:
    VanillaSwap();     // default constructor
    // at trade level, input maturity and notional (+ive is receive leg1 and -ive is pay leg2)
    // endFxFwd is the Fx forward for the Xccy swap notional exchange at maturity; default at 1.0 for VanillaSwap
    VanillaSwap(TradeType type, double maturity, double notional, Leg& Leg1, Leg& Leg2, NotionalExch exch, double endFxFwd = 1.0);
    Leg getLeg(int legNum);
    TradeType getTradeType();
    double getMaturity();
    double getNotional();
    double getAdjNotional(); // for RWA calc
    double getRiskHorizon(); // for RWA calc
    double getEndFxFwd();
};

#endif