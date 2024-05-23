#include <vector>
#include "Swap.h"
#include "Portfolio.h"
#include "Pricer.h"

Portfolio::Portfolio(){}

Portfolio::Portfolio(std::vector<VanillaSwap> swaps){
    xSwaps=swaps;
}

std::vector<VanillaSwap> Portfolio::getTrades(){
    return xSwaps;
}

double Portfolio::getNoOfTrades(){
    return xSwaps.size();
}

double Portfolio::getMaxMaturity(){
    double n = getNoOfTrades();
    double maturity = 0.0;

    for (double i=0; i<n; i++){
        double currentMat = xSwaps[i].getMaturity();
        maturity = (currentMat>maturity?currentMat:maturity);
    }
    return maturity;
}

double Portfolio::getTradesNPV(RateCurve curve){
    double npvs = 0.0;

    for (double i=0; i<getNoOfTrades();i++){
        SwapPricer price(xSwaps[i],curve);
        npvs += price.getTradeNPV();
    }
    return npvs;
}