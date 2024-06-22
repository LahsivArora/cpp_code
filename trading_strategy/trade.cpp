#include "trade.h"

int CreateTrade::id = 125; // trade id starts at 125; ideally max(tradeId) info should be fetched from a database

CreateTrade::CreateTrade(tick tick, double notional, buySell dir, trigger trig){
    xTick=tick;
    xNotional=notional;
    xDir=dir;
    xTrig=trig;
}

trade CreateTrade::get(){

    trade newTrade;
    newTrade.status = status::ALIVE;
    newTrade.notional = xNotional;
    newTrade.buySell = xDir;
    newTrade.tradeId = ++id ;
    newTrade.price = (xDir==buySell::BUY?xTick.buyPrice:xTick.sellPrice);

    if (xTrig == trigger::NEW){
        // need to add stopLoss, takeProfit, killTime
    }
    else {
        // add parentID details
    }
    return newTrade;
}