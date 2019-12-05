#include "pch.h"
class Solution {
public:
    int countServers(vector<vector<int>>& grid) {
        
        int ret = 0;
        map<int,int> rowC;
        map<int,int> colC;
        
        for (int i = 0 ; i < grid.size(); i++){
            for (int j = 0 ; j < grid[i].size(); j++){
                 if (grid[i][j] == 1){
                     rowC[i] ++;
                     colC[j] ++;
                 }
            }
        }
        
        for (int i = 0 ; i < grid.size(); i++){
            for (int j = 0 ; j < grid[i].size(); j++){
                 if (grid[i][j] == 1){
                     if(rowC[i] > 1 ||
                        colC[j] > 1){
                         ret++;
                     }
                 }
            }
        }
        return ret;
    }
};