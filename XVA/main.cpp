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
    double FxSpot = *(*mktData->getFxSpots())[0];

    // Step3a: pricing 1 Swap with MarketData object 
    SwapPricer *xccyPricer = new SwapPricer(XccySwap, mktData);

    // Step3b: pricing netting set (collection of Swaps) with MarketData object 
    SwapPricer *basePV = new SwapPricer(netSet, mktData);
    double netSetFVA = basePV->calcFVA(*FundingCurve);

    printDelta(mktData, netSet, FxSpot);

    // Step4: simulate curves using base RateCurve object + simulation params. using HW model
    double vol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    double a = 0.05; // speed of mean reversion 
    SimulateRate *simEngine1 = new SimulateRate(mktData, vol, a, simPaths);
    std::vector<RateCurve *> *simCurves = new std::vector<RateCurve *>;
    *simCurves = simEngine1->getSimulatedCurves();

    SimulateFxSpot *simEngine2 = new SimulateFxSpot(mktData, vol, simPaths);
    std::vector<double *> *simSpots = new std::vector<double *>;
    *simSpots = simEngine2->getSimulatedFxSpots();

    // Step5: generate exposure profile using Swap and Simulated curves
    // assuming Quarterly time steps in exposure calculation; exposure is already discounted to today
    ExposureCalc *netSetExProfile = new ExposureCalc(netSet,simCurves,simSpots,mktData);

    // Step6: calculate CVA, DVA and RWA 
    double ctpyLGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    double ownLGD = (1.0-0.8); // loss given default (LGD) assuming recovery rate (RR) is 80%
    XVACalc *CVA = new XVACalc(netSetExProfile,mktData->getCDSCurve("CTPY"),ctpyLGD,RiskType::CTPY);
    XVACalc *DVA = new XVACalc(netSetExProfile,mktData->getCDSCurve("OWN"),ownLGD,RiskType::OWN);

    // Step7: print out the results
    std::cout << "NPV of XccySwap (in USD):" << xccyPricer->calcTradeNPV() << std::endl;
    std::cout << "For given netting set and market data (all XVA are in $ amount):" << std::endl;
    std::cout << "FVA (+ive means charge to client):" << -1.0*netSetFVA << std::endl;
    std::cout << "CVA (+ive means charge to client):" << CVA->calcXVA() << std::endl;
    std::cout << "DVA (+ive means benefit to bank):" << DVA->calcXVA() << std::endl;
//    std::cout << "RWA (using SA-CCR):" << CVA->calcRWA() << std::endl; 
//    std::cout << "Initial Margin (using SIMM):" << CVA->calcInitialMargin(mktData) << std::endl; 
    std::cout << "--------------------------------------------------------"<< std::endl;
    printCount(1, new Leg , new Swap, netSet, new RateCurve, new CDSCurve, xccyPricer, simEngine1, new RiskEngine, netSetExProfile, CVA, mktData);

}   catch (std::string &err)
        {std::cout << "user defined exception caught:" << err << std::endl;}
    catch (std::exception &ex)
        {std::cout << "system defined exception caught:" << ex.what() << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}