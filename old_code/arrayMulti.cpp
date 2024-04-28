#include <iostream>
#include <string>
using namespace std;

int main() 
{
string var1[3] = {"a","b","c"};
string var2[2] = {"x","y"};
int arrLen1 = sizeof(var1)/sizeof(var1[0]);
int arrLen2 = sizeof(var2)/sizeof(var2[0]);
int sum = 0;
string arr1[arrLen1 * arrLen2];

for (int i=0; i < arrLen1 ; i++)
{
    for (int j=0; j < arrLen2 ; j++)
    {
    arr1[sum] = var1[i]+var2[j];
    std::cout << "value is " << arr1[sum] << std::endl;
    sum += 1;
    }
}
return 0;
}