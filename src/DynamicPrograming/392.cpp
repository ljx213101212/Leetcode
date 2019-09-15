#include "392.hpp"

/**
 * Runtime: 84 ms, faster than 12.53% of C++ online submissions for Is Subsequence.
 * Memory Usage: 17 MB, less than 66.67% of C++ online submissions for Is Subsequence.
 * 
 * O(n)
 * S(1)
 * 
 * @todo
 * This is not DP soltion, need to start learning dp method.
 */
bool Solution::isSubsequence(string s, string t){
    int idxS = 0;
    int idxT = 0;
    while (idxT < t.length())
    {
        if (s[idxS] == t[idxT])
        {
            idxS++;
        }
        idxT++;
    }
    if (idxS == s.length())
    {
        return true;
    }
    else
    {
        return false;
    }
}