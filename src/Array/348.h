#include "pch.h"
class TicTacToe {

public:
    /** Initialize your data structure here. */
    vector<vector<int>> gameboard;
    unordered_map<int,pair<int,int>>  rowCounter;
    unordered_map<int,pair<int,int>>  colCounter;
    int gameSize;
    TicTacToe(int n) {
       gameboard = vector<vector<int>>(n,vector<int>(n,0));
       gameSize = n;
    }
    
    /** Player {player} makes a move at ({row}, {col}).
        @param row The row of the board.
        @param col The column of the board.
        @param player The player, can be either 1 or 2.
        @return The current winning condition, can be either:
                0: No one wins.
                1: Player 1 wins.
                2: Player 2 wins. */
    int move(int row, int col, int player) {
        gameboard[row][col] = player;
        if (player == 1){
            rowCounter[row].first++;
            colCounter[col].first++;
        }else if (player == 2){
            rowCounter[row].second++;
            colCounter[col].second++;
        }else{
            cout << "input error" <<endl;
            return 0;
        }
        //check winning condition
        if (rowCounter[row].first == gameSize
          ||colCounter[col].first == gameSize
          ||rowCounter[row].second == gameSize
          ||colCounter[col].second == gameSize
          || checkDiagonalWinning()){
            return player;
        }
        return 0;
    }

    bool checkDiagonalWinning(){
        //check diagonal #1
        pair<int,int> sumDiagonal_1;
        pair<int,int> sumDiagonal_2;
        for (int i = 0 ; i < gameSize; i++){
            if (gameboard[i][i] == 1){
                sumDiagonal_1.first += 1; 
            }else if (gameboard[i][i] == 2){
                sumDiagonal_1.second +=1;
            }
             if (gameboard[i][gameSize - 1 - i] == 1){
                sumDiagonal_2.first += 1; 
            }else if (gameboard[i][gameSize - 1 - i] == 2){
                sumDiagonal_2.second +=1;
            }
        }
        if (isWinnerSum(sumDiagonal_1.first) ||
            isWinnerSum(sumDiagonal_1.second) ||
            isWinnerSum(sumDiagonal_2.first) ||
            isWinnerSum(sumDiagonal_2.second)){
                return true;
            }
      
        return false;
    }

    bool isWinnerSum(int sum){
        return sum == gameSize;
    }
};