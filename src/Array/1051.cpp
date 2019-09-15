#include "1051.hpp"

/**
 *     Solution * sol = new Solution();

    vector<int> input = {1,1,4,2,1,3};
    int output = sol->heightChecker(input);
 * /

/**
 * 
 * Runtime: 4 ms, faster than 62.82% of C++ online submissions for Height Checker.
Memory Usage: 8.5 MB, less than 100.00% of C++ online submissions for Height Checker.
nlogn(fast sort)
O(nlogn + n)
S(n + 1)
 */
int Solution::heightChecker(vector<int>& h){
    vector<int> tmp(h);
    sort(h.begin(), h.end());
    int res = 0;
    for (int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i] != h[i])
        {
            res++;
        }
    }
    return res;
}

/**
 * Better way
 * 1. 利用输入值范围比较小的特性进行数组穷举 1 - 100   -> Array[101]
 * 2. 利用数组下标来计数  input[1,0,3,3]  counter[0,1,0,0,2]
 * 3. 用一个固定指针后面用于比较，另一个活动指针按照顺序推着counter下标走， 如果counter值为0 ,说明input已经没有这个height的元素了，继续推活动指针，如果有counter 值不为0，
 *    那么ideally 当前 counter值与input值(固定指针的值) 比较，如果不同，说明这个位置的值顺序不对，res ++. counter值--。  最后 无论如何, 固定指针++进行下一循环
 * 
 * O(n + n) -> O(n)
 * S(101 + 1+  1) -> S(1)
 * https://leetcode.com/problems/height-checker/discuss/300472/Java-0ms-O(n)-solution-no-need-to-sort
 */

