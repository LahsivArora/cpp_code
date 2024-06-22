#include "backTestData.h"

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
        std::string midPrice = line.substr(second+1,third-second-1);
        tick newTick = {std::stoi(dateTick), std::stod(timeTick), std::stod(midPrice)};
        mkt.push(newTick);
        //testing// std::cout << dateTick << "|" << timeTick << "|" << midPrice << "|" << newTick.buyPrice << std::endl; // Print the current line 
    } 

    inputFile.close(); // Close the file
    return mkt;
}

Replay::Replay(std::queue<tick> mktData, std::string filePath){
    xData=mktData;
    xOutPath=filePath;
}

std::vector<trade> Replay::use(){
    
    std::remove("tradeLog.txt");
    std::ofstream outputFile(xOutPath); // Open log file  
    if(outputFile.is_open()){std::cout << xOutPath << std::endl;}
    outputFile << "count" << ";" << "tradeId" << ";" << "timestamp" << std::endl;

    int count = 0;
    while (!xData.empty()){

        double currentPrice = xData.front().price;
        xData.pop();
        double newPrice = xData.front().price;

        if (newPrice > (currentPrice + 0.004)){
            CreateTrade Trade(xData.front(),100000.0,buySell::SELL,trigger::NEW);
            trade newTrade = Trade.get();
            xTrades.push_back(newTrade);
            outputFile << count++ << ";" << newTrade.tradeId << ";" << xData.front().timestamp.time << std::endl;
        }        
    }

    outputFile.close(); // Close log file
    return xTrades;
}
