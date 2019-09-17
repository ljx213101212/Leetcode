#include "pch.h"
class Solution {
public:

/**
 *   vector<string> input = {"abc","acb","bac","bca","cab","cba"};
    int output = sol->numSpecialEquivGroups(input);
 */

/**
 * Signature
 * 我实在没有读懂题目 :()
 * 这题是看了别人的答案 看懂之后自己写的
 * @ref https://leetcode.com/problems/groups-of-special-equivalent-strings/discuss/163891/C%2B%2B-Create-a-signature-for-each-string
 * 
 * 题目大概是说 两个string A　B　分别 奇数idx随便换 或者 偶数idx随便换 能够最终让 A = B
 * 所以这里signature 的概念特别重要，随便换就以为着 一个基本形态 可以变换成 n 种形态， n 太多我们不用考虑, 用set装基本形态的个数就好
 */
    int numSpecialEquivGroups(vector<string>& A) {
        unordered_set<string> res;
        for (string a : A){
            vector<char> even;
            for (int i = 0; i < a.length();i+=2){
                even.push_back(a[i]);
            } 
            vector<char> odd;
            for (int i = 1; i < a.length();i+=2){
                odd.push_back(a[i]);
            }
            sort(even.begin(), even.end());
            sort(odd.begin(), odd.end());
            string even_sig(even.begin(),even.end());
            string odd_sig(odd.begin(),odd.end());
            string signature = even_sig + odd_sig;
            res.insert(signature); 
        }
        return res.size();
    }
};