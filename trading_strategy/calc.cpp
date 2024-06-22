#include "calc.h"
#include <string>

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
        throw std::string("notional need to match for P&L calc");


    return profit;
}