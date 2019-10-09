#include "pch.h"
class Solution {
public:
    int minCostToMoveChips(vector<int>& chips) {
        int ret = pow(10,9);
        for (int i = 0; i < chips.size(); i++){
            int curr = chips[i];
            int currCost = 0;
            for (int j = 0; j < chips.size(); j++){
                if (i == j){
                    continue;
                }
                int currDiff = abs(chips[j] - curr);
                if (currDiff & 1 == 1){
                    currCost ++;
                }
            }
            ret = min(ret,currCost);
        }
        return ret;
    }
};