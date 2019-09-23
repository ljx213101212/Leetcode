#include "pch.h"
class Solution {
public:
     char ROOK = 'R';
    char BISHOP = 'B';
    char pawn = 'p';
    
    /**
     *    vector<vector<char>> input = {{'.','.','.','.','.','.','.','.'},{'.','.','.','p','.','.','.','.'},{'.','.','.','p','.','.','.','.'},{'p','p','.','R','.','p','B','.'},{'.','.','.','.','.','.','.','.'},{'.','.','.','B','.','.','.','.'},{'.','.','.','p','.','.','.','.'},{'.','.','.','.','.','.','.','.'}};
    int output =  sol.numRookCaptures(input);
     */ 
    int numRookCaptures(vector<vector<char>>& board) {
        vector<char>::iterator it;
        
        int maxR = board.size();
        int maxC = board[0].size();
        int res = 0;
        int row = 0;
        int col = 0;
        for (vector<char> b : board){
           it = find(b.begin(),b.end(), ROOK);
           if (it != b.end()){
               col = distance(b.begin(),it);
               break;
           }
           row++;
        }
        int r = row;
        int c = col;
        //check horizon (use col)
        //left
        int cl = c;
        while (cl >= 0){
            if (board[r][cl] == BISHOP){
                break;
            }
            if (board[r][cl] == pawn){
                res++;
                break;
            }
            cl--;
        }
        //right
        int cr = c;
         while (cr < maxC){
            if (board[r][cr] == BISHOP){
                break;
            }
            if (board[r][cr] == pawn){
                res++;
                break;
            }
            cr++;
        }
    
        //check vertical (use row)
        //top
        int rt = r;
        while (rt >= 0){
            if (board[rt][c] == BISHOP){
                break;
            }
            if (board[rt][c] == pawn){
                res++;
                break;
            }
            rt--;
        }
        
        //bottom
        int rb = r;
           while (rb < maxR){
            if (board[rb][c] == BISHOP){
                break;
            }
            if (board[rb][c] == pawn){
                res++;
                break;
            }
            rb++;
        }
        return res;
        
    }


    int numRookCaptures_2(vector<vector<char>>& board) {
        for (int i = 0 ; i< board.size(); i++){
            for(int j = 0; j < board[i].size(); j++){
                if (board[i][j] == ROOK){
                    //top down left right
                    int topGain = catchMacMan(board, i, j , -1, 0);
                    int downGain = catchMacMan(board, i, j , 1, 0);
                    int leftGain = catchMacMan(board, i, j , 0, -1);
                    int rightGain = catchMacMan(board, i, j , 0, 1);
                    return topGain + downGain + leftGain + rightGain;
                }
            }
        }
        return 0;

    }

    int catchMacMan(vector<vector<char>>& board, int row, int col, int dy , int dx){
        int maxR = board.size();
        int maxC = board[0].size();
        while (row < maxR && row >=0 && col < maxC && col >= 0 && board[row][col] != BISHOP){
            if (board[row][col] == pawn){
                return 1;
            }
            row += dy;
            col += dx;
        } 
        return 0; 
    }
};