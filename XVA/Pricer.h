#ifndef PRICER_H_
#define PRICER_H_

#include <vector>
#include "Swap.h"
#include "NettingSet.h"
#include "RateCurve.h"

class SwapPricer{
private:
    NettingSet xNetSet;
    std::vector<Swap> xSwaps;
    Swap xSwap;
    RateCurve xCurve1, xCurve2;
    double xFxSpot;
    double xLag;
public:
    // assuming Leg1 is fixed rate and Leg2 is floating rate
    // inputs are swap object, discount factors and fwd rates 
    //SwapPricer(Swap& swap, RateCurve& curve1, double lag = 0.0);
    SwapPricer(Swap& swap, RateCurve& curve1, RateCurve& curve2, double FxSpot, double lag = 0.0);
    SwapPricer(NettingSet& netSet, RateCurve& curve1, RateCurve& curve2, double FxSpot, double lag = 0.0);
    double calcLegNPV(int legNum);
    double calcTradeNPV();
    double calcFVA(RateCurve& fundCurve);
};

#endif