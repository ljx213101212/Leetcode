#include "1190.h"

/**
 * @main
// "a(bcdefghijkl(mno)p)q"
//"(abcd)"
//"ta()usw((((a))))"
//"vdgzyj()"
    string input = "vdgzyj()";
    input = "ta()usw((((a))))";
    string output = sol->reverseParentheses(input);
 */
 string Solution::reverseParentheses(string s){
     string res = "";
     //1.利用stack的特性，记录前括号
     //2.若是拿到后括号，把最近一期的前括号拿出来，idx定位reverse
     stack<int> preParenthese;
     for (int i = 0; i < s.length(); i++)
     {
         if (s[i] == '(')
         {
             preParenthese.push(i);
         }
         else if (s[i] == ')')
         {
             int start = preParenthese.top();
             preParenthese.pop();
             //reverse
             std::reverse(s.begin() + start + 1, s.begin() + i);
         }
     }
     //3.把烦人的括号拿掉
     for (char ss : s)
     {
         if (ss == '(' || ss == ')')
         {
             continue;
         }
         res += ss;
     }
     return res;
 }