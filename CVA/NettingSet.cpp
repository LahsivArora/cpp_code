#include <vector>
#include "Swap.h"
#include "NettingSet.h"
#include "Pricer.h"

NettingSet::NettingSet(){}

NettingSet::NettingSet(std::vector<VanillaSwap> swaps){
    xSwaps=swaps;
}

std::vector<VanillaSwap> NettingSet::getTrades(){
    return xSwaps;
}

double NettingSet::getNoOfTrades(){
    return xSwaps.size();
}

double NettingSet::getMaxMaturity(){
    double n = getNoOfTrades();
    double maturity = 0.0;

    for (double i=0; i<n; i++){
        double currentMat = xSwaps[i].getMaturity();
        maturity = (currentMat>maturity?currentMat:maturity);
    }
    return maturity;
}

double NettingSet::getTradesNPV(RateCurve curve){
    double npvs = 0.0;

    for (double i=0; i<getNoOfTrades();i++){
        SwapPricer price(xSwaps[i],curve);
        npvs += price.getTradeNPV();
    }
    return npvs;
}