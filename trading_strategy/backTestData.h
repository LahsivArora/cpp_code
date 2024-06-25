#ifndef BACKTESTDATA_H_
#define BACKTESTDATA_H_

#include <fstream>
#include <iostream>
#include <queue>
#include "dataModel.h"
#include "trade.h"
#include "strategy.h"

class Rewind{
private:
    std::string xInPath;
public:
    Rewind(std::string filePath);
    std::queue<tick> load();
};

class Replay{
private:
    std::queue<tick> xData;
    std::vector<trade> xTrades;
    std::string xOutPath;
public:
    Replay(std::queue<tick> mktData, std::string filePath);
    std::pair<std::vector<trade>,std::map<std::string,double>> use();
};

#endif