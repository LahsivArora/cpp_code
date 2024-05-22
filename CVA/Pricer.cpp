#include <vector>
#include "Swap.h"
#include "Pricer.h"

SwapPricer::SwapPricer(VanillaSwap swap, RateCurve curve){
    xSwap=swap;
    xCurve=curve;
}

double SwapPricer::getLeg1NPV(){
    double npv = 0.0;
    double multi = 1.0/xSwap.getFixedFreq();
    double notional = xSwap.getNotional();
    double rate = xSwap.getFixedRate();
    std::vector<double> xLeg1disc = xCurve.getDiscFactors(xSwap.getLegFlows(1));

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
    std::vector<double> flow2 = xSwap.getLegFlows(2);
    std::vector<double> xLeg2disc = xCurve.getDiscFactors(flow2);
    std::vector<double> xLeg2fwd = xCurve.getFwdRates(flow2);

    for (int i=0; i < xLeg2disc.size(); i++){
        npv += notional * (rate + xLeg2fwd[i]) * multi * xLeg2disc[i];
    }

    return npv;
}

double SwapPricer::getTradeNPV(){
    double npv = getLeg1NPV() + getLeg2NPV();
    return npv;
}