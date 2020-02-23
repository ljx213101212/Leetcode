#include "pch.h"
/**
 * There are a row of n houses, each house can be painted with one of the three colors: red, blue or green. 
 * The cost of painting each house with a certain color is different. 
 * You have to paint all the houses such that no two adjacent houses have the same color.
 * The cost of painting each house with a certain color is represented by a n x 3 cost matrix. 
 * For example, costs[0][0] is the cost of painting house 0 with color red; costs[1][2] is the cost of painting house 1 with color green,
 * and so on... Find the minimum cost to paint all houses.

Note:
All costs are positive integers.
Example:

Input: [[17,2,17],[16,16,5],[14,3,19]]
Output: 10
Explanation: Paint house 0 into blue, paint house 1 into green, paint house 2 into blue. 
             Minimum cost: 2 + 5 + 3 = 10.
 * 
 */ 
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        
        if (costs.size() == 0){ return 0;}
        //To record the minimum sum up to now. [i]-> row of houses , [j] -> color of houses (fixed size 3 : red ,blue green)
        //dp[i][j] -> the cost of row i and color j -> for example : dp[23][2] -> The cost of row 24 color green
        vector<vector<int>> dp (costs.size(), vector<int>(costs[0].size()));
        dp[0] = costs[0];
        for (int i = 1 ; i < costs.size(); i++){ 
            //To calculate every colors' sum based on revious sum for every row.           
            dp[i][0] = min(dp[i - 1][1] + costs[i][0] , dp[i - 1][2] + costs[i][0]);
            dp[i][1] = min(dp[i - 1][0] + costs[i][1] , dp[i - 1][2] + costs[i][1]);
            dp[i][2] = min(dp[i - 1][0] + costs[i][2] , dp[i - 1][1] + costs[i][2]);
        }
        //Note: The core thought is to create dp model to record useful data. Since at end of each row, we still cannot decide
        //the min sum if we have another row follows until the every last row. (No adjacent house can be printed the same color.)
        //So we just record the data for every circumstance(red, blue ,green).
        return min(dp[costs.size() - 1][0], min(dp[costs.size() - 1][1], dp[costs.size() - 1][2])) ;
    }
  
};