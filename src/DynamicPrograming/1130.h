#include "pch.h"
class Solution {
public:
  /**
   *vector<int> input = {6,2,4,9,7};
    int output = sol.mctFromLeafValues(input); 
   */
    int maxi [41][41];
    int memo [41][41]; 
    int mctFromLeafValues(vector<int>& A) {
        int res = 0, n = A.size();
        vector<int> stack = {INT_MAX};
        for (int a : A) {
            while (stack.back() <= a) {
                int mid = stack.back();
                stack.pop_back();
                res += mid * min(stack.back(), a);
            }
            stack.push_back(a);
        }
        for (int i = 2; i < stack.size(); ++i) {
            res += stack[i] * stack[i - 1];
        }
        return res;

    }

    int mctFromLeafValuesDP(vector<int> &A){
        //1. 构造i - j 的最大叶子
        getMaxLeafDP(A);
        //2. 构造DP
        int ret = getMinSumValue(0, A.size() - 1);
        return ret;
    }

    int getMinSumValue(int left, int right){
        if (left >= right) { return 0;}
        //3. memo 记录已经算出来的最小值答案
        if (memo[left][right] != -1) {return memo[left][right];}
        int currMin = INT_MAX;
        for (int i = left; i < right; i++){
            currMin = min(currMin, maxi[left][i] * maxi[i + 1][right] + getMinSumValue(left ,i) + getMinSumValue(i+1, right));  
        }
        memo[left][right] = currMin;
        return currMin;
    }

    void getMaxLeafDP(vector<int> &A){
        memset(memo,-1,sizeof(memo));
        for (int i = 0 ; i < A.size(); i++){
            maxi[i][i] = A[i];
            for (int j = i + 1 ; j < A.size(); j++){
                maxi[i][j] = max(maxi[i][j - 1], A[j]);
            }
        }
    }
};