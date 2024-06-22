#ifndef DATAMODEL_H_
#define DATAMODEL_H_

enum class buySell {BUY, SELL};
enum class status {DEAD, ALIVE};
enum class trigger {NEW, KILL, STOPLOSS, TAKEPROFIT};

struct DateTime{
    int date; // in YYYYMMDD format
    double time; // in 24hr format/hhmmss.000
};

struct tick{
    DateTime timestamp;
    double price;
    double sellPrice = price - 0.20/100.0; // example for USDJPY; pipsize = 0.01; assuming spread = 0.4pip
    double buyPrice = price + 0.20/100.0;
};

struct trade{
    int tradeId;
    status status;      // using enum; trades are marked dead after P&L realization calc
    double notional;    // in USD
    buySell buySell;    // using enum
    double price;       // traded price
    double takeProfit;  // price to closeout trade - at profit
    double stopLoss;    // price to closeout trade - at loss
    int parentId = 0;   // i.e. if this trade is created after stopLoss/takeProfit is triggered for existing trade
    DateTime killTime = {0,0.0};  // max time to live for trade; close out excuted even if other triggers are not reached
};

#endif