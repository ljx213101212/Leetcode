/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
#include "pch.h"
struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * 
 *   Solution * sol = new Solution();
    TreeNode*input1 = sol->GetTreeNode();
    int input2 = 6;
    int input3 = 10;
    int output = sol->rangeSumBST_2(input1, input2, input3);
 */ 

class Solution {
public:
    /**
     * O(2 ^ n)
     * S(2 ^ n)
     */ 

    /**
     * recursion 递归 dfs
     */
    int rangeSumBST(TreeNode* root, int L, int R) {
       
        if (root == nullptr) { return 0;}
        int currVal = root->val >= L && root->val <= R ? root->val : 0;
        return currVal + rangeSumBST(root->left, L,R) + rangeSumBST(root->right , L, R);
    }

/**
 *  借助stack特性 dfs 非递归循环 
 */
    int rangeSumBST_2(TreeNode* root, int L, int R) {
       
        int sum = 0;
        stack<TreeNode*> s;
        if (root == nullptr) { return 0;}
        s.push(root);
        TreeNode* currNode = s.top(); s.pop();
        while (s.size() > 0 || currNode != nullptr){
            if (currNode != nullptr)
            {
                int currVal = currNode->val >= L && currNode->val <= R ? currNode->val : 0;
                sum += currVal;
                s.push(currNode->right);
                s.push(currNode->left);
            }
            currNode = s.top();
            s.pop();
        }
        return sum;
    }

    TreeNode* GetTreeNode(){
        TreeNode* head = new TreeNode(10);
        head->left = new TreeNode(5);
        head->right = new TreeNode(15);
        head->left->left = new TreeNode(3);
        head->left->right = new TreeNode(7);
        head->right->left = new TreeNode(13);
        head->right->right = new TreeNode(18);
        head->left->left->left = new TreeNode(1);
        head->left->left->right = nullptr;
        head->right->right->left = new TreeNode(6);
        return head;
    }
};