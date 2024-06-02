#include "MainData.h"

int main()
{

try
{
    // Step1: setup Trades
    NettingSet *netSet = build_trades().first;
    auto it1 = build_trades().second->begin();
    Swap* VanillaSwap = *it1; ++it1; 
    Swap* XccySwap = *it1;

    MarketData* mktData = build_MarketData();
    // Step2a:MarketData: RateCurve 
    std::vector<RateCurve*>* rateCurves = mktData->getRateCurves();
    auto it2 = rateCurves->begin();
    RateCurve *SOFR = *it2; ++it2; 
    RateCurve *EURXCCY = *it2; ++it2;
    RateCurve *FundingCurve = *it2;

    // Step2b:MarketData: CDSCurve 
    std::vector<CDSCurve*>* cdsCurves = mktData->getCDSCurves();
    auto it3 = cdsCurves->begin();
    CDSCurve *ctpyCDS = *it3; ++it3;
    CDSCurve *ownCDS = *it3;

    // Step2c:MarketData: FXAsset (containing FxSpot; assuming spot is settling today)
    double FxSpot = *(mktData->getFxSpots()->front());
    SwapPricer *xccyPricer = new SwapPricer(XccySwap, SOFR, EURXCCY,FxSpot);
    std::cout << "NPV of XccySwap (in USD):" << xccyPricer->calcTradeNPV() << std::endl;

    // Step3: pricing netting set (with Swaps and RateCurve objects) 
    SwapPricer *basePV = new SwapPricer(netSet, SOFR, SOFR,1.0);
    double netSetFVA = basePV->calcFVA(*FundingCurve);
    RiskEngine *riskSet = new RiskEngine(VanillaSwap, SOFR);
    // RiskEngine riskSet(Swap4, SOFR); // riskengine object can be created with 1 or multiple trades
    std::map<double,double> *irDelta = new std::map<double,double>;
    *irDelta = riskSet->calcIRDelta();
    for (auto it=irDelta->begin(); it != irDelta->end(); it++){
        std::cout << "Tenor:" << it->first << " irDelta:" << it->second << std::endl;
    }

    // Step4: simulate curves using base RateCurve object + simulation params. using HW model
    double rateVol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    double a = 0.05; // speed of mean reversion 
    SimulateRate *simEngine = new SimulateRate(SOFR, rateVol, a, simPaths);
    std::vector<RateCurve *> *simCurves = new std::vector<RateCurve *>;
    *simCurves = simEngine->getSimulatedCurves();

    // Step5: generate exposure profile using Swap and Simulated curves
    // assuming Quarterly time steps in exposure calculation; exposure is already discounted to today
    ExposureCalc *netSetExProfile = new ExposureCalc(netSet,simCurves);

    // Step7: calculate CVA, DVA and RWA 
    double ctpyLGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    double ownLGD = (1.0-0.8); // loss given default (LGD) assuming recovery rate (RR) is 80%
    XVACalc *CVA = new XVACalc(netSetExProfile,*ctpyCDS,ctpyLGD,RiskType::CTPY);
    XVACalc *DVA = new XVACalc(netSetExProfile,*ownCDS,ownLGD,RiskType::OWN);

    std::cout << "For given netting set and market data (all XVA are in $ amount):" << std::endl;
    std::cout << "FVA (+ive means charge to client):" << -1.0*netSetFVA << std::endl;
    std::cout << "CVA (+ive means charge to client):" << CVA->calcXVA() << std::endl;
    std::cout << "DVA (+ive means benefit to bank):" << DVA->calcXVA() << std::endl;
    std::cout << "RWA (using SA-CCR):" << CVA->calcRWA() << std::endl; 
    std::cout << "Initial Margin (using SIMM):" << CVA->calcInitialMargin() << std::endl; 
    printCount(0, new Leg , new Swap, netSet, SOFR, ownCDS, xccyPricer, simEngine, riskSet, netSetExProfile, CVA);

}   catch (std::string &err)
        {std::cout << err << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}