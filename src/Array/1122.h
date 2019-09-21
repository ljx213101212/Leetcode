#include "pch.h"
class Solution {
public:
    vector<int> relativeSortArray(vector<int>& arr1, vector<int>& arr2) {
    
        vector<int> res;
        set<int> inArr2Idx;
        vector<int> notInArr2Elements;
        for (int i = 0 ; i < arr2.size(); i++){
            // find arr2[i]
            for (int j = 0; j < arr1.size();j++){
                if (arr1[j] == arr2[i]){
                    inArr2Idx.insert(j);
                    res.push_back(arr1[j]);
                }
            }
        }
        
        for (int k = 0; k < arr1.size(); k++){
             if (!inArr2Idx.count(k)){
                 notInArr2Elements.push_back(arr1[k]);
             }
        }
        sort(notInArr2Elements.begin(),notInArr2Elements.end());
        res.insert(res.end(),notInArr2Elements.begin(), notInArr2Elements.end());
        return res;
    }

    //数组下标计数法
    vector<int> relativeSortArray_2(vector<int> & arr1, vector<int> & arr2){
        //arr1.length, arr2.length <= 1000
        //把inpur vector 的值当作数组下标(因为两个input 数组的值范围都是 0 <= x <= 1000)
        int KEY [1001] ={};
        //等下用来排序的阈值, 越大的越排在后面
        int sortVal = 0;
        //默认让所有arr1的值都是 1000 以后且按照数字大小给定相应阈值， 因为等下要优先按照 arr2的顺序排，待会儿覆盖就好
        for (int i = 0; i < arr1.size(); i++){
            KEY[arr1[i]] = 1000 + arr1[i];
        }
        // 用arr2来刷新阈值，从小到大的顺序，于是用先前的sortVal 从0开始计数(重复的没关系，刷新阈值就好，因为arr1.length, arr2.length <= 1000，不会刷爆)
        for (int i = 0; i < arr2.size(); i++){
            KEY[arr2[i]] = sortVal++;
        }
        // 用Key数组的下标定位arr1元素， 用key数组的值来当作阈值来进行排序
        sort(arr1.begin(),arr1.end(), [KEY] (int idx1, int idx2){
            return KEY[idx1] < KEY[idx2];
        });
        return arr1;
    }
};