#ifndef CHESS_H_
#define CHESS_H_
#include "main.h"


#define SIZE 3  // ∆Â≈Ã≥ﬂ¥Á 3x3
void printBoard(char board[SIZE][SIZE]);
void initBoard(char board[SIZE][SIZE]);
int makeMove(char board[SIZE][SIZE], int row, int col, char piece);
int isMovesLeft(char board[SIZE][SIZE]);
int evaluate(char board[SIZE][SIZE], char ai, char human);
int minimax(char board[SIZE][SIZE], int depth, int isMax, char ai, char human);
void bestMove(char board[SIZE][SIZE], int *bestRow, int *bestCol, char ai, char human);
void aiMove(char board[SIZE][SIZE], char ai, char human);
char getWinner(char board[SIZE][SIZE]);
uint8_t chess_data_transfor(uint8_t row_data ,uint8_t col_data);
void chess_show();
void chess_error();
void now_board_data();

#endif