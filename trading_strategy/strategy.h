#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "datamodel.h"
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <map>

std::map<std::string,double[2]> gapDown(ccyPairDef ccyPair, std::queue<tick> xData, std::vector<trade> *xTrades, 
                                     std::ofstream &outputFile, std::map<std::string,double[2]> PnL);

std::map<std::string,double[2]> gapUp(ccyPairDef ccyPair, std::queue<tick> xData, std::vector<trade> *xTrades, 
                                   std::ofstream &outputFile, std::map<std::string,double[2]> PnL);

double processCloseOut(std::string strat, trade *curr, tick tick, buySell dir, trigger trig, std::ofstream &outputFile);

#endif