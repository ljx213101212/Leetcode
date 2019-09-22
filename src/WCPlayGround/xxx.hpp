#include "pch.h"
class Solution {
public:
    int maxNumberOfBalloons(string text);
    string reverseParentheses(string s);
    string reverseStr(string s);
    string reverseParentheses_2(string s);
    vector<vector<int>> minimumAbsDifference(vector<int>& arr) {
        vector<vector<int>> res;
        int minDiff = pow(20,6);
        sort(arr.begin(),arr.end());
        for (int i = 0; i < arr.size(); i++){
            for (int j = i + 1; j < arr.size(); j++){
                int currDiff = abs(arr[j] - arr[i]);
                if (currDiff < minDiff){
                    res = {};
                    vector<int> currV = {arr[i], arr[j]};
                    res.push_back(currV);
                    minDiff = currDiff;
                }else if (currDiff == minDiff){
                    vector<int> currV = {arr[i], arr[j]};
                    res.push_back(currV);
                }
            }
        }
        return res;
    }

     vector<vector<int>> minimumAbsDifference_2(vector<int>& arr) {
         vector<vector<int>> res;
         int minDiff = pow(20, 6);
         sort(arr.begin(), arr.end());
         for (int i = 1; i < arr.size(); i++)
         {
             int currDiff = abs(arr[i] - arr[i - 1]);
             if (currDiff < minDiff)
             {
                 res = {};
                 vector<int> currV = {arr[i], arr[i - 1]};
                 res.push_back(currV);
                 minDiff = currDiff;
             }
             else if (currDiff == minDiff)
             {
                 vector<int> currV = {arr[i], arr[i - 1]};
                 res.push_back(currV);
             }
         }
         return res;
     }

     int nthUglyNumber(int n, int a, int b, int c) {
        int counter = 0;
        int myGCD = gcd(a,gcd(b,c));
        int res = max(a, max(b,c));
        for (; res <= 2* pow(10,9);res++){

            if (res % a == 0
               || res % b == 0
               || res % c == 0){
                counter++;
            }
            if (counter == n){
                return res;
            }
        }
        return 2 * pow(10,9);
    }

    int gcd(int a, int b)
    {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }
    
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        vector<set<int>> groups;
        for (vector<int> pair : pairs){
            if (groups.size() == 0){ 
                groups.push_back(set<int>(pair.begin(),pair.end()));
                continue;
            }
            for (set<int>& group : groups){
                std::vector<int> v(max(group.size(),pair.size()));
                set_intersection (pair.begin(),pair.end(),group.begin(), group.end(),v.begin());
                if (v.size() > 0){
                    group.insert(pair.begin(),pair.end());
                }else{
                    groups.push_back(set<int>(pair.begin(),pair.end()));
                }
            }
        }
      string res = "";
      for (set<int> group : groups){
          set<int>::iterator iter = group.begin();
          string currGroupString = "";
          for (int g : group){
              currGroupString += s[g];
          }
          sort(currGroupString.begin(),currGroupString.end());

        for (int i = 0 ; i < currGroupString.length(); i++){
             string currS = "";
             currS += currGroupString[*iter];
             res.insert(*iter, currS);
             advance(iter, 1);
        }
        
      }
      return res; 

    }

     const static int N = 1e5 + 10;
     //辅助分组数组
     int f[N];
     //这个方法可以用来进行 下标分组
     //相当于是用第一次进入分组的那个下标为基准，以后所有即将分到此组的f[n] 全部为这个基准值
     int find(int x){
         if (x != f[f[x]]) { f[x] = find(f[x]);}
         return f[x];
     }
     //实在做不来啊, 只好参考小姐姐
     string smallestStringWithSwaps_2(string s, vector<vector<int>>& pairs) {
         //详细分组存储单元： 用A数组来分组， 每组可能会有若干元素，这些个元素就是唯一的数组下标构成的，并且可以互相乱换，也就相当于直接可以拿去sort()一把
         vector<int> A[s.length()];
         //初始化f数组， 清理详细分组存储单元垃圾值
         for (int i = 0 ; i < s.length(); i++){
             f[i] = i;
             A[i].clear(); 
         }
         //遍历pairs, 更新f 数组
         for (int i = 0; i < pairs.size(); i++){
             vector<int> v = pairs[i];
             int rx = find(v[0]);int ry = find(v[1]);
             if (rx != ry){
                //说明这个下标之前没有出现过，建立新的分组, 初始化基准值
                f[ry] = rx;
             }
         }

         //利用辅助分组数组 f[] 更新 详细分组存储单元 A
         for (int i = 0 ; i < s.length(); i++){
             if (f[i] != i){
                 A[find(i)].push_back(i);
             }
             A[i].push_back(i);
         }
         
         string res = s;
         int red_idx = 0;
         //详细分组存储单元A
        for (vector<int> currGroup : A){
            if (currGroup.size() == 1) { continue;}
            //记录一下没有拍过序的下标currGroup 
            vector<int> noSort(currGroup);
            //排序当前分组：按照s[x] char的 ascii值从小到大
            sort(currGroup.begin(), currGroup.end(), [&] (int x,int y){
                  return s[x] < s[y]; 
            });
            //利用新的下标顺序来刷新res的各个下标的值
            for (int i = 0 ; i < currGroup.size(); i++){
                res[noSort[i]] = s[currGroup[i]];
            }
        }
        return res;
     }
};