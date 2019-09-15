#include "pch.h"
#include "Stack/1190.h"

int main(){
    Solution * sol = new Solution();

// "a(bcdefghijkl(mno)p)q"
//"(abcd)"
//"ta()usw((((a))))"
//"vdgzyj()"
    string input = "vdgzyj()";
    input = "ta()usw((((a))))";
    string output = sol->reverseParentheses(input);
    return 0;
}


