#ifndef RATECURVE_H_
#define RATECURVE_H_

#include <vector>
#include <map>

/* Rate Curve Object
   Step1: done
   Step2:
    update functions to work with time displacement for calculating EE
   Step3: using swap rates
    func1: calibrate to get zero rates
    func2: create separate swap and xccy curves
   */

class RateCurve{
private:
    std::map<double,double> xRates;
    double interpolate(double x1, double y1, double x2, double y2, double x);
    std::vector<double> tenorMatching (double tenor);
public:
    // default constructor
    RateCurve();
    // assuming Leg1 is fixed rate (input cashflow freq/year + fixed rate)
    // assuming Leg2 is floating rate (input cashflow freq/year + spread)
    // at trade level, input maturity and notional (+ive is receive fixed/leg1 and -ive is pay fixed/leg1)
    RateCurve(std::map<double,double> rates);
    std::vector<double> getDiscFactors(std::vector<double> schedule);
    std::vector<double> getFwdRates(std::vector<double> schedule);
};


  
#endif