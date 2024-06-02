#ifndef PRICER_H_
#define PRICER_H_

#include <vector>
#include "NettingSet.h"
#include "MarketData.h"

class SwapPricer{
private:
    NettingSet * xNetSet;
    std::vector<Swap *>* xSwaps;
    Swap * xSwap;
    std::vector<RateCurve*> *xCurves;
    MarketData *xMktData;
    double* xFxSpot;
    double xLag;
public:
    static int counter;
    // inputs are Swap object or Netting Set and market data (i.e. rate curves + FxSpot) and optional Lag (for exposure calc) 
    SwapPricer(Swap* swap = new Swap, MarketData* mktData = new MarketData, double lag = 0.0);
    SwapPricer(NettingSet* netSet= new NettingSet, MarketData* mktData = new MarketData, double lag = 0.0);
    double calcLegNPV(int legNum);
    double calcTradeNPV();
    double calcFVA(RateCurve& fundCurve);
    ~SwapPricer(){--counter;}
};

#endif