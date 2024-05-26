#include <vector>
#include <cmath>
#include "RateCurve.h"

RateCurve::RateCurve(){}

RateCurve::RateCurve(std::map<double,double> rates){
    xRates=rates;
}

std::map<double,double> RateCurve::getRates(){
    return xRates;
}


double RateCurve::interpolate(double x1, double y1, double x2, double y2, double x){
    double val;
    if (x1 == x2)
        val = y1;
    else
        val = y1 + (y2-y1)*(x-x1)/(x2-x1);

    return val;
}

std::vector<double> RateCurve::tenorMatching(double tenor){
    std::vector<double> coord(4);

    bool is_in = xRates.find(tenor) != xRates.end();;

    if (is_in){
        coord[0] = tenor;
        coord[1] = xRates[tenor];
        coord[2] = tenor;
        coord[3] = xRates[tenor];
    }
    else {
        for (auto it = xRates.begin(); it != xRates.end(); ++it) {
            if (it->first < tenor){
                coord[0] = it->first;
                coord[1] = it->second;
            }

            if (it->first > tenor && (coord[2]==0 || it->first < coord[2])){
                coord[2] = it->first;
                coord[3] = it->second;
            }
        }
    }

    return coord;
}

std::vector<double> RateCurve::getDiscFactors(std::vector<double> schedule){
    std::vector<double> dfs;

    for (auto it = schedule.begin(); it != schedule.end(); ++it) {
        std::vector<double> coord = tenorMatching(*it) ;
        double interRate = interpolate(coord[0],coord[1],coord[2],coord[3],*it);
        double df = 1.0/pow((1+interRate),*it);
        // for debugging: double df = interRate;
        dfs.push_back(df);
    }
    return dfs;
}

std::vector<double> RateCurve::getFwdRates(std::vector<double> schedule){
    std::vector<double> rates, fwds;
    double freq = 1.0/schedule[0];

    for (auto it = schedule.begin(); it != schedule.end(); ++it) {
        std::vector<double> coord = tenorMatching(*it) ;
        //std::cout << coord[0] << " " << coord[1] << " " << coord[2] << " " << coord[3] << std::endl;
        double interRate = interpolate(coord[0],coord[1],coord[2],coord[3],*it);
        rates.push_back(interRate);
    }

    for (unsigned int i = 0; i < schedule.size(); ++i) {
        if (i == 0)
            fwds.push_back(rates[i]);
        else{
            double fwd = (pow((1.0+rates[i]),schedule[i])/pow((1.0+rates[i-1]),schedule[i-1])-1.0)*freq;
            fwds.push_back(fwd);
        }
    }

    return fwds;
}

