#include "../include/832.hpp"

/**
 * Runtime: 16 ms, faster than 11.00% of C++ online submissions for Flipping an Image.
 * Memory Usage: 9.7 MB, less than 8.00% of C++ online submissions for Flipping an Image.
 * O(n^2) 
 * S(m * n)
 */
vector<vector<int>> Solution::flipAndInvertImage(vector<vector<int>>& A){
    vector<vector<int>> res{};
    for (vector<int> a : A){
        vector<int>resItem{};
        for (int aItem : a){
            int flipedItem = aItem ^ 1;
            resItem.insert(resItem.begin(), flipedItem);
        }
        res.push_back(resItem);
    }
    return res;
}