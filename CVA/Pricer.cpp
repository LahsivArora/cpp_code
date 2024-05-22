#include <vector>
#include "Swap.h"
#include "Pricer.h"

SwapPricer::SwapPricer(VanillaSwap swap, std::vector<double> leg1disc, std::vector<double> leg2disc, std::vector<double> leg2fwd){
    xSwap=swap;
    xLeg1disc=leg1disc;
    xLeg2disc=leg2disc;
    xLeg2fwd=leg2fwd;
}

double SwapPricer::getLeg1NPV(){
    double npv = 100.0;

    return npv;
}

double SwapPricer::getLeg2NPV(){
    double npv = -90.0;

    return npv;
}

double SwapPricer::getTradeNPV(){
    double npv = getLeg1NPV() + getLeg2NPV();
    return npv;
}