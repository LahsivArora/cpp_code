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
    double periodAdj = 1.0/calcLeg.getLegFreq();
    double notional = (legNum==1?1.0:-1.0)*xSwap.getNotional()*(legNum==1?1.0:xSwap.getEndFxFwd()); // add conversion for xccy
    double rate = calcLeg.getLegRate();
    std::vector<double> flow = calcLeg.getLegFlows(xSwap.getMaturity());
    std::vector<double> xLegdisc = xCurve.getDiscFactors(flow);

    if (calcLeg.getLegType() == LegType::Fixed){
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag)
                npv += (notional * rate * periodAdj * xLegdisc[i]);
        }
    }
    else {
        std::vector<double> xLegfwd = xCurve.getFwdRates(flow);
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag)
                npv += (notional * (xLegfwd[i] + rate) * periodAdj * xLegdisc[i]);
        }
    }
    return npv;
}


double SwapPricer::getTradeNPV(){
    double npv = getLegNPV(1) + getLegNPV(2);
    return npv;
}
