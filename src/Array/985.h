#include "pch.h"
class Solution {
public:
    vector<int> sumEvenAfterQueries(vector<int>& A, vector<vector<int>>& queries) {
          vector<int> ret;
        int currEven = 0;
        for (int a : A){
            if (a % 2 == 0){
                currEven += a;
            }
        }
        for (vector<int> query : queries){
            int val = query[0];int index = query[1];
            if ((val + A[index]) % 2 == 0){
                if (A[index] % 2 != 0){
                    currEven += val + A[index];
                }else{
                    currEven += val;
                }
                
            }else{
                  if (A[index] % 2 == 0){
                    currEven -= A[index];
                  }
               
            }
            A[index] += val;
            ret.push_back(currEven);
        }
        
        return ret;
    }
};