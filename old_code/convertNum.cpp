#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int main() 
{
string var1;
cout << "input a string number" << endl;
cin >> var1;
unsigned int strLen = var1.length();
int sum=0;
int convertNum=0;
std::cout << "number of digits " << strLen << std::endl;

for (unsigned int i=strLen-1; i == 0 ; i--){
    std::cout << "number is " << var1[i] << std::endl;
    sum += int(var1[i])-48;
    convertNum += (int(var1[i])-48) * int(pow(10,strLen-i-1));
}

std::cout << "sum is " << sum << std::endl;
std::cout << "converted num is " << convertNum << std::endl;

return 0;

}