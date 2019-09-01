#include "../include/922.hpp"


/**
 * Todo: 1.reduce extra space. 2.reduce extra time complexity
 * O(2n)
 * S(3n + 2)
 */
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

/**
 * Runtime: 76 ms, faster than 99.01% of C++ online submissions for Sort Array By Parity II.
 * Memory Usage: 11.8 MB, less than 56.25% of C++ online submissions for Sort Array By Parity II.
 * 
 * O(n)
 * S(n + 2)
 * 
 * todo: reduce extra space , try to implement S(2)
 */
vector<int> Solution::sortArrayByParityII_2(vector<int> &A)
{
    vector<int> result(A.size());
    int eventPtr = 0;
    int oddPtr = eventPtr + 1;
    for (auto a : A){
        if (a % 2 ==0){
            result[eventPtr] = a;
            eventPtr += 2;
        }else{
            result[oddPtr] = a;
            oddPtr+=2;
        }
    }
    return result;
}
int Solution::test(){
    return 1;
}