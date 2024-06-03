#include "MainData.h"

int main()
{

try
{
    // Step1: setup Trades
    NettingSet *netSet = build_trades().first;
    Swap* XccySwap = build_trades().second->back();

    // Step2: setup MarketData with RateCurves, CDSCurves and FXSpot 
    MarketData* mktData = build_MarketData();
    RateCurve *FundingCurve = mktData->getRateCurve("USD.FUND");

    // Step3a: pricing 1 Swap with MarketData object 
    SwapPricer *xccyPricer = new SwapPricer(XccySwap, mktData);
    std::cout << "NPV of XccySwap (in USD):" << xccyPricer->calcTradeNPV() << std::endl;

    // Step3b: pricing netting set (collection of Swaps) with MarketData object 
    SwapPricer *basePV = new SwapPricer(netSet, mktData);
    double netSetFVA = basePV->calcFVA(*FundingCurve);

    // example for IrDelta calc per curve 
    RiskEngine *riskSet = new RiskEngine(netSet, mktData, "USD.SOFR");
    std::map<double,double> *irDelta = new std::map<double,double>;
    *irDelta = riskSet->calcIRDelta();
    for (auto it=irDelta->begin(); it != irDelta->end(); it++){
        std::cout << "Tenor:" << it->first << " irDelta:" << it->second << std::endl;
    }

    // Step4: simulate curves using base RateCurve object + simulation params. using HW model
    double rateVol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    double a = 0.05; // speed of mean reversion 
    SimulateRate *simEngine = new SimulateRate(mktData, rateVol, a, simPaths);
    std::vector<RateCurve *> *simCurves = new std::vector<RateCurve *>;
    *simCurves = simEngine->getSimulatedCurves();

    // Step5: generate exposure profile using Swap and Simulated curves
    // assuming Quarterly time steps in exposure calculation; exposure is already discounted to today
    ExposureCalc *netSetExProfile = new ExposureCalc(netSet,simCurves,mktData);

    // Step6: calculate CVA, DVA and RWA 
    double ctpyLGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    double ownLGD = (1.0-0.8); // loss given default (LGD) assuming recovery rate (RR) is 80%
    XVACalc *CVA = new XVACalc(netSetExProfile,mktData->getCDSCurve("CTPY"),ctpyLGD,RiskType::CTPY);
    XVACalc *DVA = new XVACalc(netSetExProfile,mktData->getCDSCurve("OWN"),ownLGD,RiskType::OWN);

    std::cout << "For given netting set and market data (all XVA are in $ amount):" << std::endl;
    std::cout << "FVA (+ive means charge to client):" << -1.0*netSetFVA << std::endl;
    std::cout << "CVA (+ive means charge to client):" << CVA->calcXVA() << std::endl;
    std::cout << "DVA (+ive means benefit to bank):" << DVA->calcXVA() << std::endl;
    std::cout << "RWA (using SA-CCR):" << CVA->calcRWA() << std::endl; 
    std::cout << "Initial Margin (using SIMM):" << CVA->calcInitialMargin(mktData) << std::endl; 
    printCount(1, new Leg , new Swap, netSet, new RateCurve, new CDSCurve, xccyPricer, simEngine, riskSet, netSetExProfile, CVA);

}   catch (std::string &err)
        {std::cout << err << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}