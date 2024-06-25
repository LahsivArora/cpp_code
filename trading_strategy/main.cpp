#include <iostream>
#include <queue>
#include <ctime>
#include "datamodel.h"
#include "calc.h"
#include "trade.h"
#include "backTestData.h"

int main()
{
try
{
    ccyPairDef USDJPY = {ccyPairs::USDJPY, 0.01, 0.4};
    ccyPairDef EURUSD = {ccyPairs::EURUSD, 0.0001, 0.4};

    {Rewind mktData(USDJPY,"./data/USDJPY_Jan2024.csv");
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnlJan24_"+launchTime+".txt");}

    {Rewind mktData(USDJPY,"./data/USDJPY_Feb2024.csv");
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnlFeb24_"+launchTime+".txt");
    usdjpy_d.empty();}

    {Rewind mktData(USDJPY,"./data/USDJPY_Mar2024.csv");
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnlMar24_"+launchTime+".txt");}

    {Rewind mktData(USDJPY,"./data/USDJPY_Apr2024.csv");
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnlApr24_"+launchTime+".txt");}

    {Rewind mktData(USDJPY,"./data/USDJPY_May2024.csv");
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnlMay24_"+launchTime+".txt");}

}   catch (std::string &err)
        {std::cout << "user defined exception caught:" << err << std::endl;}
    catch (std::exception &ex)
        {std::cout << "system defined exception caught:" << ex.what() << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}