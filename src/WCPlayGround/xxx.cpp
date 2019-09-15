
#include "xxx.hpp"


int Solution::maxNumberOfBalloons(string text){
    std::map<char,int> counter;
    for (char t : text){
        if (t == 'l'){
            counter[t]+= 1;
        }else if (t == 'o'){
            counter[t]+= 1;
        }else{
            counter[t]+= 2;
        }
    }
    int minNum = 10000;
    if (counter['b'] / 2 < minNum){
        minNum = counter['b'] / 2;
    }
    if (counter['a'] / 2  < minNum){
        minNum = counter['a'] / 2 ;
    }
    if (counter['l'] / 2  < minNum){
           minNum = counter['l'] / 2 ;
    }
     if (counter['o'] / 2  < minNum){
           minNum = counter['o'] / 2 ;
    }
     if (counter['n'] / 2  < minNum){
           minNum = counter['n'] / 2 ;
    }
    return minNum;
}


string Solution::reverseParentheses(string s){
    if (s == ""){
        return s;
    }
    int front = 0;
    int end = s.length() - 1;
    int frontend = 0;
    bool frontendF = false;
    bool frontF = false;
    bool endF = false;
    while (front < end){
        if (frontF && frontendF){
            break;
        }
        if (s[front] == '('){
            frontF = true;
        }
        if (s[frontend] == ')'){
            frontendF = true;
        }
        if (!frontF){ front++; frontend++;}
        if (frontF && !frontendF){frontend++;}
    }
    if (front >= end){
        return s;
    }
    front++;
    frontend++;
    string frontString = s.substr(0,front - 1);
    string innerString = front + 1 >= frontend ? "" : s.substr(front, frontend-front + 1);
    string endString = frontend < s.length() ? s.substr(frontend) : "";
    return frontString + reverseStr(reverseParentheses(innerString)) + endString;
}

//<abcd> -> <dcba>
//(ab(cd)) -> (())
string Solution::reverseParentheses_2(string s){
    string res = "";
    //1.利用stack的特性，记录前括号
    //2.若是拿到后括号，把最近一期的前括号拿出来，idx定位reverse
    stack<int>preParenthese;
    for (int i = 0 ; i < s.length(); i++){
        if (s[i] == '('){
            preParenthese.push(i);   
        }else if (s[i] == ')'){
            int start = preParenthese.top();
            preParenthese.pop();
            //reverse
            std::reverse(s.begin() + start + 1, s.begin() + i);
        }
    }
    //3.把烦人的括号拿掉
    for (char ss : s){
        if (ss == '('|| ss == ')'){
            continue;
        }
        res += ss;
    }
    return res;
}


// Function to reverse a string 
string Solution::reverseStr(string str) 
{ 
    int n = str.length(); 
    for (int i = 0; i < n / 2; i++) 
        swap(str[i], str[n - i - 1]); 
    return str;
} 
