#ifndef DATAMODEL_H_
#define DATAMODEL_H_

#include <ctime>
#include <string>

enum class buySell {BUY, SELL};
enum class status {DEAD, ALIVE};
enum class trigger {NEW, KILL, STOPLOSS, TAKEPROFIT};
enum class ccyPairs {USDJPY, EURUSD, GBPUSD, EURJPY};
enum class dataType {SEC, TICK, MIN, HOUR};

struct tick{
    dataType datatype;
    time_t timestamp;
    double millisec;
    double price;
    double sellPrice = price - 0.20/100.0; // example for USDJPY; pipsize = 0.01; assuming spread = 0.4pip
    double buyPrice = price + 0.20/100.0;
};

struct ccyPairDef{
    ccyPairs name;
    double pipSize;
    double spread = 0.4; // default spread of 0.4pip, if not specified
    double coeff = 1.0; // coefficient of restitution for Down trending strategy (with fade up)
    // For more volatile ccyPair higher tolerance and minGap should be used
    double maxTolerance = 0.0; // if tick change is within tolerance then continue trend. 
    double minGap = 10.0; // trade only if trend is >= minGap (in pips)
};

struct trade{
    int tradeId;
    status status;      // using enum; trades are marked dead after P&L realization calc
    double notional;    // in USD
    buySell buySell;    // using enum
    double price;       // traded price
    time_t creationTime;
    double creationMs;  // millisecond component
    double takeProfit;  // price to closeout trade - at profit
    double stopLoss;    // price to closeout trade - at loss
    int parentId = 0;   // i.e. if this trade is created after stopLoss/takeProfit is triggered for existing trade
    double killTime = 0.0;  // max time to live for trade; close out excuted even if other triggers are not reached
    double origStopLoss = stopLoss;
    double origKillTime = killTime;
};

#endif