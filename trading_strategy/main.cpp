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
    Rewind usdjpy_d("./data/USDJPY_Mar2024.csv");
    std::queue<tick> mktData = usdjpy_d.load();
    std::cout << "size of tick queue:" << mktData.size() << std::endl;

    std::string launchTime = std::to_string(std::time(0));
    Replay usdjpy(mktData,"./data/tradeLog_"+launchTime+".txt");
    std::pair<std::vector<trade>,std::map<std::string,double>> result = usdjpy.use();
    
    printResult(result,"./data/pnlSummary_"+launchTime+".txt");

}   catch (std::string &err)
        {std::cout << "user defined exception caught:" << err << std::endl;}
    catch (std::exception &ex)
        {std::cout << "system defined exception caught:" << ex.what() << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}