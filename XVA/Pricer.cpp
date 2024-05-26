#include <vector>
#include "Pricer.h"

SwapPricer::SwapPricer(VanillaSwap &swap, RateCurve &curve, double lag){
    xSwap=swap;
    xCurve=curve;
    xLag=lag;
}

double SwapPricer::getLeg1NPV(){
    double npv = 0.0;
    double periodAdj = 1.0/xSwap.getFixedFreq();
    double notional = xSwap.getNotional();
    double rate = xSwap.getFixedRate();
    std::vector<double> flow1 = xSwap.getLegFlows(1);
    std::vector<double> xLeg1disc = xCurve.getDiscFactors(flow1);

    for (unsigned int i=0; i < xLeg1disc.size(); i++){
        if (flow1[i] > xLag)
            npv += (notional * rate * periodAdj * xLeg1disc[i]);
    }
    return npv;
}

double SwapPricer::getLeg2NPV(){
    double npv = 0.0;
    double periodAdj = 1.0/xSwap.getFloatFreq();
    double notional = -1.0*xSwap.getNotional();
    double spread = xSwap.getFloatSpread();
    std::vector<double> flow2 = xSwap.getLegFlows(2);
    std::vector<double> xLeg2disc = xCurve.getDiscFactors(flow2);
    std::vector<double> xLeg2fwd = xCurve.getFwdRates(flow2);

    for (unsigned int i=0; i < xLeg2disc.size(); i++){
        if (flow2[i] > xLag)
            npv += (notional * (xLeg2fwd[i] + spread) * periodAdj * xLeg2disc[i]);
    }

    return npv;
}

double SwapPricer::getTradeNPV(){
    double npv = getLeg1NPV() + getLeg2NPV();
    return npv;
}
