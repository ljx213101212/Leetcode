#include "pch.h"
class Solution {
public:
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
     string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
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