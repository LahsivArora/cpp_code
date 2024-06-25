#include "calc.h"
#include <string>
#include <iostream>
#include <fstream>

ProfitLoss::ProfitLoss(trade* trade1, trade* trade2){
    xTrade1=trade1;
    xTrade2=trade2;
}

double ProfitLoss::calc(){
    double profit = 0.0;
    if (xTrade1->notional == xTrade2->notional){

        profit = xTrade1->notional * (xTrade1->price - xTrade2->price);                // calc profit/Loss in JPY
        profit *= (xTrade1->buySell == buySell::SELL?1.0:-1.0);                        // adding direction
        profit /= (xTrade1->tradeId < xTrade2->tradeId?xTrade2->price:xTrade1->price);  // conversion to USD
        xTrade1->status = status::DEAD;
        xTrade2->status = status::DEAD; 
    }
    else
        throw std::string("notionals need to match for P&L calc");

    return profit;
}

void printResult(std::pair<std::vector<trade>,std::map<std::string,double>> result, std::string xOutPath){

    std::ofstream outputFile(xOutPath); // Open log file  

    //std::pair<std::vector<trade>,std::map<std::string,double>> result;
    std::vector<trade> trades = result.first;
    outputFile << "Size of trades vector (i.e. NEW trades):" << trades.size() << std::endl;
    std::map<std::string,double> PnL = result.second;

    outputFile << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    outputFile << "Gap Down Strat: TakeProfit PnL: " << PnL["GapDn_TakeProfit"] << std::endl;
    outputFile << "Gap Down Strat: StopLoss PnL: " << PnL["GapDn_StopLoss"] << std::endl;
    outputFile << "Gap Down Strat: Kill PnL: " << PnL["GapDn_Kill"] << std::endl;
    outputFile << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    outputFile << "Total Gap Down Strat PnL: " << PnL["GapDn_TakeProfit"]+PnL["GapDn_StopLoss"]+PnL["GapDn_Kill"] << std::endl;
    outputFile << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    outputFile << "Gap Up Strat: TakeProfit PnL: " << PnL["GapUp_TakeProfit"] << std::endl;
    outputFile << "Gap Up Strat: StopLoss PnL: " << PnL["GapUp_StopLoss"] << std::endl;
    outputFile << "Gap Up Strat: Kill PnL: " << PnL["GapUp_Kill"] << std::endl;
    outputFile << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    outputFile << "Total Gap Up Strat PnL: " << PnL["GapUp_TakeProfit"]+PnL["GapUp_StopLoss"]+PnL["GapUp_Kill"] << std::endl;
    outputFile << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    outputFile << "Total PnL: " << PnL["GapDn_TakeProfit"]+PnL["GapDn_StopLoss"]+PnL["GapDn_Kill"]+PnL["GapUp_TakeProfit"]+PnL["GapUp_StopLoss"]+PnL["GapUp_Kill"] << std::endl;
    outputFile << "++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    outputFile.close(); // Close log file
}
