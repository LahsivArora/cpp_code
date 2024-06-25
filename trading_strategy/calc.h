#ifndef CALC_H_
#define CALC_H_

#include "datamodel.h"
#include <map>
#include <vector>

class ProfitLoss{
private:
    trade *xTrade1, *xTrade2;
public:
    ProfitLoss(trade* trade1, trade* trade2);
    double calc();
};

void printResult(std::pair<std::vector<trade>,std::map<std::string,double>> result, std::string xOutPath);

#endif