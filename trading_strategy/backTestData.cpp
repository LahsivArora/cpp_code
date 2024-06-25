#include "backTestData.h"
#include "calc.h"

Rewind::Rewind(std::string filePath){
    xInPath=filePath;
}

std::queue<tick> Rewind::load(){
        std::queue<tick> mkt;

    std::ifstream inputFile(xInPath); // Open the file  
    std::string line; 
    
    while (std::getline(inputFile, line)) { 
        unsigned int first = line.find(",");
        unsigned int second = (line.substr(first+1)).find(",") + first + 1;
        unsigned int third = (line.substr(second+1)).find(",") + second + 1;
        std::string dateTick = line.substr(0,first);
        std::string timeTick = (line.substr(first+1,second-first-1)).replace(5,1,"").replace(2,1,""); 

        std::tm tick_tm; 
        tick_tm.tm_sec = std::stoi(timeTick.substr(4,2));
        tick_tm.tm_min = std::stoi(timeTick.substr(2,2));
        tick_tm.tm_hour = std::stoi(timeTick.substr(0,2));
        tick_tm.tm_mday = std::stoi(dateTick.substr(6,2));
        tick_tm.tm_mon = std::stoi(dateTick.substr(4,2))-1;
        tick_tm.tm_year = std::stoi(dateTick.substr(0,4))-1900;
        time_t dateTime = mktime(&tick_tm);

        std::string midPrice = line.substr(second+1,third-second-1);
        tick newTick = {dateTime, std::stod(midPrice)};
        mkt.push(newTick);
    } 

    inputFile.close(); // Close the file
    return mkt;
}

Replay::Replay(std::queue<tick> mktData, std::string filePath){
    xData=mktData;
    xOutPath=filePath;
}

std::pair<std::vector<trade>,std::map<std::string,double>> Replay::use(){
    
    std::pair<std::vector<trade>,std::map<std::string,double>> result;
    std::ofstream outputFile(xOutPath); // Open log file  
    std::map<std::string,double> PnL;

    PnL["GapDn_Kill"] = 0.0;
    PnL["GapDn_StopLoss"] = 0.0;
    PnL["GapDn_TakeProfit"] = 0.0;
    PnL = gapDown(xData, &xTrades, outputFile, PnL);

    PnL["GapUp_Kill"] = 0.0;
    PnL["GapUp_StopLoss"] = 0.0;
    PnL["GapUp_TakeProfit"] = 0.0;
    PnL = gapUp(xData, &xTrades, outputFile, PnL);

    outputFile.close(); // Close log file
    result.first = xTrades;
    result.second = PnL;
    return result;
}
