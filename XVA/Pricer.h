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
    // inputs are Swap object or Netting Set and market data (i.e. rate curves + FxSpot) and optional Lag (for exposure calc) 
    SwapPricer(Swap& swap, RateCurve& curve1, RateCurve& curve2, double FxSpot, double lag = 0.0);
    SwapPricer(NettingSet& netSet, RateCurve& curve1, RateCurve& curve2, double FxSpot, double lag = 0.0);
    double calcLegNPV(int legNum);
    double calcTradeNPV();
    double calcFVA(RateCurve& fundCurve);
};

#endif