#include "pch.h"
class Solution {
public:
    vector<int> arraysIntersection(vector<int>& arr1, vector<int>& arr2, vector<int>& arr3) {
        vector<int> res;
        int idx1 = 0 ,idx2 = 0 ,idx3 = 0;
        int iterTimes = min(min(arr1.size(),arr2.size()),arr3.size());
        while (idx1 < iterTimes && idx2 < iterTimes && idx3 < iterTimes) {
            vector<int> threeItems =  {arr1[idx1], arr2[idx2],arr3[idx3]};
            if (checkIntersection(threeItems)){
                res.push_back(arr1[idx1]);
            }
            int idxOfMin = getIndexOfMin(threeItems);
            if (idxOfMin == 0){
                idx1 ++;
            }else if (idxOfMin == 1){
                idx2 ++;
            }else if (idxOfMin == 2){
                idx3 ++;
            }
        }
        return res;
    }
    
    bool checkIntersection(vector<int>threeItems){
        set<int> s;
        for (int item : threeItems){
            s.insert(item);
        }   
        return s.size() == 1;
    }
    
    int getIndexOfMin(vector<int> threeItems){
        int ret;
        int min = INT_MAX;
        for (int i = 0 ; i < threeItems.size(); i++){
            if (min > threeItems[i]){
                min = threeItems[i];
                ret = i;
            }
        }
        return ret;
    }
    
};