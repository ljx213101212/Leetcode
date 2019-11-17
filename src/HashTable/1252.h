#include "pch.h"

class Solution {
public:
    int oddCells(int n, int m, vector<vector<int>>& indices) {
        
        const int ROW = n;
        const int COL = m;
        int ret = 0;
        map<int,int> rowCounter;
        map<int,int> colCounter;
        
        for (vector<int> indice : indices){
           rowCounter[indice[0]]++;
           colCounter[indice[1]]++;
        }
       for (int i = 0;  i < ROW; i++){
           for (int j = 0; j < COL; j++){
            int val = rowCounter[i] + colCounter[j];
             if ((val & 1) != 0){
                    ret++;
                }
           }
       }
       return ret; 
    }
};