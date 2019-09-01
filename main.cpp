#include "pch.h"
#include "include/922.hpp"

int main(){
    Solution * sol = new Solution();
    sol->test();
    vector<int> testInput = {1,2,3,4};
    vector<int> result = sol->sortArrayByParityII(testInput);
    return 0;
}