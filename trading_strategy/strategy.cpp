#include "strategy.h"
#include "trade.h"
#include "calc.h"
#include <cmath>

char timeString[26];
double notional = 1000000.0; // 1mln USD

std::map<std::string,double[2]> gapDown(ccyPairDef ccyPair, std::queue<tick> xData, std::vector<trade> *xTrades, 
                                     std::ofstream &outputFile, std::map<std::string,double[2]> PnL){
    // ccy Pair settings
    double coeff = ccyPair.coeff;
    double jumpCheck = 1.0 - sqrt(coeff);
    double tolerance= (ccyPair.maxTolerance * ccyPair.pipSize); 
    double minGap= ccyPair.minGap;

    // strategy 1
    int count = 0;
    double h0 = 0.0, h1 = 0.0, h_cur = 0.0; // h0 is Gap move depth; h1 is Fade move depth
    double t0 = 0.0, t1 = 0.0, t_cur = 0.0; // t0 is Gap move time ; t1 is Fade move time
    bool no_trade_alive = true; // once NEW trade is alive stop checking for trend till trade is closed out
    double prev2Price = 0.0, prevPrice = 0.0, currentAvg = 0.0;
    dataType tickType = xData.front().datatype;
    if (tickType == dataType::SEC){tolerance *= 0.75;} // use smaller tolerance for 1s data 

    while (!xData.empty()){

        double currentPrice = xData.front().buyPrice;
        time_t currentTime = xData.front().timestamp; 
        double currentTime_ms = xData.front().millisec;
        xData.pop();
        double newPrice = xData.front().buyPrice;
        time_t newTime = xData.front().timestamp; 
        double newTime_ms = xData.front().millisec;
        double newAvg = (prev2Price + prevPrice + currentPrice + newPrice)/4.0;

        if (tickType == dataType::SEC){
            newAvg = newPrice; currentAvg = currentPrice; // dont use smoothing for 1s data; for tick data smooth across 4 ticks
        } 

        // reset and skip for weekend break
        if (difftime(newTime,currentTime) > 150000){
            t0 = t1 = h0 = h1 = 0.0;
            continue;}

        if ((newAvg <= currentAvg + tolerance) && no_trade_alive){
            if (h1 != 0.0){
                h1 = t1 = 0.0;
                h0 = (currentPrice - newPrice);
                t0 = difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
                }
            else {
                h0 += (currentPrice - newPrice);
                t0 += difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
            }
        }        
        else {
            h_cur = h0; t_cur = t0;
            h1 += (newPrice - currentPrice);
            t1 += difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
            if (!no_trade_alive){
                trade *currentTrade = &(xTrades->back());
                
                // update trigger levels based on trend till trigger is hit
                double changePrice = std::abs(newAvg - currentAvg) * 0.20; 
                double changeTime = currentTrade->killTime * changePrice/std::abs(currentTrade->takeProfit - currentTrade->stopLoss);
                if (newAvg <= currentAvg - tolerance/2.0){ 
                    currentTrade->stopLoss += changePrice;
                    currentTrade->killTime -= changeTime;
                    outputFile << "stopLoss change:" << changePrice << ";killTime change:" << -1.0*changeTime << std::endl;}
                else if (newAvg >= currentAvg + tolerance/2.0){
                    currentTrade->takeProfit += changePrice;
                    currentTrade->killTime += changeTime;
                    outputFile << "takeProfit change:" << changePrice << ";killTime change:" << changeTime << std::endl;}

                // check trigger levels and process close out events
                if (xData.front().sellPrice >= currentTrade->takeProfit){
                    PnL["GapDn_TakeProfit"][0] += processCloseOut("GapDown",currentTrade,xData.front(),buySell::SELL,trigger::TAKEPROFIT, outputFile);
                    PnL["GapDn_TakeProfit"][1] += 1;
                    no_trade_alive = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (xData.front().sellPrice <= currentTrade->stopLoss){
                    PnL["GapDn_StopLoss"][0] += processCloseOut("GapDown",currentTrade,xData.front(),buySell::SELL,trigger::STOPLOSS, outputFile);
                    PnL["GapDn_StopLoss"][1] += 1;
                    no_trade_alive = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                    // Continue following trend after stopLoss trigger
                    h0 = (currentTrade->price - currentTrade->origStopLoss)/(coeff*coeff);
                    t0 = currentTrade->origKillTime/coeff;
                }
                else if (difftime(xData.front().timestamp,currentTrade->creationTime)+(xData.front().millisec - currentTrade->creationMs) >= currentTrade->killTime){
                    PnL["GapDn_Kill"][0] += processCloseOut("GapDown",currentTrade,xData.front(),buySell::SELL,trigger::KILL, outputFile);
                    PnL["GapDn_Kill"][1] += 1;
                    no_trade_alive = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
            }
            else if (h1 >= (h_cur*jumpCheck*coeff*coeff) && no_trade_alive && (h_cur/ccyPair.pipSize) >= minGap){
                double t_price = xData.front().buyPrice;
                double t_loss = t_price - h1;
                double t_profit = t_loss + h_cur*coeff*coeff;
                if (h1/(h_cur*coeff*coeff) >= 0.9){
                    // do nothing since not much expected profit left; ratio>1 means takeProfit level is below trade price
                    outputFile << "INFO: skipping trade as not much expected profit is left" << ";h_cur:" << h_cur << ";h1:" << h1 << std::endl;
                    h0 = t0 = 0.0;
                }
                else {
                    double t_kill_low = (coeff*t_cur<2.0?2.0:coeff*t_cur);
                    double t_kill = (t_kill_low>900.0?900:t_kill_low);
                    CreateTrade Trade(xData.front(),notional,buySell::BUY,trigger::NEW,t_profit,t_loss, t_kill,t_loss, t_kill);
                    trade newTrade = Trade.get();
                    xTrades->push_back(newTrade);
                    std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&xData.front().timestamp));
                    outputFile << "INFO: GapDown;new trade; tradeCount:" << count++ << ";tradeId:" << newTrade.tradeId << ";timeStamp:" << timeString <<"." <<xData.front().millisec <<";t_cur:" << t_cur;
                    outputFile << ";h_cur:" << h_cur << ";h1:" << h1 <<";price:" << newTrade.price << ";takeProfit:" << t_profit << ";stopLoss:" << t_loss << ";killTime:" << t_kill << std::endl;
                    no_trade_alive = false;
                    h0 = t0 = 0.0;
                    }
                }
        }
        prev2Price = prevPrice;
        prevPrice = currentPrice;
        currentAvg = newAvg;
    }
    return PnL;
}


std::map<std::string,double[2]> gapUp(ccyPairDef ccyPair, std::queue<tick> xData, std::vector<trade> *xTrades, 
                                   std::ofstream &outputFile, std::map<std::string,double[2]> PnL){
    
    // ccy Pair settings
    double coeff = ccyPair.coeff;
    double jumpCheck = 1.0 - sqrt(coeff);
    double tolerance=ccyPair.maxTolerance * ccyPair.pipSize; 
    double minGap=ccyPair.minGap;

    // strategy 2
    int count = 0;
    double h0 = 0.0, h1 = 0.0, h_cur = 0.0; // h0 is Gap move depth; h1 is Fade move depth
    double t0 = 0.0, t1 = 0.0, t_cur = 0.0; // t0 is Gap move time ; t1 is Fade move time
    bool no_trade_alive = true; // once NEW trade is alive stop checking for trend till trade is closed out 
    double prev2Price = 0.0, prevPrice = 0.0, currentAvg = 0.0;
    dataType tickType = xData.front().datatype;
    if (tickType == dataType::SEC){tolerance *= 0.75;} // use smaller tolerance for 1s data 

    while (!xData.empty()){

        double currentPrice = xData.front().sellPrice;
        time_t currentTime = xData.front().timestamp; 
        double currentTime_ms = xData.front().millisec;
        xData.pop();
        double newPrice = xData.front().sellPrice;
        time_t newTime = xData.front().timestamp; 
        double newTime_ms = xData.front().millisec;
        double newAvg = (prev2Price + prevPrice + currentPrice + newPrice)/4.0;

        if (tickType == dataType::SEC){
            newAvg = newPrice; currentAvg = currentPrice; // dont use smoothing for 1s data; for tick data smooth across 4 ticks
        }
        // reset and skip for weekend break
        if (difftime(newTime,currentTime) > 150000){
            t0 = t1 = h0 = h1 = 0.0;
            continue;}

        if ((newAvg >= currentAvg - tolerance) && no_trade_alive){
            if (h1 != 0.0){
                h1 = t1 = 0.0;
                h0 = (newPrice - currentPrice);
                t0 = difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
                }
            else {
                h0 += (newPrice - currentPrice);
                t0 += difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
            }
        }        
        else {
            h_cur = h0; t_cur = t0;
            h1 += (currentPrice - newPrice);
            t1 += difftime(newTime,currentTime);
            if (!no_trade_alive){
                trade *currentTrade = &(xTrades->back());

                // update trigger levels based on trend till trigger is hit
                double changePrice = std::abs(newAvg - currentAvg) * 0.20; 
                double changeTime = currentTrade->killTime * changePrice/std::abs(currentTrade->takeProfit - currentTrade->stopLoss);
                 if (newAvg >= currentAvg + tolerance/2.0){ 
                    currentTrade->stopLoss -= changePrice;
                	currentTrade->killTime -= changeTime;
                    outputFile << "stopLoss change:" << changePrice << ";killTime change:" << -1.0*changeTime << std::endl;}
                else if (newAvg <= currentAvg - tolerance/2.0) {
                    currentTrade->takeProfit -= changePrice;
                    currentTrade->killTime += changeTime;
                    outputFile << "takeProfit change:" << changePrice << ";killTime change:" << changeTime << std::endl;}
                
                // check trigger levels and process close out events
                if (xData.front().buyPrice <= currentTrade->takeProfit){
                    PnL["GapUp_TakeProfit"][0] += processCloseOut("GapUp",currentTrade,xData.front(),buySell::BUY,trigger::TAKEPROFIT, outputFile);
                    PnL["GapUp_TakeProfit"][1] += 1;
                    no_trade_alive = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (xData.front().buyPrice >= currentTrade->stopLoss){
                    PnL["GapUp_StopLoss"][0] += processCloseOut("GapUp",currentTrade,xData.front(),buySell::BUY,trigger::STOPLOSS, outputFile);
                    PnL["GapUp_StopLoss"][1] += 1;
                    no_trade_alive = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                    // Continue following trend after stopLoss trigger
                    h0 = (currentTrade->origStopLoss - currentTrade->price)/(coeff*coeff);
                    t0 = currentTrade->origKillTime/coeff;
                }
                else if (difftime(xData.front().timestamp,currentTrade->creationTime)+(xData.front().millisec - currentTrade->creationMs) >= currentTrade->killTime){
                    PnL["GapUp_Kill"][0] += processCloseOut("GapUp",currentTrade,xData.front(),buySell::BUY,trigger::KILL, outputFile);
                    PnL["GapUp_Kill"][1] += 1;
                    no_trade_alive = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
            }
            else if (h1 >= (h_cur*jumpCheck*coeff*coeff) && no_trade_alive && h_cur/ccyPair.pipSize > minGap){
                double t_price = xData.front().sellPrice;
                double t_loss = t_price + h1;
                double t_profit = t_loss - h_cur*coeff*coeff; 
                if (h1/(h_cur*coeff*coeff) >= 0.9){
                    // do nothing since not much expected profit left; ratio>1 means takeProfit level is below trade price
                    outputFile << "INFO: skipping trade as not much expected profit is left" << ";h_cur:" << h_cur << ";h1:" << h1 << std::endl;
                    h0 = t0 = 0.0;
                } else {
                    double t_kill_low = (coeff*t_cur<2.0?2.0:coeff*t_cur);
                    double t_kill = (t_kill_low>900.0?900:t_kill_low);
                    CreateTrade Trade(xData.front(),notional,buySell::SELL,trigger::NEW,t_profit,t_loss, t_kill,t_loss, t_kill);
                    trade newTrade = Trade.get();
                    xTrades->push_back(newTrade);
                    std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&xData.front().timestamp));
                    outputFile << "INFO: GapUp;new trade; tradeCount:" << count++ << ";tradeId:" << newTrade.tradeId << ";timeStamp:" << timeString <<"." <<xData.front().millisec<<";t_cur:" << t_cur;
                    outputFile << ";h_cur:" << h_cur << ";h1:" << h1 <<";price:" << newTrade.price << ";takeProfit:" << t_profit << ";stopLoss:" << t_price << ";killTime:" << t_kill << std::endl;
                    no_trade_alive = false;
                    h0 = t0 = 0.0;
                    }
                }
        }
        prev2Price = prevPrice;
        prevPrice = currentPrice;
        currentAvg = newAvg;
    }
    return PnL;
}


double processCloseOut(std::string strat, trade *curr, tick tick, buySell dir, trigger trig, std::ofstream &outputFile){

    CreateTrade Trade(tick,notional,dir,trig);
    trade closeoutTrade = Trade.get();
    closeoutTrade.parentId = curr->tradeId;
    std::vector<std::string> trigger = {"NEW", "KILL", "STOPLOSS", "TAKEPROFIT"};
    std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&tick.timestamp));
    outputFile << "INFO: " << strat << ";" << trigger[unsigned(trig)] << ";tradeId:" << closeoutTrade.tradeId << ";parenttradeId:" << closeoutTrade.parentId  << ";timeStamp:" << timeString <<"." <<tick.millisec;
    ProfitLoss realize(curr, &closeoutTrade);
    outputFile << ";price:" << closeoutTrade.price << ";killTime:" << curr->killTime << ";profit loss for trades:" << realize.calc() << std::endl;
    return realize.calc();
}