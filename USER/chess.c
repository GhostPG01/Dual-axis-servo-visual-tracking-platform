
#include <stdio.h>
#include <stdlib.h>
#include "chess.h"
#include "OLED.h"
//row������ ��col������
uint8_t r_data=0,c_data =0;
uint8_t row_error_null=0,col_error_null=0,row_error_chess,col_error_chess;
uint8_t chess_set_error = 0;
extern uint8_t board[3][3],data[10];
extern uint8_t time_flag;
uint8_t now_board[3][3];
//2�ǰ��� 1�Ǻ���

// ��ʼ�����̣������е�Ԫ����Ϊ�ո� ' '
void initBoard(char board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

// ��ӡ���̣����ڵ��Ի���ʾ��ǰ���״̬
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

// ������ָ��λ�����ӣ�����λ��Ϊ����������Ӳ����� 1�����򷵻� 0
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

// �ж��������Ƿ񻹴��ڿ�λ�������ڷ��� 1�����򷵻� 0������������
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

// ������ǰ����״̬
// �� ai�����ԣ�Ӯ�򷵻� +10���� human Ӯ�򷵻� -10�����򷵻� 0
int evaluate(char board[SIZE][SIZE], char ai, char human) {
    int i;
    // �����
    for (i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == ai)
                return 10;
            else if (board[i][0] == human)
                return -10;
        }
    }
    // �����
    for (i = 0; i < SIZE; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == ai)
                return 10;
            else if (board[0][i] == human)
                return -10;
        }
    }
    // ��������Խ���
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

// ʹ�� Minimax �㷨������ѷ���
// ���� depth ���Լ��ٻ�ʤ������Ӱ�죨�ᳫ��Ӯ�����䣩
// ���� isMax ָʾ��ǰ�������������󻯻�����С��
int minimax(char board[SIZE][SIZE], int depth, int isMax, char ai, char human) {
    int score = evaluate(board, ai, human);
    
    // ���վ��򷵻ط�������ȥ depth ��ʹ��Խ���ʤ�÷ָ���
    if (score == 10)
        return score - depth;
    if (score == -10)
        return score + depth;
    
    // �������޿�λ����Ϊƽ��
    if (!isMovesLeft(board))
        return 0;
    
    int best;
    int i, j;
    if (isMax) {
        best = -1000;
        // �������п�λ��ģ�� ai ����
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = ai;
                    int value = minimax(board, depth + 1, 0, ai, human);
                    if (value > best)
                        best = value;
                    board[i][j] = 0; // ��������
                }
            }
        }
    } else {
        best = 1000;
        // �������п�λ��ģ�� human ����
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = human;
                    int value = minimax(board, depth + 1, 1, ai, human);
                    if (value < best)
                        best = value;
                    board[i][j] = 0; // ��������
                }
            }
        }
    }
    return best;
}

// ���� Minimax �㷨Ѱ���������λ�ã���ͨ�� bestRow �� bestCol ���ظ�λ��
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
                board[i][j] = 0; // �ָ���λ
                if (moveVal > bestVal) {
                    *bestRow = i;
                    *bestCol = j;
											
										 r_data =i; //���������ݸ���
									   c_data =j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

// ���Ը��ݵ�ǰ����״̬�������壨AI ���ӣ�
// ���� ai ��ʾ���ԣ��豸���������ַ���human ��ʾ�˵������ַ�
void aiMove(char board[SIZE][SIZE], char ai, char human) {
    int row = -1, col = -1;
    // �����̿�ʱ����ֱ�ӽ����Ե�һ���ŵ��м�λ�ã�ʵ�� ����һ�������á�
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

// �����������������ʤ�ߣ��������򷵻ػ�ʤ���ӵ��ַ������򷵻ؿո� ' '
// ע�⣺ƽ��ʱ�˺������� ' '����Ϸ�����ж���Ҫ��� isMovesLeft() ʵ��
char getWinner(char board[SIZE][SIZE]) {
    int i;
    // �����
    for (i = 0; i < SIZE; i++) {
        if (board[i][0] != 0 &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];
    }
    // �����
    for (i = 0; i < SIZE; i++) {
        if (board[0][i] != 0 &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }
    // ���Խ���
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

//����ά�����ֵת��Ϊ���̵���ֵ
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
