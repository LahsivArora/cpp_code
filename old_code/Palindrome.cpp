#include <iostream>
#include <string>
#include<bits/stdc++.h> 
using namespace std;

int main() 
{
string var1;
cout << "input a word" << endl;
cin >> var1;
transform(var1.begin(), var1.end(), var1.begin(), ::tolower);
int strLen = int(var1.length());
int loop = (strLen % 2) ? strLen/2 : (strLen- 1)/2;
bool palindrome = true;

for (int i=0; i < loop ; i++){
    std::cout << "comparing " << var1[i] << " and " << var1[strLen-1-i] << std::endl;
    if (var1[i] != var1[strLen-1-i]){
        palindrome = false;
        break;
    }
}

if (palindrome)
    std::cout << var1 << " is a palindrome" << std::endl;
else 
    std::cout << var1 << " is not a palindrome" << std::endl;

return 0;
}