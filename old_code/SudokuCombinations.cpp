#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

int main() 
{

int list[9]={1,2,3,4,5,6,7,8,9};
int items = sizeof(list)/sizeof(list[0]);

int numCheck;
cout << "input a number to check combination" << endl;
cin >> numCheck;

for (int i = 0; i < items ; i++){
    if (list[i] >= numCheck){
        break;
    }
    
    for (int j = i+1; j < items ; j++){
        if (list[j] >= numCheck){
                continue;;
            }
        if (numCheck == list[i]+list[j]){
            std::cout << "eligible combination is: " << list[i] << " + " << list[j] << std::endl;
        }
    }
}

return 0;

}