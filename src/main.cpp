#include "pch.h"
// #include "Array/1217.h"
// #include "DynamicPrograming/1130.h"
//#include "Math/1238.h"
// #include "WCPlayGround/xxx.hpp"
// #include "HashTable/1252.h"
// #include "MyLab/pointers/DynamicArray.h"
#include "Tree/5264.h"


int main(){
    
    // Solution sol;
    // vector<vector<int>> input = {{40,5}};
    // int output = sol.oddCells(48,37,input);

    // DynamicArray sol;
    // sol.trySomething();

    TreeNode* node = new TreeNode(-1);
    node ->left = new TreeNode(-1);
        node ->left->left = new TreeNode(-1);
             node ->left->right = new TreeNode(-1);
        node ->right = new TreeNode(-1);
    FindElements sol = FindElements(node);
    bool isFind = sol.find(3);


    return 0;
}
