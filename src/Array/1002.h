#include "pch.h"
class Solution {
public:
    vector<string> commonChars(vector<string>& A) {
       vector<string> ret;
        map<char,int> tmp;
        for (int i = 0 ; i < A.size(); i++){
            string s = A[i];
            map<char,int> next;
            for (char c : s){
                if (i == 0){
                    next[c]++;
                }else{       
                    if (tmp[c] > 0){
                        tmp[c]--;
                        next[c]++;
                    }
                }
            }
            tmp = next;
        }
        for (map<char,int>::iterator it = tmp.begin(); it != tmp.end(); ++it){
            for (int i = 0; i < it->second; i++){
                string currStr(1,it->first);
                ret.push_back(currStr);
            }
        }
        return ret;
        
    }
};