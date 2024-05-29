#include <vector>
#include "Swap.h"
#include "NettingSet.h"
#include "Pricer.h"

NettingSet::NettingSet(){}

NettingSet::NettingSet(std::vector<Swap>& swaps){
    xSwaps=swaps;
}

std::vector<Swap> NettingSet::getTrades(){
    return xSwaps;
}

unsigned int NettingSet::getNoOfTrades(){
    return xSwaps.size();
}

double NettingSet::getMaxMaturity(){
    double maturity = 0.0;

    for (unsigned int i=0; i<getNoOfTrades(); i++){
        double currentMat = xSwaps[i].getMaturity();
        maturity = (currentMat>maturity?currentMat:maturity);
    }
    return maturity;
}