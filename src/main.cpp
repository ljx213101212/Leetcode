#include "pch.h"
#include "Array/985.h"

int main(){
    
    Solution sol;
    vector<int> A = {1,2,3,4};
    vector<vector<int>> queries = {{1,0},{-3,1},{-4,0},{2,3}};
    vector<int> output = sol.sumEvenAfterQueries(A,queries);
 
    return 0;
}


