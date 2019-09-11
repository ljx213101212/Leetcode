
#include "977.hpp"


/**
 * Runtime: 128 ms, faster than 13.62% of C++ online submissions for Squares of a Sorted Array.
Memory Usage: 13.3 MB, less than 98.65% of C++ online submissions for Squares of a Sorted Array.
 */
vector<int> Solution::sortedSquares(vector<int>& A){
    std::for_each(A.begin(),A.end(),[](int & x){
        x = pow(x,2);
    });
    sort(A.begin(),A.end());
    return A;
}

/**
 * Utilize the characteristic of non-decreasing order
 * two pointer converge towards center.
 * ref: https://leetcode.com/problems/squares-of-a-sorted-array/discuss/379984/C%2B%2B-solution-using-two-pointers.-Faster-than-94-solutions
 */
vector<int> Solution::sortedSquares_2(vector<int>& A){
       auto start = A.begin();
        auto end = A.end() - 1;
        vector<int> output;

        while(start <= end){

            int startnum = abs(*start);
            int endnum = abs(*end);

            if(startnum >= endnum){
                output.push_back(startnum*startnum);
                start++;
            }else{
                output.push_back(endnum*endnum);
                end--;
            }
        }
        
        reverse(output.begin(), output.end());
        
        return output;
}