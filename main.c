#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define BOARD_SIZE 8
#define T_PLAYER 'T'
#define B_PLAYER 'B'
#define LEFT 0
#define RIGHT 1
#define FIRST_COL 1
#define INVALID_POS -1
#define NEXT_LEFT_IS_OPEN_FROM_B(board, row, col) ((board[(row- '0') - 17][(col) - 2] != B_PLAYER) ? true : false)
#define NEXT_RIGHT_IS_OPEN_FROM_B(board, row, col) ((board[(row - '0') - 17][(col) + 2] != B_PLAYER) ? true : false)
#define NEXT_LEFT_IS_OPEN_FROM_T(board, row, col) ((board[(row- '0') - 17][(col) - 2] != T_PLAYER) ? true : false)
#define NEXT_RIGHT_IS_OPEN_FROM_T(board, row, col) ((board[(row- '0') - 17][(col) - 2] != T_PLAYER) ? true : false)

typedef struct _checkersPos
{
    char row, col;
}checkersPos;



typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;



typedef struct _SingleSourceMovesTreeNode
{
    Board board;
    checkersPos* pos;
    unsigned short total_capture_so_far;
    struct _SingleSourceMovesTreeNode* next_move[2];

}SingleSourceMovesTreeNode;



typedef struct _SingleSourceMovesTree
{
    SingleSourceMovesTreeNode* source;

}SingleSourceMovesTree;



void checkAlloTree(SingleSourceMovesTree* tr);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(SingleSourceMovesTreeNode* source, Board board, char row, char col); // if the player is T
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(SingleSourceMovesTreeNode* source, Board board, char row, char col); // if the player is B
SingleSourceMovesTreeNode* createSingleSourceMovesTreeNode(Board board, char row, char col,SingleSourceMovesTreeNode* next_move);
bool thePosIsOutOfTheBoard(char row,char col);
bool thePosIsOnTPlayer(Board board,char row,char col);
bool thePosIsOnBPlayer(Board board, char row, char col);


int main()
{
   return 0;
}





SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{
    SingleSourceMovesTree* tr = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
    checkAlloTree(tr);


    if ((board[src->col][src->row] != T_PLAYER) && (board[src->col][src->row] != B_PLAYER))
        return NULL;

    if ((board[(src->col - 'A')][(src->row - '0')] == T_PLAYER))
    {
      tr->source = FindSingleSourceMovesHelperT(tr->source,board, src->row,src->col); //TODO
    }
    else
    {
        tr->source = FindSingleSourceMovesHelperB(tr->source,board, src->row,src->col);
    }
    return tr;
}

SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(SingleSourceMovesTreeNode* source, Board board, char row, char col)
{
    SingleSourceMovesTreeNode *left, * right;

    if (thePosIsOutOfTheBoard(row,col) == true)
        return NULL;

    else if (thePosIsOnTPlayer(board,row,col) == true)
        return NULL;

    else if ((thePosIsOnBPlayer(board,row +1 ,col - 1) == true) || (thePosIsOnBPlayer(board,row +1 ,col + 1) == true))
    {
        source = createSingleSourceMovesTreeNode(board, row,col,NULL);
        if(NEXT_LEFT_IS_OPEN_FROM_B(board,(row + 1),(col - 1)) == true)
            left = FindSingleSourceMovesHelperT(source->next_move[0], board, row + 2, col - 2);
        if(NEXT_RIGHT_IS_OPEN_FROM_B(board,(row + 1),(col + 1)) == true)
            right = FindSingleSourceMovesHelperT(source->next_move[1], board, row + 1, col + 2);
    }
    else
    {
        source = createSingleSourceMovesTreeNode(board,row,col, NULL);
        left = FindSingleSourceMovesHelperT(source->next_move[0], board, row + 1, col - 1);
        right = FindSingleSourceMovesHelperT(source->next_move[1], board, row + 1, col + 1);
    }
    return source;
}

bool thePosIsOutOfTheBoard(char row,char col)
{
    int mRow, mCol;
    mRow = row - 'A';
    mCol = (col - '0') - 1;
    if((mRow > BOARD_SIZE) || (mCol > BOARD_SIZE) || (mCol < FIRST_COL))
        return true;
    else
        return false;
}

bool thePosIsOnTPlayer(Board board,char row,char col)
{
    int mRow, mCol;
    mRow = row - 'A';
    mCol = (col - '0') - 1;
    if (board[mRow][mCol] == T_PLAYER)
        return true;
    else
        return false;
}

bool thePosIsOnBPlayer(Board board,char row,char col)
{
    int mRow, mCol;
    mRow = row - 'A';
    mCol = (col - '0') - 1;
    if (board[mRow][mCol] == B_PLAYER)
        return true;
    else
        return false;
}


SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(SingleSourceMovesTreeNode* source, Board board, char row, char col)
{
    SingleSourceMovesTreeNode *left, * right;

    if ((row > BOARD_SIZE) || (col > BOARD_SIZE) || (col < FIRST_COL))
        return NULL;

    else if (board[(row - 'A') - 1][(col - '0')] == B_PLAYER)
        return NULL;
    else if(board[(row - 'A')][(col - '0')] == T_PLAYER || board[(row - 'A')][(col - '0') - 1] == T_PLAYER)
    {
        source = createSingleSourceMovesTreeNode(board, row,col,NULL);
        if(NEXT_LEFT_IS_OPEN_FROM_T(board,(row - 1),(col - 1)) == true)
            left = FindSingleSourceMovesHelperT(source->next_move[0], board, row + 2, col - 2);
        if(NEXT_RIGHT_IS_OPEN_FROM_T(board,(row - 1),(col + 1)) == true)
            right = FindSingleSourceMovesHelperT(source->next_move[1], board, row + 1, col + 1);
    }
    else
    {
        source = createSingleSourceMovesTreeNode(board, row,col,NULL);
        left = FindSingleSourceMovesHelperB(source->next_move[0], board, row - 1, col - 1);
        right = FindSingleSourceMovesHelperB(source->next_move[1], board, row - 1, col + 1);
    }
    return source;
}



SingleSourceMovesTreeNode* createSingleSourceMovesTreeNode(Board board, char row, char col,SingleSourceMovesTreeNode* next_move)
{
    SingleSourceMovesTreeNode* res;
    res = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
    if (res == NULL)
        exit(1);
    memcpy(res->board, board, (strlen(board) + 1));
    //res->board = board;
    res->pos->row = row;
    res->pos->col = col;
    res->next_move[0] = next_move;
    res->next_move[1] = next_move;
    return res;
}

void checkAlloTree(SingleSourceMovesTree* tr)
{
    if (tr == NULL)
    {
        printf("Memmory allocation failed\n");
        exit(1);
    }
}
