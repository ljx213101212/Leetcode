#include "pch.h"
class Solution {
public:
    vector<vector<int>> minimumAbsDifference(vector<int>& arr) {
        vector<vector<int>> res;
         int minDiff = pow(20, 6);
         sort(arr.begin(), arr.end());
         for (int i = 1; i < arr.size(); i++)
         {
             int currDiff = abs(arr[i] - arr[i - 1]);
             if (currDiff < minDiff)
             {
                 res = {};
                 vector<int> currV = {arr[i - 1], arr[i]};
                 res.push_back(currV);
                 minDiff = currDiff;
             }
             else if (currDiff == minDiff)
             {
                 vector<int> currV = {arr[i - 1], arr[i]};
                 res.push_back(currV);
             }
         }
         return res;
    }
};