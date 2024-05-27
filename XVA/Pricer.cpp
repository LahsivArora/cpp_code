#include <vector>
#include "Pricer.h"
#include "Leg.h"

SwapPricer::SwapPricer(VanillaSwap& swap, RateCurve& curve, double lag){
    xSwap=swap;
    xCurve=curve;
    xLag=lag;
}

double SwapPricer::getLegNPV(int legNum){
    double npv = 0.0;
    Leg calcLeg = xSwap.getLeg(legNum);

    if (calcLeg.getLegType() == LegType::Fixed){
        double periodAdj = 1.0/calcLeg.getFixedFreq();
        double notional = (legNum==1?1.0:-1.0)*xSwap.getNotional();
        double rate = calcLeg.getFixedRate();
        std::vector<double> flow = calcLeg.getLegFlows(xSwap.getMaturity());
        unsigned int num = flow.size();
        std::vector<double> xLeg1disc = xCurve.getDiscFactors(flow);
        for (unsigned int i=0; i < xLeg1disc.size(); i++){
            if (flow[i] > xLag)
                npv += (notional * rate * periodAdj * xLeg1disc[i]);
        }
    }
    else {
        double periodAdj = 1.0/calcLeg.getFloatFreq();
        double notional = (legNum==1?1.0:-1.0)*xSwap.getNotional();
        double spread = calcLeg.getFloatSpread();
        std::vector<double> flow = calcLeg.getLegFlows(xSwap.getMaturity());
        unsigned int num = flow.size();
        std::vector<double> xLeg2disc = xCurve.getDiscFactors(flow);
        std::vector<double> xLeg2fwd = xCurve.getFwdRates(flow);

        for (unsigned int i=0; i < xLeg2disc.size(); i++){
            if (flow[i] > xLag)
                npv += (notional * (xLeg2fwd[i] + spread) * periodAdj * xLeg2disc[i]);
        }
    }
    return npv;
}


double SwapPricer::getTradeNPV(){
    double npv = getLegNPV(1) + getLegNPV(2);
    return npv;
}
