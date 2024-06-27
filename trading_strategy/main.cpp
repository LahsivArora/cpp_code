#include <iostream>
#include <queue>
#include <ctime>
#include "datamodel.h"
#include "calc.h"
#include "backTestData.h"

// setup ccyPair. definitions available in DataModel file
ccyPairDef USDJPY = {ccyPairs::USDJPY, 0.01, 0.4, 0.75, 0.004, 10.0};
ccyPairDef EURUSD = {ccyPairs::EURUSD, 0.0001, 0.4};

int main()
{
try
{
    {std::string name = "Jan2024_1s";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::SEC);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Jan2024_t";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::TICK);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Feb2024_1s";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::SEC);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Feb2024_t";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::TICK);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Mar2024_1s";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::SEC);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Mar2024_t";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::TICK);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Apr2024_1s";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::SEC);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "Apr2024_t";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::TICK);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "May2024_1s";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::SEC);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}

    {std::string name = "May2024_t";
    Rewind mktData(USDJPY,"./data/USDJPY_"+name+".csv",dataType::TICK);
    std::queue<tick> usdjpy_d = mktData.load();
    std::cout << "size of tick queue:" << usdjpy_d.size() << std::endl;
    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(&usdjpy_d, USDJPY,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = usdjpy.use();    
    printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");}/**/

}   catch (std::string &err)
        {std::cout << "user defined exception caught:" << err << std::endl;}
    catch (std::exception &ex)
        {std::cout << "system defined exception caught:" << ex.what() << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}