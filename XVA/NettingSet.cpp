#include <vector>
#include "Swap.h"
#include "NettingSet.h"
#include "Pricer.h"

int NettingSet::counter = 0;

NettingSet::NettingSet(std::vector<Swap *>* swaps){
    xSwaps=swaps;
    counter++;
}

std::vector<Swap *>* NettingSet::getTrades(){
    return xSwaps;
}

unsigned int NettingSet::getNoOfTrades(){
    return xSwaps->size();
}

double NettingSet::getMaxMaturity(){
    double maturity = 0.0;

    for (auto it = xSwaps->begin(); it != xSwaps->end(); it++){
        double currentMat = (*it)->getMaturity();
        maturity = (currentMat>maturity?currentMat:maturity);
    }
    return maturity;
}