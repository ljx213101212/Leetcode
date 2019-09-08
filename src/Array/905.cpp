 #include "905.hpp"

/**
 * Runtime: 60 ms, faster than 9.28% of C++ online submissions for Sort Array By Parity.
 * Memory Usage: 10 MB, less than 32.76% of C++ online submissions for Sort Array By Parity.
 * O(n)
 * S(n)
 * 
 * @description 
 * 知识点考察：
 * 1.STL 的基本函数 push_back , insert
 * 2.判断奇数偶数
 *  2.1 常规方法： 模2 
 *  2.2 位运算方法: 看二进制最小位
 * 
 * @Todo
 * 尝试用swap特性来进行 空间复杂度为1的算法
 */
 vector<int> Solution::sortArrayByParity(vector<int>& A) {
     vector<int> res;
     for (int a : A)
     {
         if (a & 1 != 0)
         {
             res.push_back(a);
         }
         else
         {
             res.insert(res.begin(), a);
         }
     }
     return res;
}