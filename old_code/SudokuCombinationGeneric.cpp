#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


std::vector<int> arrMultival(std::vector<int> var1, std::vector<int> var2) {
    unsigned int arrLen1 = var1.size();
    unsigned int arrLen2 = var2.size();
    int sum = 0;
    std::vector<int> arr1;

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

std::vector<string> arrMultistr(std::vector<string> var1, std::vector<int> var2) {
    unsigned int arrLen1 = var1.size();
    unsigned int arrLen2 = var2.size();
    int sum = 0;
    std::vector<string> arr1;

    for (unsigned int i=0; i < arrLen1 ; i++)
    {
        for (unsigned int j=0; j < arrLen2 ; j++)
        {
        arr1.push_back(var1[i]+"+"+std::to_string(var2[j]));
        //std::cout << "value is " << arr1[sum] << std::endl;
        sum += 1;
        }
    }
    return arr1;
}

unsigned int cntDistinct(string str)
{
    // Set to store unique characters in the given string
    std::unordered_set<char> s;
 
    // Loop to traverse the string
    for (unsigned int i = 0; i < str.size(); i++) {
 
        // Insert current character into the set
        s.insert(str[i]);
    }
    return s.size();
}


int main() 
{

std::vector<int> listval{1,2,3,4,5,6,7,8,9};
std::vector<string> liststr{"1","2","3","4","5","6","7","8","9"};
std::vector<std::vector<int>> collection;

int numCheck;
cout << "input a number to check combination" << endl;
cin >> numCheck;
unsigned int numDigits;
cout << "input number of digits in combination" << endl;
cin >> numDigits;

for (unsigned int i = 0; i < numDigits ; i++){
    collection.push_back(listval);
}

std::vector<int> finalval = listval;
std::vector<string> finalstr = liststr;
std::vector<int> intdval;
std::vector<string> intdstr;

for (unsigned int x=1; x < numDigits ; x++)
    {
    intdval = arrMultival(finalval, collection[x]);
    intdstr = arrMultistr(finalstr, collection[x]);

    finalstr.clear();
    finalval.clear();
    finalstr = intdstr;
    finalval = intdval;

    intdval.clear();
    intdstr.clear();
    }

bool orderChk;
for (unsigned int y=1; y < finalval.size() ; y++)
    {
    orderChk = true;
    for (unsigned int z=0; z < numDigits-1; z++){
        if (int(finalstr[y][2*z]) >= int(finalstr[y][2*z+2])){
            orderChk = false;
            break;
        }
    }
    
    if (finalval[y] == numCheck && orderChk ) {
        if (cntDistinct(finalstr[y]) == (numDigits + 1)) {
            std::cout << finalstr[y] << " = " << finalval[y] << std::endl;
        }
    }
}

return 0;

}