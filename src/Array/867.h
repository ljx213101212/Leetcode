#include "pch.h"
class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& A) {
        int OR = A.size();
        int OC = A[0].size();
        int retR = OC;
        int retC = OR;
         vector<vector<int>> ret(retR,vector<int>(retC));
        for (int i = 0; i < OR ; i++){
            for (int j = 0; j < OC ; j++){
                ret[j][i] = A[i][j];
            }
        }
        return ret;
    }
};