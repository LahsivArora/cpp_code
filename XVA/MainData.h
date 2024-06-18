#ifndef MAINDATA_CPP_
#define MAINDATA_CPP_

#include <iostream>
#include <vector>
#include "NettingSet.h"
#include "MarketData.h"
#include "Pricer.h"
#include "SimModel.h"
#include "Exposure.h"
#include "XVACalc.h"
#include "RiskEngine.h"

#include "MainData.h"

void printDelta(MarketData* mktData,NettingSet* netSet,double FxSpot);
void printCount(bool print, Leg* xLeg, Swap* xSwap, NettingSet* xNetSet, RateCurve* xCurve,CDSCurve* xCDS, SwapPricer* xPricer,  
                SimulateRate* xEngine, RiskEngine* xSet,ExposureCalc* xExp, XVACalc* xXVA,MarketData* xMktData);
std::vector<RateCurve*>* build_RateCurves();
std::vector<CDSCurve*>* build_CDSCurves();
MarketData* build_MarketData();
std::pair<NettingSet*,std::vector<Swap*>*> build_trades();

#endif