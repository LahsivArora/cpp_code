#include "strategy.h"
#include "trade.h"
#include "calc.h"
#include <cmath>

char timeString[26];
double coeff_Dn = 0.80;
double coeff_Up = 0.6667;
double maxGap = 1000.0;
double jumpCheck_Dn = 1.0 - sqrt(coeff_Dn);
double jumpCheck_Up = 1.0 - sqrt(coeff_Up);
double notional = 1000000.0; // 1mln USD

std::map<std::string,double[2]> gapDown(ccyPairDef ccyPair, std::queue<tick> xData, std::vector<trade> *xTrades, 
                                     std::ofstream &outputFile, std::map<std::string,double[2]> PnL){
    
    double tolerance = 0.0, minGap = 1000.0;
    int count = 0;
    double h0 = 0.0, h1 = 0.0, h_cur = 0.0;
    double t0 = 0.0, t1 = 0.0, t_cur = 0.0; 
    bool no_trend_live = true;
    double prevPrice = 0.0, currentAvg = 0.0;
    dataType tickType = xData.front().datatype;
    if (tickType == dataType::SEC){
        tolerance = 0.003; minGap = 10.0;} 
    else if (tickType == dataType::TICK){
        tolerance = 0.004; minGap = 7.5;} 

    while (!xData.empty()){

        double currentPrice = xData.front().buyPrice;
        time_t currentTime = xData.front().timestamp; 
        double currentTime_ms = xData.front().millisec;
        xData.pop();
        double newPrice = xData.front().buyPrice;
        time_t newTime = xData.front().timestamp; 
        double newTime_ms = xData.front().millisec;
        double newAvg = (prevPrice + currentPrice + newPrice)/3.0;

        if (tickType == dataType::SEC){
            newAvg = newPrice; currentAvg = currentPrice; // dont use smoothing for 1s data; for tick data we smooth across 3 ticks
        } 

        // reset and skip for weekend break
        if (difftime(newTime,currentTime) > 150000){
            t0 = t1 = h0 = h1 = 0.0;
            continue;}

        //if ((newPrice <= currentPrice + tolerance) && no_trend_live){
        if ((newAvg <= currentAvg + tolerance) && no_trend_live){
            if (h1 != 0.0){
                h1 = t1 = 0.0;
                h0 = (currentPrice - newPrice);
                t0 = difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
                }
            else {
                h0 += (currentPrice - newPrice);
                t0 += difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
            }
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h0:" << h0 << std::endl;
        }        
        else {
            h_cur = h0; t_cur = t0;
            h1 += (newPrice - currentPrice);
            t1 += difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h1:" << h1 << ":" << h_cur << ":" << h_cur*0.15*coeff*coeff << std::endl;
            if (!no_trend_live){
                trade *currentTrade = &(xTrades->back());
                double changePrice = (newPrice - currentPrice) * 0.14; 
                double changeTime = currentTrade->killTime*(newPrice - currentPrice) * 0.14/(currentTrade->takeProfit - currentTrade->stopLoss);
                
                // update trigger levels based on trend till trigger is hit
                if (newPrice < currentPrice){ 
                    currentTrade->stopLoss -= changePrice;
                    //currentTrade->takeProfit += changePrice;
                    //currentTrade->killTime -= changeTime;
                    outputFile << "stopLoss change: no changes" << std::endl;}
                if (newPrice > currentPrice){ 
                    //currentTrade->stopLoss += changePrice;
                    currentTrade->takeProfit += changePrice;
                    currentTrade->killTime += changeTime;
                    outputFile << "takeProfit change:" << changePrice << ";killTime change:" << changeTime << std::endl;}

                // check trigger levels and process close out events
                if (xData.front().sellPrice >= currentTrade->takeProfit){
                    PnL["GapDn_TakeProfit"][0] += processCloseOut("GapDown",currentTrade,xData.front(),buySell::SELL,trigger::TAKEPROFIT, outputFile);
                    PnL["GapDn_TakeProfit"][1] += 1;
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (xData.front().sellPrice <= currentTrade->stopLoss){
                    PnL["GapDn_StopLoss"][0] += processCloseOut("GapDown",currentTrade,xData.front(),buySell::SELL,trigger::STOPLOSS, outputFile);
                    PnL["GapDn_StopLoss"][1] += 1;
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                    // testing: continue trend after stopLoss
                    h0 = (currentTrade->price - currentTrade->origStopLoss)/(jumpCheck_Up*coeff_Up*coeff_Up);
                    t0 = currentTrade->origKillTime/coeff_Up;
                }
                else if (difftime(xData.front().timestamp,currentTrade->creationTime)+(xData.front().millisec - currentTrade->creationMs) >= currentTrade->killTime){
                    PnL["GapDn_Kill"][0] += processCloseOut("GapDown",currentTrade,xData.front(),buySell::SELL,trigger::KILL, outputFile);
                    PnL["GapDn_Kill"][1] += 1;
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
            }
            else if (h1 >= (h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn) && no_trend_live && h_cur*100.0 >= minGap && h_cur*100.0 <= maxGap){
                double t_price = xData.front().buyPrice;
                double t_loss = t_price - h1;
                //double t_loss = t_price - h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn;
                //double t_loss = t_price - (h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn>0.02?0.02:h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn);
                double t_profit = t_loss + h_cur*coeff_Dn*coeff_Dn;
                //double t_profit = t_price + h_cur*(1.0-jumpCheck_Dn)*coeff_Dn*coeff_Dn;
                if (t_profit <= t_price || h1/(h_cur*coeff_Dn*coeff_Dn) >= 0.75){
                    h0 = t0 = 0.0;
                    // skip since not much expected profit left in trade
                    continue;
                }
                double t_kill = (coeff_Dn*t_cur<2.0?2.0:coeff_Dn*t_cur);
                CreateTrade Trade(xData.front(),notional,buySell::BUY,trigger::NEW,t_profit,t_loss, t_kill,t_loss, t_kill);
                trade newTrade = Trade.get();
                xTrades->push_back(newTrade);
                std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&xData.front().timestamp));
                outputFile << "INFO: GapDown;new trade; tradeCount:" << count++ << ";tradeId:" << newTrade.tradeId << ";timeStamp:" << timeString <<";t_cur:" << t_cur;
                outputFile << ";h_cur:" << h_cur << ";h1:" << h1 <<";price:" << xData.front().buyPrice << ":" << xData.front().sellPrice << ";takeProfit:" << t_profit << ";stopLoss:" << t_loss << ";killTime:" << t_kill << std::endl;
                no_trend_live = false;
                h0 = t0 = 0.0;
                }
        }
        prevPrice = currentPrice;
        currentAvg = newAvg;
    }
    return PnL;
}


std::map<std::string,double[2]> gapUp(ccyPairDef ccyPair, std::queue<tick> xData, std::vector<trade> *xTrades, 
                                   std::ofstream &outputFile, std::map<std::string,double[2]> PnL){
    
    double tolerance = 0, minGap = 1000.0;
    int count = 0;
    double h0 = 0.0, h1 = 0.0, h_cur = 0.0;
    double t0 = 0.0, t1 = 0.0, t_cur = 0.0; 
    bool no_trend_live = true;
    double prevPrice = 0.0, currentAvg = 0.0;
    dataType tickType = xData.front().datatype;
    if (tickType == dataType::SEC){
        tolerance = 0.003; minGap = 10.0;} 
    else if (tickType == dataType::TICK){
        tolerance = 0.004; minGap = 7.5;} 

    while (!xData.empty()){

        double currentPrice = xData.front().sellPrice;
        time_t currentTime = xData.front().timestamp; 
        double currentTime_ms = xData.front().millisec;
        xData.pop();
        double newPrice = xData.front().sellPrice;
        time_t newTime = xData.front().timestamp; 
        double newTime_ms = xData.front().millisec;
        double newAvg = (prevPrice + currentPrice + newPrice)/3.0;

        if (tickType == dataType::SEC){
            newAvg = newPrice; currentAvg = currentPrice; // dont use smoothing for 1s data; for tick data we smooth across 3 ticks
        }
        // reset and skip for weekend break
        if (difftime(newTime,currentTime) > 150000){
            t0 = t1 = h0 = h1 = 0.0;
            continue;}

        //if ((newPrice >= currentPrice - tolerance) && no_trend_live){
        if ((newAvg >= currentAvg - tolerance) && no_trend_live){
            if (h1 != 0.0){
                h1 = t1 = 0.0;
                h0 = (newPrice - currentPrice);
                t0 = difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
                }
            else {
                h0 += (newPrice - currentPrice);
                t0 += difftime(newTime,currentTime) + (newTime_ms - currentTime_ms);
            }
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h0:" << h0 << std::endl;
        }        
        else {
            h_cur = h0; t_cur = t0;
            h1 += (currentPrice - newPrice);
            t1 += difftime(newTime,currentTime);
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h1:" << h1 << ":" << h_cur << ":" << h_cur*0.15*coeff*coeff << std::endl;
            if (!no_trend_live){
                trade *currentTrade = &(xTrades->back());
                double changePrice = (currentPrice - newPrice) * 0.14; 
                double changeTime = currentTrade->killTime*(currentPrice - newPrice) * 0.14/(currentTrade->stopLoss - currentTrade->takeProfit);
                
                // update trigger levels based on trend till trigger is hit
                if (newPrice > currentPrice){ 
                    currentTrade->stopLoss += changePrice;
                    //currentTrade->takeProfit -= changePrice;
                	//currentTrade->killTime -= changeTime;
	                outputFile << "stopLoss change: no changes" << std::endl;}
                if (newPrice < currentPrice){ 
                    //currentTrade->stopLoss -= changePrice;
                    currentTrade->takeProfit -= changePrice;
                    currentTrade->killTime += changeTime;
                    outputFile << "takeProfit change:" << changePrice << ";killTime change:" << changeTime << std::endl;}

                // check trigger levels and process close out events
                if (xData.front().buyPrice <= currentTrade->takeProfit){
                    PnL["GapUp_TakeProfit"][0] += processCloseOut("GapUp",currentTrade,xData.front(),buySell::BUY,trigger::TAKEPROFIT, outputFile);
                    PnL["GapUp_TakeProfit"][1] += 1;
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (xData.front().buyPrice >= currentTrade->stopLoss){
                    PnL["GapUp_StopLoss"][0] += processCloseOut("GapUp",currentTrade,xData.front(),buySell::BUY,trigger::STOPLOSS, outputFile);
                    PnL["GapUp_StopLoss"][1] += 1;
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                    // testing: continue trend after stopLoss
                    h0 = (currentTrade->origStopLoss - currentTrade->price)/(jumpCheck_Up*coeff_Up*coeff_Up);
                    t0 = currentTrade->origKillTime/coeff_Up;
                }
                else if (difftime(xData.front().timestamp,currentTrade->creationTime)+(xData.front().millisec - currentTrade->creationMs) >= currentTrade->killTime){
                    PnL["GapUp_Kill"][0] += processCloseOut("GapUp",currentTrade,xData.front(),buySell::BUY,trigger::KILL, outputFile);
                    PnL["GapUp_Kill"][1] += 1;
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
            }
            else if (h1 >= (h_cur*jumpCheck_Up*coeff_Up*coeff_Up) && no_trend_live && h_cur*100.0 > minGap && h_cur*100.0 <= maxGap){
                double t_price = xData.front().sellPrice;
                double t_loss = t_price + h1;
                //double t_loss = t_price + h_cur*jumpCheck_Up*coeff_Up*coeff_Up;
                //double t_loss = t_price + (h_cur*jumpCheck_Up*coeff_Up*coeff_Up>0.02?0.02:h_cur*jumpCheck_Up*coeff_Up*coeff_Up);
                //double t_profit = t_price - h_cur*(1.0-jumpCheck_Up)*coeff_Up*coeff_Up; // t_price + h1 - h_cur*coeff_Up*coeff_Up 
                double t_profit = t_loss - h_cur*coeff_Up*coeff_Up; // t_price + h1 - h_cur*coeff_Up*coeff_Up 
                if (t_profit >= t_price || h1/(h_cur*coeff_Dn*coeff_Dn) >= 0.75){
                    h0 = t0 = 0.0;
                    // skip since not much expected profit left in trade
                    continue;
                }
                double t_kill = (coeff_Up*t_cur<2.0?2.0:coeff_Up*t_cur);
                CreateTrade Trade(xData.front(),notional,buySell::SELL,trigger::NEW,t_profit,t_loss, t_kill,t_loss, t_kill);
                trade newTrade = Trade.get();
                xTrades->push_back(newTrade);
                std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&xData.front().timestamp));
                outputFile << "INFO: GapUp;new trade; tradeCount:" << count++ << ";tradeId:" << newTrade.tradeId << ";timeStamp:" << timeString;
                outputFile << ";h_cur:" << h_cur << ";h1:" << h1 <<";price:" << newTrade.price << ";takeProfit:" << t_profit << ";stopLoss:" << t_price << ";killTime:" << t_kill << std::endl;
                no_trend_live = false;
                h0 = t0 = 0.0;
                }
        }
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
    outputFile << "INFO: " << strat << ";" << trigger[unsigned(trig)] << ";tradeId:" << closeoutTrade.tradeId << ";parenttradeId:" << closeoutTrade.parentId  << ";timeStamp:" << timeString;
    ProfitLoss realize(curr, &closeoutTrade);
    outputFile << ";price:" << closeoutTrade.price << ";killTime:" << curr->killTime << ";profit loss for trades:" << realize.calc() << std::endl;
    return realize.calc();
}