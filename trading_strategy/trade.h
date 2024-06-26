#ifndef TRADE_H_
#define TRADE_H_

#include "datamodel.h"

class CreateTrade{
private:
    double xNotional, xProfitLevel, xLossLevel, xKillTime, xOrigLossLevel, xOrigKillTime;
    buySell xDir;
    tick xTick;
    trigger xTrig;
    static int id;
public:
    CreateTrade(tick tick, double notional, buySell dir, trigger trig, double takeProfit = 0.0, 
                double stopLoss = 0.0, double killTime = 0.0, double origStopLoss = 0.0, double origKillTime = 0.0);
    trade get();
};


#endif