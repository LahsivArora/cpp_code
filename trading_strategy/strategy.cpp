#include "strategy.h"
#include "trade.h"
#include "calc.h"
#include <cmath>

char timeString[26];
double tolerance = 0.003;
double coeff_Dn = 0.766;
double coeff_Up = 0.65;
double minGap = 10.0;
double jumpCheck_Dn = 1.0 - sqrt(coeff_Dn);
double jumpCheck_Up = 1.0 - sqrt(coeff_Up);
double notional = 1000000.0; // 1mln USD

std::map<std::string,double> gapDown(std::queue<tick> xData, std::vector<trade> *xTrades, std::ofstream &outputFile, std::map<std::string,double> PnL){
    
    int count = 0;
    double h0 = 0.0, h1 = 0.0, h_cur = 0.0;
    double t0 = 0.0, t1 = 0.0, t_cur = 0.0; 
    bool no_trend_live = true;
    while (!xData.empty()){

        double currentPrice = xData.front().buyPrice;
        time_t currentTime = xData.front().timestamp; 
        xData.pop();
        double newPrice = xData.front().buyPrice;
        time_t newTime = xData.front().timestamp; 

        // reset and skip for weekend break
        if (difftime(newTime,currentTime) > 150000){
            t0 = t1 = h0 = h1 = 0.0;
            continue;}

        if ((newPrice <= currentPrice + tolerance) && no_trend_live){
            if (h1 != 0.0){
                h1 = t1 = 0.0;
                h0 = (currentPrice - newPrice);
                t0 = difftime(newTime,currentTime);
                }
            else {
                h0 += (currentPrice - newPrice);
                t0 += difftime(newTime,currentTime);
            }
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h0:" << h0 << std::endl;
        }        
        else {
            h_cur = h0; t_cur = t0;
            h1 += (newPrice - currentPrice);
            t1 += difftime(newTime,currentTime);
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h1:" << h1 << ":" << h_cur << ":" << h_cur*0.15*coeff*coeff << std::endl;
            if (!no_trend_live){
                trade currentTrade = xTrades->back();
                if (xData.front().sellPrice >= currentTrade.takeProfit){
                    PnL["GapDn_TakeProfit"] += processCloseOut("GapDown",&currentTrade,xData.front(),buySell::SELL,trigger::TAKEPROFIT, outputFile);
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (xData.front().sellPrice <= currentTrade.stopLoss){
                    PnL["GapDn_StopLoss"] += processCloseOut("GapDown",&currentTrade,xData.front(),buySell::SELL,trigger::TAKEPROFIT, outputFile);
                    processCloseOut("GapDown",&currentTrade,xData.front(),buySell::SELL,trigger::STOPLOSS, outputFile);
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (difftime(xData.front().timestamp,currentTrade.creationTime) >= currentTrade.killTime){
                    PnL["GapDn_Kill"] += processCloseOut("GapDown",&currentTrade,xData.front(),buySell::SELL,trigger::TAKEPROFIT, outputFile);
                    processCloseOut("GapDown",&currentTrade,xData.front(),buySell::SELL,trigger::KILL, outputFile);
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
            }
            else if (h1 >= (h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn) && no_trend_live && h_cur*100.0 >= minGap){
                double t_price = xData.front().buyPrice;
                double t_loss = t_price - h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn;
                //double t_loss = t_price - (h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn>0.02?0.02:h_cur*jumpCheck_Dn*coeff_Dn*coeff_Dn);
                double t_profit = t_loss + h_cur*coeff_Dn*coeff_Dn;
                if (t_profit <= t_price || h1/(h_cur*coeff_Dn*coeff_Dn) >= 0.75){
                    h0 = t0 = 0.0;
                    outputFile << "WARN: takeProfit trigger doesn't make sense" << std::endl ;
                    continue;
                }
                double t_kill = (coeff_Dn*t_cur<2.0?2.0:coeff_Dn*t_cur);
                CreateTrade Trade(xData.front(),notional,buySell::BUY,trigger::NEW,t_profit,t_loss, t_kill);
                trade newTrade = Trade.get();
                xTrades->push_back(newTrade);
                std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&xData.front().timestamp));
                outputFile << "INFO: GapDown;new trade; tradeCount:" << count++ << ";tradeId:" << newTrade.tradeId << ";timeStamp:" << timeString <<";t_cur:" << t_cur;
                outputFile << ";h_cur:" << h_cur << ";h1:" << h1 <<";price:" << newTrade.price << ";takeProfit:" << t_profit << ";stopLoss:" << (t_loss-t_price)*100.0 << ";killTime:" << t_kill << std::endl;
                no_trend_live = false;
                h0 = t0 = 0.0;
                }
        }
    }
    return PnL;
}


std::map<std::string,double> gapUp(std::queue<tick> xData, std::vector<trade> *xTrades, std::ofstream &outputFile, std::map<std::string,double> PnL){
    
    int count = 0;
    double h0 = 0.0, h1 = 0.0, h_cur = 0.0;
    double t0 = 0.0, t1 = 0.0, t_cur = 0.0; 
    bool no_trend_live = true;
    while (!xData.empty()){

        double currentPrice = xData.front().sellPrice;
        time_t currentTime = xData.front().timestamp; 
        xData.pop();
        double newPrice = xData.front().sellPrice;
        time_t newTime = xData.front().timestamp; 

        if ((newPrice >= currentPrice - tolerance) && no_trend_live){
            if (h1 != 0.0){
                h1 = t1 = 0.0;
                h0 = (newPrice - currentPrice);
                t0 = difftime(newTime,currentTime);
                }
            else {
                h0 += (newPrice - currentPrice);
                t0 += difftime(newTime,currentTime);
            }
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h0:" << h0 << std::endl;
        }        
        else {
            h_cur = h0; t_cur = t0;
            h1 += (currentPrice - newPrice);
            t1 += difftime(newTime,currentTime);
            //std::cout << "currentPrice:" << currentPrice << ";newPrice:" << newPrice << ";h1:" << h1 << ":" << h_cur << ":" << h_cur*0.15*coeff*coeff << std::endl;
            if (!no_trend_live){
                trade currentTrade = xTrades->back();
                if (xData.front().buyPrice <= currentTrade.takeProfit){
                    PnL["GapUp_TakeProfit"] += processCloseOut("GapUp",&currentTrade,xData.front(),buySell::BUY,trigger::TAKEPROFIT, outputFile);
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (xData.front().buyPrice >= currentTrade.stopLoss){
                    PnL["GapUp_StopLoss"] += processCloseOut("GapUp",&currentTrade,xData.front(),buySell::BUY,trigger::STOPLOSS, outputFile);
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
                else if (difftime(xData.front().timestamp,currentTrade.creationTime) >= currentTrade.killTime){
                    PnL["GapUp_Kill"] += processCloseOut("GapUp",&currentTrade,xData.front(),buySell::BUY,trigger::KILL, outputFile);
                    no_trend_live = true;
                    h_cur = h1 = t_cur = t1 = 0.0;
                }
            }
            else if (h1 >= (h_cur*jumpCheck_Up*coeff_Up*coeff_Up) && no_trend_live && h_cur*100.0 > minGap){
                double t_price = xData.front().sellPrice;
                double t_loss = t_price + h_cur*jumpCheck_Up*coeff_Up*coeff_Up;
                //double t_loss = t_price + (h_cur*jumpCheck_Up*coeff_Up*coeff_Up>0.02?0.02:h_cur*jumpCheck_Up*coeff_Up*coeff_Up);
                double t_profit = t_loss - h_cur*coeff_Up*coeff_Up; // t_price + h1 - h_cur*coeff_Up*coeff_Up 
                if (t_profit >= t_price || h1/(h_cur*coeff_Dn*coeff_Dn) >= 0.75){
                    h0 = t0 = 0.0;
                    outputFile << "WARN: takeProfit trigger doesn't make sense" << std::endl;
                    continue;
                }
                double t_kill = (coeff_Up*t_cur<2.0?2.0:coeff_Up*t_cur);
                CreateTrade Trade(xData.front(),notional,buySell::SELL,trigger::NEW,t_profit,t_loss, t_kill);
                trade newTrade = Trade.get();
                xTrades->push_back(newTrade);
                std::strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", std::localtime(&xData.front().timestamp));
                outputFile << "INFO: GapUp;new trade; tradeCount:" << count++ << ";tradeId:" << newTrade.tradeId << ";timeStamp:" << timeString;
                outputFile << ";h_cur:" << h_cur << ";h1:" << h1 <<";price:" << newTrade.price << ";takeProfit:" << t_profit << ";stopLoss:" << (t_loss-t_price)*100.0 << ";killTime:" << t_kill << std::endl;
                no_trend_live = false;
                h0 = t0 = 0.0;
                }
        }
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
    outputFile << ";price:" << closeoutTrade.price << ";profit loss for trades:" << realize.calc() << std::endl;
    return realize.calc();
}