#include "backTestData.h"
#include "calc.h"
#include <cmath>

Rewind::Rewind(ccyPairDef ccyPair, std::string filePath, dataType dataType){
    xCcyPair=ccyPair;    
    xInPath=filePath;
    xType=dataType;
}

std::queue<tick> Rewind::load(){
        
    std::queue<tick> mkt;
    std::ifstream inputFile(xInPath); // Open the file  
    std::string line; 
    int count = 0;
    
    while (std::getline(inputFile, line)) { 
        unsigned int first = line.find(",");
        unsigned int second = (line.substr(first+1)).find(",") + first + 1;
        unsigned int third = (line.substr(second+1)).find(",") + second + 1;
        std::string dateTick = line.substr(0,first); // expected format is YYYYMMDD
        std::string timeTick = (line.substr(first+1,second-first-1)); // expected format is HHMMSS.fff

        std::tm tick_tm; 
        tick_tm.tm_sec = std::stoi(timeTick.substr(4,2));
        tick_tm.tm_min = std::stoi(timeTick.substr(2,2));
        tick_tm.tm_hour = std::stoi(timeTick.substr(0,2));
        tick_tm.tm_mday = std::stoi(dateTick.substr(6,2));
        tick_tm.tm_mon = std::stoi(dateTick.substr(4,2))-1;
        tick_tm.tm_year = std::stoi(dateTick.substr(0,4))-1900;
        time_t dateTime = mktime(&tick_tm);

        double millisec = std::stoi(timeTick.substr(7,3))/1000.0;

        double askPrice = std::stod(line.substr(second+1,third-second-1));
        double bidPrice = std::stod(line.substr(third+1,sizeof(line)-third));
        double midPrice = (askPrice+bidPrice)/2.0;
        double spread = (xCcyPair.pipSize * xCcyPair.spread)/2.0; 
        tick newTick = {xType, dateTime, millisec, midPrice, midPrice-spread, midPrice+spread};

        // filtering out ticks with data issues
        if( std::abs(mkt.front().price - midPrice) < 1000.0*xCcyPair.pipSize || mkt.empty())
            mkt.push(newTick);
        else
            ++count;
    }
    inputFile.close(); // Close the file
    std::cout << ":" << "filtered out:" << count << " ticks" << std::endl;
    return mkt;
}

Replay::Replay(std::queue<tick> mktData, ccyPairDef ccyPair, std::string filePath){
    xData=mktData;
    xCcyPair=ccyPair;
    xOutPath=filePath;
}

std::pair<std::vector<trade>,std::map<std::string,double[2]>> Replay::use(){
    
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result;
    std::ofstream outputFile(xOutPath); // Open log file  
    std::map<std::string,double[2]> PnL;

    PnL["GapDn_Kill"][0] = 0.0;
    PnL["GapDn_StopLoss"][0] = 0.0;
    PnL["GapDn_TakeProfit"][0] = 0.0;
    PnL = gapDown(xCcyPair, xData, &xTrades, outputFile, PnL);

    PnL["GapUp_Kill"][0] = 0.0;
    PnL["GapUp_StopLoss"][0] = 0.0;
    PnL["GapUp_TakeProfit"][0] = 0.0;
    PnL = gapUp(xCcyPair, xData, &xTrades, outputFile, PnL);

    outputFile.close(); // Close log file
    result.first = xTrades;
    result.second = PnL;
    return result;
}
