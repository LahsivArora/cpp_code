#ifndef CDSCURVE_H_
#define CDSCURVE_H_

#include <vector>
#include <map>

class CDSCurve{
private:
    double xSpread, xLGD, xMaturity, xSteps;
public:
    static int counter;
    CDSCurve(double CDSSpread = 0.0, double LGD = 0.0, double maxMaturity = 0.0, double timesteps = 0.0);
    std::map<double,double> calcMarginalPDs();
};
  
#endif