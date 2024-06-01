#ifndef RATECURVE_H_
#define RATECURVE_H_

#include <vector>
#include <string>
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
    std::string xName;
    std::map<double,double> xRates;
    double interpolate(double x1, double y1, double x2, double y2, double x);
    std::vector<double> tenorMatching (double tenor);
    std::vector<double> getZeroRates(std::vector<double> schedule);
public:
    RateCurve(std::string name = "", std::map<double,double> rates = *(new std::map<double,double>));
    std::map<double,double> getRates();
    std::string getName();
    std::vector<double> getDiscFactors(std::vector<double> schedule);
    std::vector<double> getFwdRates(std::vector<double> schedule);
    RateCurve templateTransform(std::vector<double> schedule);
    RateCurve nameTransform(std::string newName);
};
  
#endif