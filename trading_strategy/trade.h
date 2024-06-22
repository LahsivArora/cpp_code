#ifndef TRADE_H_
#define TRADE_H_

#include "datamodel.h"

class CreateTrade{
private:
    double xNotional;
    buySell xDir;
    tick xTick;
    trigger xTrig;
    static int id;
public:
    CreateTrade(tick tick, double notional, buySell dir, trigger trig);
    trade get();
};


#endif