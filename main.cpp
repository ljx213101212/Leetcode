#include "pch.h"
#include "include/832.hpp"

int main(){
    Solution * sol = new Solution();

    vector<vector<int>> input = {{1,1,0},{1,0,1},{0,0,0}};
    vector<vector<int>> output = sol->flipAndInvertImage(input);
    return 0;
}