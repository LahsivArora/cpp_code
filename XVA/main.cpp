#include <iostream>
#include <vector>
#include <cmath>
#include "RateCurve.h"
#include "Leg.h"
#include "Swap.h"
#include "Pricer.h"
#include "SimModel.h"
#include "Exposure.h"
#include "CDSCurve.h"
#include "NettingSet.h"
#include "Enums.h"
#include "XVACalc.h"
#include "RiskEngine.h"


int main()
{
    // Step1a: defining portfolio of swaps
    // +ive notional means receive Leg1/pay Leg2 and 4 means Quarterly payments
    Leg *floatLeg = new Leg(LegType::Float,Currency::USD,4.0,-0.0025,"USD.SOFR");
    Leg *fixLeg1 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0480,"USD.SOFR");
    Swap *VanillaSwap1 = new Swap(TradeType::IrSwap,7.0,-1500000.0, *fixLeg1, *floatLeg, NotionalExch::NO);
    Leg *fixLeg2 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0500,"USD.SOFR");
    Swap *VanillaSwap2 = new Swap(TradeType::IrSwap,4.5,1000000.0, *fixLeg2, *floatLeg, NotionalExch::NO);
    Leg *fixLeg3 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0475,"USD.SOFR");
    Swap *VanillaSwap3 = new Swap(TradeType::IrSwap,8.5,1500000.0, *fixLeg3, *floatLeg, NotionalExch::NO);
    Leg *fixLeg4 = new Leg(LegType::Fixed,Currency::USD,4.0,0.0480,"USD.SOFR");
    Swap *VanillaSwap4 = new Swap(TradeType::IrSwap,2.5,-1000000.0, *fixLeg4, *floatLeg, NotionalExch::NO);

    // Step1b: define portfolio with 4 swaps 
    std::vector<Swap *> *trades = new std::vector<Swap *>;
    trades->push_back(VanillaSwap1);
    trades->push_back(VanillaSwap2);
    trades->push_back(VanillaSwap3);
    trades->push_back(VanillaSwap4);
    NettingSet *netSet = new NettingSet(*trades); 

    // define xccy swap
    Leg *xccyFixLeg = new Leg(LegType::Fixed,Currency::EUR,4.0,0.0280,"EUR.XCCY");
    Leg *xccyFltLeg = new Leg(LegType::Float,Currency::USD,4.0,0.0,"USD.SOFR");
    Swap *XccySwap = new Swap(TradeType::XccySwap,7.0,-1500000.0, *xccyFixLeg, *xccyFltLeg, NotionalExch::YES, 1.2418);

    // Step2a:MarketData: RateCurve 
    // defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    // USD curve has hump at 2Y point i.e. slowly downward sloping from 2Y to 10Y
    RateCurve *SOFR = new RateCurve("USD.SOFR",{{0.5,0.0225},{1.0,0.0375},{2.0,0.05},{5.0,0.049},{7.0,0.0475},{10.0,0.045}}); 
    // EUR curve with xccy basis
    RateCurve *EURXCCY = new RateCurve("EUR.XCCY",{{0.5,0.0325},{1.0,0.0300},{2.0,0.03},{5.0,0.030},{7.0,0.0275},{10.0,0.025}});     
    // USD funding curve: 15bp upto 5y; 25bp thereafter
    RateCurve *FundingCurve = new RateCurve("USD.FUND",{{0.5,0.024},{1.0,0.039},{2.0,0.0515},{5.0,0.0505},{7.0,0.05},{10.0,0.0475}});

    // Step2b:MarketData: FXAsset (containing FxSpot; assuming spot is settling today)
    double FxSpot = 1.0856;
    SwapPricer *xccyPricer = new SwapPricer(XccySwap, SOFR, EURXCCY,FxSpot);
    std::cout << "NPV of XccySwap (in USD):" << xccyPricer->calcTradeNPV() << std::endl;

    // Step3: pricing netting set (with Swaps and RateCurve objects) 
    SwapPricer *basePV = new SwapPricer(netSet, SOFR, SOFR,1.0);
    double netSetFVA = basePV->calcFVA(*FundingCurve);
    RiskEngine *riskSet = new RiskEngine(netSet, SOFR);
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
    std::vector<double> simNPVs = simEngine->getSimulatedBaseNPVs(VanillaSwap1);

    // Step5: generate exposure profile using Swap and Simulated curves
    // assuming Quarterly time steps in exposure calculation; exposure is already discounted to today
    ExposureCalc *netSetExProfile = new ExposureCalc(netSet,simCurves);

    // Step6: create CDS curve with marginal default probabilities assuming contant hazard rate    
    double timesteps = 0.25; // quarterly steps to match exposure profile
    double maxmaturity = 10.0; // generate marginal PDs upto 10Y

    double ctpyCDSSpread = 300.0; // assuming constant CDS spread of 250bps for counterparty
    double ctpyLGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    CDSCurve *ctpyCDS = new CDSCurve(ctpyCDSSpread,ctpyLGD,maxmaturity,timesteps);

    double ownLGD = (1.0-0.8); // loss given default (LGD) assuming recovery rate (RR) is 80%
    double ownCDSSpread = 150.0; // assuming constant CDS spread of 150bps for own
    CDSCurve *ownCDS = new CDSCurve(ownCDSSpread,ownLGD,maxmaturity,timesteps);

    // Step7: calculate CVA, DVA and RWA 
    XVACalc *CVA = new XVACalc(*netSetExProfile,*ctpyCDS,ctpyLGD,RiskType::CTPY);
    XVACalc *DVA = new XVACalc(*netSetExProfile,*ownCDS,ownLGD,RiskType::OWN);

    std::cout << "For given netting set and market data (all XVA are in $ amount):" << std::endl;
    std::cout << "FVA (+ive means charge to client):" << -1.0*netSetFVA << std::endl;
    std::cout << "CVA (+ive means charge to client):" << CVA->calcXVA() << std::endl;
    std::cout << "DVA (+ive means benefit to bank):" << DVA->calcXVA() << std::endl;
    std::cout << "RWA (using SA-CCR):" << CVA->calcRWA() << std::endl; 
    std::cout << "Initial Margin (using SIMM):" << CVA->calcInitialMargin() << std::endl; 

    std::cout << "number of Leg objects:" << XccySwap->counter << std::endl; 
    std::cout << "number of Swap objects:" << XccySwap->counter << std::endl; 
    std::cout << "number of NettingSet objects:" << netSet->counter << std::endl; 
    std::cout << "number of RateCurve objects:" << SOFR->counter << std::endl; 
    std::cout << "number of CDSCurve objects:" << ownCDS->counter << std::endl; 
    std::cout << "number of SwapPricer objects:" << xccyPricer->counter << std::endl;
    std::cout << "number of RiskEngine objects:" << riskSet->counter << std::endl;
    std::cout << "number of SimModel objects:" << simEngine->counter << std::endl;
    std::cout << "number of Exposure objects:" << netSetExProfile->counter << std::endl;
    std::cout << "number of XVACalc objects:" << CVA->counter << std::endl;

    return 0;
}