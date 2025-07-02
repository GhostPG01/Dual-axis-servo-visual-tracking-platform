
#include <stdio.h>
#include <stdlib.h>
#include "chess.h"
#include "OLED.h"
//row行数据 ，col列数据
uint8_t r_data=0,c_data =0;
uint8_t row_error_null=0,col_error_null=0,row_error_chess,col_error_chess;
uint8_t chess_set_error = 0;
extern uint8_t board[3][3],data[10];
extern uint8_t time_flag;
uint8_t now_board[3][3];
//2是白棋 1是黑棋

// 初始化棋盘，将所有单元格置为空格 ' '
void initBoard(char board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

// 打印棋盘，用于调试或显示当前棋局状态
void printBoard(char board[SIZE][SIZE]) {
    int i, j;
    printf("\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1)
                printf("|");
        }
        printf("\n");
        if (i < SIZE - 1)
            printf("---+---+---\n");
    }
    printf("\n");
}

// 尝试在指定位置落子，若该位置为空则放置棋子并返回 1，否则返回 0
int makeMove(char board[SIZE][SIZE], int row, int col, char piece) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return 0;
    }
    if (board[row][col] == 0) {
        board[row][col] = piece;
        return 1;
    }
    return 0;
}

// 判断棋盘中是否还存在空位，若存在返回 1，否则返回 0（棋盘已满）
int isMovesLeft(char board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0)
                return 1;
        }
    }
    return 0;
}

// 评估当前棋盘状态
// 若 ai（电脑）赢则返回 +10，若 human 赢则返回 -10，否则返回 0
int evaluate(char board[SIZE][SIZE], char ai, char human) {
    int i;
    // 检查行
    for (i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == ai)
                return 10;
            else if (board[i][0] == human)
                return -10;
        }
    }
    // 检查列
    for (i = 0; i < SIZE; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == ai)
                return 10;
            else if (board[0][i] == human)
                return -10;
        }
    }
    // 检查两条对角线
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == ai)
            return 10;
        else if (board[0][0] == human)
            return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == ai)
            return 10;
        else if (board[0][2] == human)
            return -10;
    }
    return 0;
}

// 使用 Minimax 算法计算最佳分数
// 参数 depth 用以减少获胜步数的影响（提倡快赢或慢输）
// 参数 isMax 指示当前搜索层次属于最大化还是最小化
int minimax(char board[SIZE][SIZE], int depth, int isMax, char ai, char human) {
    int score = evaluate(board, ai, human);
    
    // 若终局则返回分数，减去 depth 可使得越快获胜得分更高
    if (score == 10)
        return score - depth;
    if (score == -10)
        return score + depth;
    
    // 若棋盘无空位，则为平局
    if (!isMovesLeft(board))
        return 0;
    
    int best;
    int i, j;
    if (isMax) {
        best = -1000;
        // 遍历所有空位，模拟 ai 落子
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = ai;
                    int value = minimax(board, depth + 1, 0, ai, human);
                    if (value > best)
                        best = value;
                    board[i][j] = 0; // 撤销落子
                }
            }
        }
    } else {
        best = 1000;
        // 遍历所有空位，模拟 human 落子
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = human;
                    int value = minimax(board, depth + 1, 1, ai, human);
                    if (value < best)
                        best = value;
                    board[i][j] = 0; // 撤销落子
                }
            }
        }
    }
    return best;
}

// 根据 Minimax 算法寻找最佳落子位置，并通过 bestRow 与 bestCol 返回该位置
void bestMove(char board[SIZE][SIZE], int *bestRow, int *bestCol, char ai, char human) {
    int bestVal = -1000;
    *bestRow = -1;
    *bestCol = -1;
    int i, j;
    
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                board[i][j] = ai;
                int moveVal = minimax(board, 0, 0, ai, human);
                board[i][j] = 0; // 恢复空位
                if (moveVal > bestVal) {
                    *bestRow = i;
                    *bestCol = j;
											
										 r_data =i; //将行列数据更新
									   c_data =j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

// 电脑根据当前棋盘状态进行下棋（AI 落子）
// 参数 ai 表示电脑（设备）的棋子字符，human 表示人的棋子字符
void aiMove(char board[SIZE][SIZE], char ai, char human) {
    int row = -1, col = -1;
    // 当棋盘空时，可直接将电脑第一步放到中间位置，实现 “第一步可设置”
    int isEmpty = 1;
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) {
                isEmpty = 0;
                break;
            }
        }
        if (!isEmpty)
            break;
    }
    if (isEmpty) {
        row = 1;
        col = 1;
    } else {
        bestMove(board, &row, &col, ai, human);
    }
    if (row != -1 && col != -1) {
        makeMove(board, row, col, ai);
    }
}

// 辅助函数：检查棋盘胜者，若存在则返回获胜棋子的字符，否则返回空格 ' '
// 注意：平局时此函数返回 ' '，游戏结束判断需要结合 isMovesLeft() 实现
char getWinner(char board[SIZE][SIZE]) {
    int i;
    // 检查行
    for (i = 0; i < SIZE; i++) {
        if (board[i][0] != 0 &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];
    }
    // 检查列
    for (i = 0; i < SIZE; i++) {
        if (board[0][i] != 0 &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }
    // 检查对角线
    if (board[0][0] != 0 &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != 0 &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];
    
    return ' ';
}

//将二维数组的值转换为棋盘的数值
uint8_t chess_data_transfor(uint8_t row_data ,uint8_t col_data)
{
	if(row_data == 0 &&col_data ==0) { return 9;}
	if(row_data == 0 &&col_data ==1) { return 8;}
	if(row_data == 0 &&col_data ==2) { return 7;}
	if(row_data == 1 &&col_data ==0) { return 6;}
	if(row_data == 1 &&col_data ==1) { return 5;}
	if(row_data == 1 &&col_data ==2) { return 4;}
	if(row_data == 2 &&col_data ==0) { return 3;}
	if(row_data == 2 &&col_data ==1) { return 2;}
	if(row_data == 2 &&col_data ==2) { return 1;}
}


void chess_show()
{
	OLED_ShowNum(1,1,board[2][2],1,12,0);
	OLED_ShowChar(20,1,'|',12,0);
	OLED_ShowNum(40,1,board[2][1],1,12,0);
	OLED_ShowChar(60,1,'|',12,0);
	OLED_ShowNum(80,1,board[2][0],1,12,0);
	OLED_ShowChar(100,1,'|',12,0);
	
	OLED_ShowNum(1,2,board[1][2],1,12,0);
	OLED_ShowChar(20,2,'|',12,0);
	OLED_ShowNum(40,2,board[1][1],1,12,0);
	OLED_ShowChar(60,2,'|',12,0);
	OLED_ShowNum(80,2,board[1][0],1,12,0);
	OLED_ShowChar(100,2,'|',12,0);
	
	OLED_ShowNum(1,3,board[0][2],1,12,0);
	OLED_ShowChar(20,3,'|',12,0);
	OLED_ShowNum(40,3,board[0][1],1,12,0);
	OLED_ShowChar(60,3,'|',12,0);
	OLED_ShowNum(80,3,board[0][0],1,12,0);
	OLED_ShowChar(100,3,'|',12,0);	
	OLED_ShowString(1,5,"time:",12,0);
	OLED_ShowNum(30,5,time_flag,2,12,0);
	
}

void now_board_data()
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		now_board[0][i] =data[i+1];
	}
	for(i=0;i<3;i++)
	{
		now_board[1][i] =data[i+4];
	
	}
	for(i=0;i<3;i++)
	{
		now_board[2][i] =data[i+7];
	}
}

void chess_error()
{
	uint8_t i,j;

	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			if(now_board[i][j]!=board[i][j])
			{
				
				if(now_board[i][j] == 0)
				{
					row_error_null =i;
					col_error_null =j;
				}
				if(now_board[i][j] == 2)
				{
					chess_set_error =1;
					row_error_chess =i;
					col_error_chess =j;
				}
			}
		}
	}
}
