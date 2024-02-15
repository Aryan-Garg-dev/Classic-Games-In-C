#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#include <ctype.h>

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)
#define and &&
#define or ||
#define true 1
#define false 0

typedef struct Move{
    int row, col;
}Move;

const int WINNING_COMBINATIONS[8][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6},
};

char board[3][3]; 
const char player = 'X';
const char computer = 'O';
int gameOn = false;
int score = 0; //* +10 if user win -10 if user lose
int playerTurn = true; //* to rotate turns
const int ESC = 27;
const int ENTER = 13;
const int SPACE = 32;

//draw functions
void putPieces(int row, int col){
    char boardLocation = board[row][col];
    if (boardLocation==computer){
        printf("\033[1;31m%c\033[0m", boardLocation);
    } else if (boardLocation==player) {
        printf("\033[1;33m%c\033[0m", boardLocation);
    } else {
        int pieceLocation = 3*row+col;
        printf("%c", (char)pieceLocation+48);
    }
}

void drawBoard(){
    printf("Score: %d\n", score);
    printf("#-----------#\n");
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            printf("| ");
            putPieces(i, j);
            printf(" ");
        }
        printf("|\n");
        if (i!=2){
            printf("|---+---+---|\n");
        }
    }
    printf("#-----------#\n");
}


//setUpBoard()
void setUpBoard(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            board[i][j] = (char)(3*i+j + 48);
        }
    }
}

//startGame()
void startGame(){
    printf("\033[1;32mWelcome to the classic tic-tac-toe game\033[0m\n\n");
    printf("\033[1;34m");
    drawBoard();
    printf("\033[0m\n");
    printf("Press ENTER to start game\n");
    char ch;
    if (kbhit){
        ch = getch();
        if ((int)ch==ENTER){
            gameOn = true;
            setUpBoard();
            system("cls||clear");
            drawBoard();
        }
    }
}

//resetGame
void resetGame(){
    gameOn = true;
    playerTurn = true;
    setUpBoard();
}


void gameMenu(){
    printf("\n");
    printf("Enter \033[0;34mESC\033[0m key again to exit the game\n");
    printf("Enter \033[0;34mSPACE\033[0m key to reset the game\n");
    char ch;
    if (kbhit){
        ch = getch();
        if ((int)ch==SPACE){
            system("cls||clear");
            resetGame();
            drawBoard();
        } else if ((int)ch==ESC){
            gameOn = false;
            exit(0);
        }
    }
}

//isMovesLeft()
int isMovesLeft(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (isdigit((int)board[i][j])){
                return true;
            }
        }
    }
    return false;
}

//checkWin()
int checkWin(char piece){
    int checkTurns = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j]==piece){
                checkTurns++;
            }
        }
    }
    int checkMoves[checkTurns];
    int index=0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j]==piece){
                checkMoves[index] = 3*i+j;
                index++;
            }
        }
    }
    for (int i = 0; i < 8; i++){
        int hasWon = 1;
        for (int j = 0; j < 3; j++){
            int match = 0;
            for (int k = 0; k < checkTurns; k++){
                if (WINNING_COMBINATIONS[i][j] == checkMoves[k]){
                    match = 1;
                    break;
                }
            }
            if (!match){
                hasWon = 0;
                break;
            }
        }
        if (hasWon){
            return true;
            break;
        }
    }
    return false;
}

int evaluate(){
    if (checkWin(player)){
        return +10;
    } else if (checkWin(computer)){
        return -10;
    } else {
        return 0;
    }
}

//minimax() // player->Maximiser computer->Minimiser
int minimax(int depth, int isPlayer){
    int score = evaluate();
    if (score == 10 || score == -10){
        return score;
    }
    if (!isMovesLeft()){
        return 0;
    }
    if (isPlayer){
        int max = INT_MIN;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                char boardChar = board[i][j];
                if (isdigit(boardChar)){
                    board[i][j]=player;
                    max = MAX(max, minimax(depth+1, !isPlayer));
                    board[i][j]=boardChar; 
                }
            }
        }
        return max;
    } else {
        int min = INT_MAX;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                char boardChar = board[i][j];
                if (isdigit(boardChar)){
                    board[i][j]=computer;
                    min = MIN(min, minimax(depth+1, !isPlayer));
                    board[i][j]=boardChar;
                }
            }
        }
        return min;
    }
}

//getTheBestMove() - gets the best possible move for computer;
int getTheBestMove(){
    int best = -1;
    int min = INT_MAX;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            char boardChar = board[i][j];
            if (isdigit(boardChar)){
                board[i][j]=computer;
                int val = minimax(0, true);
                board[i][j]=boardChar;
                if (val < min){
                    best = 3*i+j;
                    min = val;
                }
            }
        }
    }
    return best;
}

//updateBoard()
void updateBoard(int move, char piece){
    int row = move / 3;
    int col = move % 3;
    board[row][col]=piece;
}

char inputKeyPress(){
    char ch;
    if (kbhit){
        ch = getch();
    }
    return ch;
}

void main(){
    system("cls||clear");
    startGame();
    while(gameOn){
        if (playerTurn){
            int illegal = true;
            while (illegal){
                printf("\033[0;32mPress ESC to invoke game menu\033[0m\n");
                printf("Enter move: \n");
                int input = (int) inputKeyPress();
                int move = input-48;
                if (input == ESC){
                    gameMenu();
                    continue;
                    illegal = false;
                } else if (!isdigit(input) || !isdigit(board[move/3][move%3])){
                    printf("\033[0;36mIllegal Move\033[0m\n");
                } else {
                    updateBoard(move, player);
                    illegal = false;
                }
            }
        } else {
            int move = getTheBestMove();
            updateBoard(move, computer);
        }
        char piece = (playerTurn)?player:computer;
        system("cls||clear");
        if (checkWin(piece)){
            if (playerTurn){
                score += 10;
                printf("\033[1;33mYou Won\033[0m\n\n");
            } else {
                score -= 10;
                printf("\033[1;31mYou Lose\033[0m\n\n");
            }
            drawBoard();
            gameMenu();
        } else {
            if (isMovesLeft()){
                playerTurn = !playerTurn;   
                drawBoard(); 
            } else {
                printf("\033[1;32mIt's a Draw\033[0m\n\n");
                drawBoard();
                gameMenu();
            }
        }
    }
    
}
