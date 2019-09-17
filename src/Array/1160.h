#include "pch.h"

class Solution {
public:

/*
*   vector<string> input1 = {"hello","world","leetcode"};
    string input2 = "welldonehoneyr";
    int output = sol->countCharacters(input1,input2);
*/
    int countCharacters(vector<string>& words, string chars) {
        vector<char> lowerEnglish(26);
        int res = 0;
        for (char c : chars){
            lowerEnglish[c - 'a']++;
        }
        for (string word : words){
            vector<char> tempCounter(lowerEnglish);
            bool isGood = true;
            for (char w : word){
                if (tempCounter[ w - 'a'] != 0){
                    tempCounter[ w - 'a'] --;
                }else{
                    isGood = false;
                    break;
                }
            }
            if (isGood){
                res += word.length();
            }
        }
        return res;
    }
};