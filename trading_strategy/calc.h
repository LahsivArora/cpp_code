#ifndef CALC_H_
#define CALC_H_

#include "datamodel.h"

class ProfitLoss{
private:
    trade *xTrade1, *xTrade2;
public:
    ProfitLoss(trade* trade1, trade* trade2);
    double calc();
};


#endif