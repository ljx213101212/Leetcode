#include "pch.h"
#include "String/893.h"

int main(){
    Solution * sol = new Solution();

    vector<string> input = {"abc","acb","bac","bca","cab","cba"};
    int output = sol->numSpecialEquivGroups(input);
    return 0;
}


