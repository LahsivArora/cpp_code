#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "datamodel.h"
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <map>

std::map<std::string,double> gapDown(std::queue<tick> xData, std::vector<trade> *xTrades, std::ofstream &outputFile, std::map<std::string,double> PnL);

std::map<std::string,double> gapUp(std::queue<tick> xData, std::vector<trade> *xTrades, std::ofstream &outputFile, std::map<std::string,double> PnL);

double processCloseOut(std::string strat, trade *curr, tick tick, buySell dir, trigger trig, std::ofstream &outputFile);

#endif