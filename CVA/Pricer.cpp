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
    double npv = 0.0;
    double multi = 1.0/xSwap.getFixedFreq();
    double notional = xSwap.getNotional();
    double rate = xSwap.getFixedRate();

    for (int i=0; i < xLeg1disc.size(); i++){
        npv += notional * rate * multi * xLeg1disc[i];
    }
    return npv;
}

double SwapPricer::getLeg2NPV(){
    double npv = 0.0;
    double multi = 1.0/xSwap.getFloatFreq();
    double notional = -1.0*xSwap.getNotional();
    double rate = xSwap.getFloatSpread();

    for (int i=0; i < xLeg2disc.size(); i++){
        npv += notional * (rate + xLeg2fwd[i]) * multi * xLeg2disc[i];
    }

    return npv;
}

double SwapPricer::getTradeNPV(){
    double npv = getLeg1NPV() + getLeg2NPV();
    return npv;
}