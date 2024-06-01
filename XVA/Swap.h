#ifndef SWAP_H_
#define SWAP_H_

#include <vector>
#include "Enums.h"
#include "Leg.h"

class Swap{
private:
    double xmaturity, xnotional, xEndFxFwd;
    Leg xLeg1, xLeg2;
    TradeType xType;
    NotionalExch xExch;
public:
    // at trade level, input maturity and notional (+ive is receive leg1 and -ive is pay leg2)
    // endFxFwd is the Fx forward for the Xccy swap notional exchange at maturity; default at 1.0 for VanillaSwap
    static int counter;
    Swap(TradeType type = TradeType::IrSwap, double maturity = 0.0, double notional = 0.0, Leg& Leg1 = *(new Leg()), 
         Leg& Leg2 = *(new Leg()), NotionalExch exch = NotionalExch::NO, double endFxFwd = 1.0);
    Leg getLeg(int legNum);
    TradeType getTradeType();
    double getMaturity();
    double getNotional();
    NotionalExch getNotionalExch();
    double getAdjNotional(); // for RWA calc
    double getRiskHorizon(); // for RWA calc
    double getEndFxFwd();
};

#endif