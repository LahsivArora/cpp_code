#include <vector>
#include "Swap.h"
#include "NettingSet.h"
#include "Pricer.h"

int NettingSet::counter = 0;

NettingSet::NettingSet(std::vector<Swap *> swaps){
    this->xSwaps=swaps;
    counter++;
}

std::vector<Swap *> NettingSet::getTrades(){
    return this->xSwaps;
}

unsigned int NettingSet::getNoOfTrades(){
    return this->xSwaps.size();
}

double NettingSet::getMaxMaturity(){
    double maturity = 0.0;

    for (unsigned int i=0; i< this->getNoOfTrades(); i++){
        double currentMat = (xSwaps[i])->getMaturity();
        maturity = (currentMat>maturity?currentMat:maturity);
    }
    return maturity;
}