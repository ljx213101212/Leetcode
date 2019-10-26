#include "pch.h"
class Solution {
public:
    //dp is the maximum value of any player can earn from picking subarray item from index i to j
    bool stoneGame(vector<int>& piles) {
        int size = piles.size();
        int dp[size][size];
        for (int i = 0; i < size; i++){
            dp[i][i] = piles[i];
        }
        for (int gap = 1 ; gap < size;gap++){
            for (int start = 0; start < size - gap; start ++){
                dp[start][start + gap] = max((piles[start] - dp[start + 1][start + gap]),
                                              piles[start+gap] - dp[start][start + gap - 1]);
            }
        }
        return dp[0][size - 1] > 0;
    }
};