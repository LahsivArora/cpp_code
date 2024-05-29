#ifndef CDSCURVE_H_
#define CDSCURVE_H_

#include <vector>
#include <map>

class CDSCurve{
private:
    double xSpread, xLGD, xMaturity, xSteps;
public:
    CDSCurve();
    CDSCurve(double CDSSpread, double LGD, double maxMaturity, double timesteps);
    std::map<double,double> calcMarginalPDs();
};
  
#endif