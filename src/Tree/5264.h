#include "pch.h"

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class FindElements {
public:
    set<int> s;
    FindElements(TreeNode* root) {
        s.clear();
        recoverTree(root,s);
    }

    void recoverTree(TreeNode *root, set<int> &s)
    {
        if (root == nullptr)
        {
            return;
        }
        if (root->val == -1)
        {
            root->val = 0;
        }
        s.insert(root->val);
        //left
        if (root->left != nullptr)
        {
            root->left->val = 2 * (root->val) + 1;
            recoverTree(root->left, s);
        }
        if (root->right != nullptr)
        {
            root->right->val = 2 * (root->val) + 2;
            recoverTree(root->right, s);
        }
        return;
    }

    bool find(int target)
    {
        return s.find(target) != s.end();
    }
};