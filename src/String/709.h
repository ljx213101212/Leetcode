#include "pch.h"
class Solution {
public:

/**
 * @main
 *  string input = "LOVELY Girl, I love@y ou";
    string output = sol->toLowerCase(input);
 */ 
/**
 * Runtime: 0 ms, faster than 100.00% of C++ online submissions for To Lower Case.
Memory Usage: 8.2 MB, less than 76.92% of C++ online submissions for To Lower Case.
 */
    string toLowerCase(string str)
    {
        int gap = 'A' - 'a';
        string res;
        for (char s : str)
        {
            if (s - 'A' < 26 && s - 'A' >= 0)
            {
                res += s - gap;
            }
            else
            {
                res += s;
            }
        }
        return res;
    }
};