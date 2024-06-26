#include "trade.h"

int CreateTrade::id = 125; // trade id starts at 125; ideally max(tradeId) info should be fetched from a database

CreateTrade::CreateTrade(tick tick, double notional, buySell dir, trigger trig, double takeProfit, 
                         double stopLoss, double killTime, double origStopLoss, double origKillTime){
    xTick=tick;
    xNotional=notional;
    xDir=dir;
    xTrig=trig;
    xProfitLevel=takeProfit;
    xLossLevel=stopLoss;
    xKillTime=killTime;
    xOrigKillTime=origKillTime;
    xOrigLossLevel=origStopLoss;
}

trade CreateTrade::get(){

    trade newTrade;
    newTrade.status = status::ALIVE;
    newTrade.notional = xNotional;
    newTrade.buySell = xDir;
    newTrade.tradeId = ++id ;
    newTrade.price = (xDir==buySell::BUY?xTick.buyPrice:xTick.sellPrice);
    newTrade.creationTime = xTick.timestamp;
    newTrade.creationMs = xTick.millisec;

    if (xTrig == trigger::NEW){
        newTrade.takeProfit = xProfitLevel;
        newTrade.stopLoss = xLossLevel;
        newTrade.killTime = xKillTime;
        newTrade.origKillTime = xOrigKillTime;
        newTrade.origStopLoss = xLossLevel;
    }
    return newTrade;
}