#include "pch.h"
// #include "Array/1217.h"
// #include "DynamicPrograming/1130.h"
//#include "Math/1238.h"
// #include "WCPlayGround/xxx.hpp"
// #include "HashTable/1252.h"
// #include "MyLab/pointers/DynamicArray.h"
// #include "Tree/5264.h"
// #include "String/1268.h"
#include "Array/1213.h"

int main(){
    
    // Solution sol;
    // vector<vector<int>> input = {{40,5}};
    // int output = sol.oddCells(48,37,input);

    // DynamicArray sol;
    // sol.trySomething();

    // TreeNode* node = new TreeNode(-1);
    // node ->left = new TreeNode(-1);
    //     node ->left->left = new TreeNode(-1);
    //          node ->left->right = new TreeNode(-1);
    //     node ->right = new TreeNode(-1);
    // FindElements sol = FindElements(node);
    // bool isFind = sol.find(3);

    // vector<vector<int>> v = {{1,2,3},{4,5,6}};
    // vector<int> &v1 = v[0];
    // v1[0] = 8;
    
    //  char *p;
    //  p = (char*) malloc(10);
    //  for (int i = 0 ; i < 10 ; i++){
    //      p[i] = 'E';
    //  }
    //  char *q = p;
    //  p[0] = 'C';
    //  *(p+2) = 'F';
    //  cout << *(p+1) << endl;
    //  char pa[10];
    // strcpy(pa,p);
    
    // free(p);
    // pa[0] = 'D';
    // return 0;

    //Solution sol;
    // vector<vector<int>> input = {{1,1,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,1}};
    // vector<string> input1 = {"mobile","mouse","moneypot","monitor","mousepad"};
    // string input2 = "mouse";
    // sol.suggestedProducts(input1,input2);

    //#Test case 1
    //TicTacToe toe = TicTacToe(3);
    // toe.move(0, 0, 1);
    // toe.move(0, 2, 2);
    // toe.move(2, 2, 1);
    // toe.move(1, 1, 2);
    // toe.move(2, 0, 1);
    // toe.move(1, 0, 2);
    // int res = toe.move(2, 1, 1);

    //#Test case 2
    // TicTacToe toe = TicTacToe(2);
    // toe.move(0, 0, 2);
    // toe.move(0, 1, 1);
    // int res = toe.move(1, 1, 2);

    //#Test case 3
    // TicTacToe toe = TicTacToe(3);
    // toe.move(0, 0, 1);
    // toe.move(0, 2, 2);
    // toe.move(2, 2, 1);
    // toe.move(1, 1, 2);
    // toe.move(2, 0, 1);
    // toe.move(1, 0, 2);
    // int res = toe.move(2, 1, 1);
    // int output = sol.countServers(input);
    // int output = sol.oddCells(48,37,input);

    Solution sol;
     vector <int> a1 = {1,2,3,4,5};
     vector<int> a2 = {1,2,5,7,9};
     vector<int> a3 = {1,3,4,5,8};

     vector<int> output = sol.arraysIntersection(a1,a2,a3);
}
