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
#define NEXT_LEFT_IS_OPEN_FROM_B(board, row, col) ((board[(row- 'A')][(col - '0') - 1] != B_PLAYER) ? true : false)
#define NEXT_RIGHT_IS_OPEN_FROM_B(board, row, col) ((board[(row - 'A')][(col - '0')] != B_PLAYER) ? true : false)
#define NEXT_LEFT_IS_OPEN_FROM_T(board, row, col) ((board[(row- 'A')][(col - '0') - 1] != T_PLAYER) ? true : false)
#define NEXT_RIGHT_IS_OPEN_FROM_T(board, row, col) ((board[(row- 'A')][(col - '0')] != T_PLAYER) ? true : false)

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


SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);
void checkAlloTree(SingleSourceMovesTree* tr);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(SingleSourceMovesTreeNode* source, Board board, char row, char col); // if the player is T
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(SingleSourceMovesTreeNode* source, Board board, char row, char col); // if the player is B
SingleSourceMovesTreeNode* createSingleSourceMovesTreeNode(Board board, char row, char col,SingleSourceMovesTreeNode* next_move);
bool thePosIsOutOfTheBoard(char row,char col);
bool thePosIsOnTPlayer(Board board,char row,char col);
bool thePosIsOnBPlayer(Board board, char row, char col);
void printfWaysOfMovments(SingleSourceMovesTree* tr);
void printfWaysOfMovmentsHelper(SingleSourceMovesTreeNode* source);
int main()
{
    Player T;
    Player B;
    SingleSourceMovesTree* tr;
    Board board;
    board[0][1] = T;
//    board[0][3] = T;
//    board[0][5] = T;
//    board[0][7] = T;
//    board[7][1] = B;
//    board[7][3] = B;
//    board[7][5] = B;
//    board[7][7] = B;

    tr = FindSingleSourceMoves(board, &board[0][1]); // the parameters that we send here is not good
    printfWaysOfMovments(tr);
   return 0;
}

void printfWaysOfMovments(SingleSourceMovesTree* tr)
{
    printfWaysOfMovmentsHelper(tr->source);
}

void printfWaysOfMovmentsHelper(SingleSourceMovesTreeNode* source) // DLR print
{
    if (source == NULL)
    {
        printf("\n");
        return;
    }
    printf("%c%c_", source->pos->row, source->pos->col);
    printfWaysOfMovmentsHelper(source->next_move[0]);
    printfWaysOfMovmentsHelper(source->next_move[1]);
}



SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{
    SingleSourceMovesTree* tr = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
    checkAlloTree(tr);

    if ((board[src->row][src->col] != T_PLAYER) && (board[src->row][src->col] != B_PLAYER)) // if the player on the board doesnt exist
        return NULL;

    if (thePosIsOnTPlayer(board,src->row,src->col) == true)
    {
       tr->source = FindSingleSourceMovesHelperT(tr->source,board, src->row,src->col); // if the player is T
    }
    else // if the the player is B
    {
        tr->source = FindSingleSourceMovesHelperB(tr->source,board, src->row,src->col);
    }
    return tr;
}

SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(SingleSourceMovesTreeNode* source, Board board, char row, char col) // T player
{
    SingleSourceMovesTreeNode *left, * right;

    if (thePosIsOutOfTheBoard(row,col) == true) // checks if the postion of the player is on the board
        return NULL;

    else if (thePosIsOnTPlayer(board,row,col) == true) // checks if the position of player T is on his friend
        return NULL;

    else if ((thePosIsOnBPlayer(board,row +1 ,col - 1) == true) || (thePosIsOnBPlayer(board,row +1 ,col + 1) == true)) // checks if the T player has player B in the next movment
    {
        source = createSingleSourceMovesTreeNode(board, row,col,NULL);
        if(NEXT_LEFT_IS_OPEN_FROM_B(board,(row + 1),(col - 1)) == true)
            left = FindSingleSourceMovesHelperT(source->next_move[0], board, row + 2, col - 2);
        if(NEXT_RIGHT_IS_OPEN_FROM_B(board,(row + 1),(col + 1)) == true)
            right = FindSingleSourceMovesHelperT(source->next_move[1], board, row + 1, col + 2);
    }
    else // if the next movment is open
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


SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(SingleSourceMovesTreeNode* source, Board board, char row, char col) // B player
{
    SingleSourceMovesTreeNode *left, * right;

    if (thePosIsOutOfTheBoard(row,col) == true) // checks if the postion of the player is on the board
        return NULL;

    else if (thePosIsOnBPlayer(board,row,col) == true) // checks if the position of player B is on his friend
        return NULL;

    else if((thePosIsOnTPlayer(board,row - 1,col + 1) == true) || (thePosIsOnTPlayer(board,row - 1,col + 1) == true)) // checks if the B player has player T in the next movment
    {
        source = createSingleSourceMovesTreeNode(board, row,col,NULL);
        if(NEXT_LEFT_IS_OPEN_FROM_T(board,(row - 1),(col - 1)) == true)
            left = FindSingleSourceMovesHelperT(source->next_move[0], board, row - 2, col - 2);
        if(NEXT_RIGHT_IS_OPEN_FROM_T(board,(row - 1),(col + 1)) == true)
            right = FindSingleSourceMovesHelperT(source->next_move[1], board, row - 2, col + 2);
    }
    else // if the next movment is open
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
