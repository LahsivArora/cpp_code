#include "MainData.h"

void printCount(bool print, Leg* xLeg, Swap* xSwap, NettingSet* xNetSet, RateCurve* xCurve,CDSCurve* xCDS, 
                SwapPricer* xPricer, SimulateRate* xEngine, RiskEngine* xSet,ExposureCalc* xExp, XVACalc* xXVA,MarketData* xMktData){
    
    if (print){
        std::cout << "number of Leg objects:" << xLeg->counter << std::endl; 
        std::cout << "number of Swap objects:" << xSwap->counter << std::endl; 
        std::cout << "number of NettingSet objects:" << xNetSet->counter << std::endl; 
        std::cout << "number of RateCurve objects:" << xCurve->counter << std::endl; 
        std::cout << "number of CDSCurve objects:" << xCDS->counter << std::endl; 
        std::cout << "number of SwapPricer objects:" << xPricer->counter << std::endl;
        std::cout << "number of RiskEngine objects:" << xSet->counter << std::endl;
        std::cout << "number of SimModel objects:" << xEngine->counter << std::endl;
        std::cout << "number of Exposure objects:" << xExp->counter << std::endl;
        std::cout << "number of XVACalc objects:" << xXVA->counter << std::endl;
        std::cout << "number of MarketData objects:" << xMktData->counter << std::endl;
        std::cout << "--------------------------------------------------------"<< std::endl;
    }
}

std::vector<RateCurve*>* build_RateCurves(){
    // Step2a:MarketData: RateCurve 
    // defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    // USD curve has hump at 2Y point i.e. slowly downward sloping from 2Y to 10Y
    RateCurve *SOFR = new RateCurve("USD.SOFR",{{0.5,0.0225},{1.0,0.0375},{2.0,0.05},{5.0,0.049},{7.0,0.0475},{10.0,0.045}}); 
    // EUR curve with xccy basis
    RateCurve *EURXCCY = new RateCurve("EUR.XCCY",{{0.5,0.0325},{1.0,0.0300},{2.0,0.03},{5.0,0.030},{7.0,0.0275},{10.0,0.025}});     
    // USD funding curve: 15bp upto 5y; 25bp thereafter
    RateCurve *FundingCurve = new RateCurve("USD.FUND",{{0.5,0.024},{1.0,0.039},{2.0,0.0515},{5.0,0.0505},{7.0,0.05},{10.0,0.0475}});

    std::vector<RateCurve*>* curves = new std::vector<RateCurve*>;
    curves->push_back(SOFR);
    curves->push_back(EURXCCY);
    curves->push_back(FundingCurve);
    return curves;
}

std::vector<CDSCurve*>* build_CDSCurves(){
    // Step6: create CDS curve with marginal default probabilities assuming contant hazard rate    
    double timesteps = 0.25; // quarterly steps to match exposure profile
    double maxmaturity = 10.0; // generate marginal PDs upto 10Y

    double ctpyCDSSpread = 300.0; // assuming constant CDS spread of 250bps for counterparty
    double ctpyLGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    CDSCurve *ctpyCDS = new CDSCurve("CTPY",ctpyCDSSpread,ctpyLGD,maxmaturity,timesteps);

    double ownLGD = (1.0-0.8); // loss given default (LGD) assuming recovery rate (RR) is 80%
    double ownCDSSpread = 150.0; // assuming constant CDS spread of 150bps for own
    CDSCurve *ownCDS = new CDSCurve("OWN",ownCDSSpread,ownLGD,maxmaturity,timesteps);
    
    std::vector<CDSCurve*>* curves = new std::vector<CDSCurve*>;
    curves->push_back(ctpyCDS);
    curves->push_back(ownCDS);
    return curves;
}

MarketData* build_MarketData(){
    double* FxSpot = new double; 
    *FxSpot = 1.0856;
    std::vector<double*>* fxSpots = new std::vector<double*>;
    fxSpots->push_back(FxSpot);

    MarketData* mktData = new MarketData;
    *mktData = MarketData(build_RateCurves(),build_CDSCurves(),fxSpots);
    return mktData; 
}

std::pair<NettingSet*,std::vector<Swap*>*> build_trades(){
    // Step1a: defining portfolio of swaps
    // +ive notional means receive Leg1/pay Leg2 and 4 means Quarterly payments
    Leg *floatLeg = new Leg(LegType::Float,Currency::USD,4.0,-0.0025,"USD.SOFR");
    Leg *fixLeg1 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0480,"USD.SOFR");
    Swap *VanillaSwap1 = new Swap(TradeType::IrSwap,7.0,-1500000.0, fixLeg1, floatLeg, NotionalExch::NO);
    Leg *fixLeg2 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0500,"USD.SOFR");
    Swap *VanillaSwap2 = new Swap(TradeType::IrSwap,4.5,1000000.0, fixLeg2, floatLeg, NotionalExch::NO);
    Leg *fixLeg3 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0475,"USD.SOFR");
    Swap *VanillaSwap3 = new Swap(TradeType::IrSwap,8.5,1500000.0, fixLeg3, floatLeg, NotionalExch::NO);
    Leg *fixLeg4 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0480,"USD.SOFR");
    Swap *VanillaSwap4 = new Swap(TradeType::IrSwap,2.5,-1000000.0, fixLeg4, floatLeg, NotionalExch::NO);

    // Step1b: define portfolio with 4 swaps 
    std::vector<Swap *> *trades = new std::vector<Swap *>;
    trades->push_back(VanillaSwap1);
    trades->push_back(VanillaSwap2);
    trades->push_back(VanillaSwap3);
    trades->push_back(VanillaSwap4);
    NettingSet *netSet = new NettingSet(trades); 
    
    // define xccy swap
    Leg *xccyFixLeg = new Leg(LegType::Fixed,Currency::EUR,4.0,0.0300,"EUR.XCCY");
    Leg *xccyFltLeg = new Leg(LegType::Float,Currency::USD,4.0,0.0030,"USD.SOFR");
    Swap *XccySwap = new Swap(TradeType::XccySwap,8.5,-2000000.0, xccyFixLeg, xccyFltLeg, NotionalExch::YES, 1.2418);

    std::vector<Swap *> *tradesRet = new std::vector<Swap *>;
    tradesRet->push_back(VanillaSwap1);
    tradesRet->push_back(XccySwap);
    // return VanillaSwap1 and XccySwap
    return std::pair<NettingSet*,std::vector<Swap*>*>(netSet,tradesRet);
}