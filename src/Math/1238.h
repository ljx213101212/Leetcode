#include "pch.h"
class Solution {
public:
    /**
     *  liouzhou_101 和 SeymourLee 的方法都是通过:
     *  1. 构造gray code数组 (有点dp的感觉)
     *     1.1 设n 为需要构建的gary code 二进制版本的元素值的最大长度， 比如n = 3 ,那么 二进制就需要是 010 110 这种, 表达的十进制 范围则是  0 <= x <= 2 ^ n - 1
     *     1.2 类似dp, gray code的后续元素的构建取决于前面的元素,所以这边需要进行 源状态定义， n = 0 ->  array = [0]
     *     1.3 n = 1, 第二个元素则是与前一个元素的二进制形式 有1位不同，这边算法是  array[x] = array[x-1] | 1 << (n - 1) -> array [1] = array [0] | 1 << 0 = 1
     *     1.3 详细说明   
     *           1.循环所有的已有元素（倒序)，分别 + / 或上 2 ^(n-1)
     *           2.把结果push_back
     *           3.循环结束时得到 n 的gary code
     *  2. 利用gray code数组特性 （1. 可以circular使用 2.相邻元素binary只有1位不一样，其它位都1一样) 
     *  3. 从给定的start 开始为下标从gary code数组中取值 一直到 下标为2^n -1 (...11111)，接着下标翻版 从0开始一直到start 前一个下标
     *  比如: n = 3 , start = 5
     *    那么下标数组为:  
     *    [0,1,2,3,4,start(5),6,7]
     *    gary code 数组(简称g) 为
     *    [0,1,3,2,6,7,5,4] (integer 版本)
     *    [0000,0001,0011,0010,0110,0111,0101,0100] (binary 版本)
     * 
     *    g[5],g[6],g[7],g[0]....,g[4] (注意其中每个gary code)
     *    return:
     *    [7,5,4,0,1,3,2,6]
     * 
     * 
     */
    
    //这边我用 dp 来试试生成 gray code
    vector<int> getGrayCodeByDp(int n){
        vector<vector<int>> dp;
        dp.push_back({0});
        for (int i = 1; i  <= n ; i++){
            vector<int> prev = dp[i - 1];
            vector<int> curr(prev.begin(),prev.end());
            for (int j = 0; j < prev.size();j++){
                curr.push_back(prev[prev.size() - 1 - j] | 1 << (i - 1));
            }
            dp.push_back(curr);
        }
        return dp[n];
    }

    vector<int> circularPermutation(int n,int start){

        vector<int> ret;
        vector<int> g = getGrayCodeByDp(n);
        int size = pow(2,n) - 1;
        //find gary code index where g[index] == start
        //then start with idx till the end of 2 ^n -1 ,then fill the rest of elements starts from [0,idx)
        int idx = 0 ;
        while (g[idx] != start) { idx++;}
        for (int i = idx; i <= size; i++){
            ret.push_back(g[i]);
        }
        for (int i = 0 ; i < idx; i++){
            ret.push_back(g[i]);
        }
        return ret;
    }

    
    /**
     * @ref liouzhou_101
     */ 
    vector<int> go(int n)
    {
        if (n == 1)
        {
            return {0, 1};
        }
        vector<int> v = go(n-1);
        vector<int> ret = v;
        reverse(v.begin(), v.end());
        for (auto x : v)
        {
            
            ret.push_back(x|(1<<(n-1)));
        }
        
        return ret;
    }
    vector<int> circularPermutation_liouzhou_101(int n, int start) {
        auto v = go(n);
        vector<int> ret;
        for (int i = 0; i < 1<<n; ++ i)
            if (v[i] == start)
            {
                for (int j = i; j < 1<<n; ++ j) ret.push_back(v[j]);
                for (int j = 0; j < i; ++ j) ret.push_back(v[j]);
                break;
            }
        return ret;
    }

    vector<int> circularPermutation_SeymourLee(int n, int start) {
        vector<int> ans = grayCode(n);
        int i = 0;
        while(ans[i] != start) ans.push_back(ans[i++]);
        return vector<int>(ans.begin()+i, ans.end());
    }

private:
     vector<int> grayCode(int n) {
        if(n == 0) return vector<int>{0};
        vector<int> result = grayCode(n-1);
        int size = result.size();
        for(int i = 0; i < size; i++){
            result.push_back(result[size-i-1] + pow(2, n-1));
        }
        return result;
    }
};
