#include "1189.h"
int Solution::maxNumberOfBalloons(string text){
    std::map<char, int> counter;
    for (char t : text)
    {
        if (t == 'l')
        {
            counter[t] += 1;
        }
        else if (t == 'o')
        {
            counter[t] += 1;
        }
        else
        {
            counter[t] += 2;
        }
    }
    int minNum = 10000;
    if (counter['b'] / 2 < minNum)
    {
        minNum = counter['b'] / 2;
    }
    if (counter['a'] / 2 < minNum)
    {
        minNum = counter['a'] / 2;
    }
    if (counter['l'] / 2 < minNum)
    {
        minNum = counter['l'] / 2;
    }
    if (counter['o'] / 2 < minNum)
    {
        minNum = counter['o'] / 2;
    }
    if (counter['n'] / 2 < minNum)
    {
        minNum = counter['n'] / 2;
    }
    return minNum;
}