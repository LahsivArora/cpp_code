#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

map<int, int> histogram (vector<int> raw){
set<int> unique;
vector<int>::iterator it1 ;
set<int>::iterator it2 ;
map<int,int> histo;

for (it1 = raw.begin(); it1 != raw.end(); ++it1)
{
    unique.insert(*it1);
}

for (it2 = unique.begin(); it2 != unique.end(); ++it2)
{
    auto it = find(raw.begin(), raw.end(), *it2);
    int count = 0; 
    while (it != raw.end()) { 
        it = find(it + 1, raw.end(), *it2);
        count += 1; 
        }
    histo.insert(std::make_pair(*it2,count));
}
return histo;
}

int main() 
{
vector<int> input = {1,2,3,3,4,4,4,5,5,5,5,6,6,6,6,6,6,2,3,4,42,12,3456,7890,51,7,7,7,8,8,9,11,13};
map<int,int> output = histogram(input);
map<int,int>::iterator it ;

std::cout << output.size() << std::endl;

for (it = output.begin(); it != output.end(); ++it)
{
    std::cout << it->first << " is present " << it->second << " times" << std::endl ;
}

return 0;
}