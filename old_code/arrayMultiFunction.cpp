#include <iostream>
#include <string>
#include <vector>
using namespace std;

std::vector<string> arrMulti(std::vector<string> var1, std::vector<string> var2) {
    unsigned int arrLen1 = var1.size();
    unsigned int arrLen2 = var2.size();
    int sum = 0;
    std::vector<string> arr1;

    for (unsigned int i=0; i < arrLen1 ; i++)
    {
        for (unsigned int j=0; j < arrLen2 ; j++)
        {
        arr1.push_back(var1[i]+var2[j]);
        //std::cout << "value is " << arr1[sum] << std::endl;
        sum += 1;
        }
    }
    return arr1;
}

int main() 
{
std::vector<std::vector<string>> newArr{ {"a","b","c"},{"x","y"},{"i","j","k"},{"s","t"}}; 
unsigned int arrays = newArr.size();
std::cout << "number of arrays = " << arrays << std::endl;
std::vector<string> arrFinal = newArr[0];
std::vector<string> arrInt;

for (unsigned int x=1; x < arrays ; x++)
    {
    arrInt = arrMulti(arrFinal, newArr[x]);
    arrFinal.clear();
    arrFinal = arrInt;
    arrInt.clear();
    }

for (unsigned int y=1; y < arrFinal.size() ; y++)
    {
    std::cout << arrFinal[y] << std::endl;
    }

std::cout << "Total number of strings in final array " << arrFinal.size() << std::endl;
return 0;
}