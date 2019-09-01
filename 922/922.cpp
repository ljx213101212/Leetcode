#include "../include/922.hpp"

vector<int> Solution::sortArrayByParityII(vector<int> &A)
{

    vector<int> result;
    vector<int> odd;
    vector<int> even;

    for (auto a : A)
    {
        if (a % 2 == 0)
        {
            even.push_back(a);
        }
        else
        {
            odd.push_back(a);
        }
    }

    int oddPtr = 0;
    int evenPtr = 0;
    for (int i = 0; i < odd.size() * 2; i++)
    {
        if (i % 2 == 0)
        {
            result.push_back(even[evenPtr++]);
        }
        else
        {
            result.push_back(odd[oddPtr++]);
        }
    }
    return result;
}

int Solution::test(){
    return 1;
}