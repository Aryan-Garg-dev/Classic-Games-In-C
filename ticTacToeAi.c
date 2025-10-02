#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)
#define and &&
#define or ||
#define is ==
#define true 1
#define false 0

const int WINNING_COMBINATIONS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, //Row
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, //Column
    {0, 4, 8}, {2, 4, 6} //Diagonal
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
int difficulty = 3; // easy -> 1, medium -> 2, hard -> 3

//draw functions
void putPieces(int row, int col){
    char boardChar = board[row][col];
    if (boardChar==computer){
        printf("\033[1;31m%c\033[0m", boardChar);
    } else if (boardChar==player) {
        printf("\033[1;33m%c\033[0m", boardChar);
    } else {
        int pieceLocation = 3*row+col;
        printf("%c", (char)pieceLocation+48);
    }
}

void drawBoard(){
    printf("\033[0;34mScore:\033[0m %d\n", score);
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

//resetGame
void resetGame(){
    gameOn = true;
    playerTurn = true;
    setUpBoard();
}

void gameMenu(){
    printf("\n");
    printf("Press \033[0;34mESC\033[0m key to exit the game\n");
    printf("Press \033[0;34mSPACE\033[0m key to start new game\n");
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
        } else {
            printf("Invalid Key Pressed. Try Again...!\n");
            gameMenu();
        }
    }
}

//startGame()
void startGame(){
    start_game:
    printf("\033[1;32mWelcome to the classic tic-tac-toe game\033[0m\n\n");
    printf("\033[1;34m");
    drawBoard();
    printf("\033[0m\n");

    
    printf("Select Difficulty \n\033[0;34m1.\033[0m Easy \n\033[0;34m2.\033[0m Medium \n\033[0;34m3.\033[0m Hard \033[0;34m<DEFAULT>\033[0m \nEnter Choice (1-3): ");
    scanf("%d", &difficulty);
    if (difficulty < 0 || difficulty > 3) {
        system("cls||clear");
        goto start_game;
    }

    
    printf("\nPress \033[0;34mENTER\033[0m to start game\n");
    char ch;
    if (kbhit){
        ch = getch();
        if ((int)ch==ENTER){
            gameOn = true;
            setUpBoard();
            system("cls||clear");
            drawBoard();
        } else gameMenu();
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
    if (score == 10 or score == -10){
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

int evaluate_with_heuristics(){
    if (checkWin(player)) return +10;
    if (checkWin(computer)) return -10;

    int score = 0;

    for (int i = 0; i < 8; i++) {
        int playerCount = 0, computerCount = 0;

        for (int j = 0; j < 3; j++) {
            int r = WINNING_COMBINATIONS[i][j] / 3;
            int c = WINNING_COMBINATIONS[i][j] % 3;

            if (board[r][c] == player) playerCount++;
            else if (board[r][c] == computer) computerCount++;
        }

        // Only one side can occupy a line
        if (playerCount > 0 && computerCount == 0) {
            // The more marks in a line, the stronger the score
            if (playerCount == 1) score += 1;
            else if (playerCount == 2) score += 5;
        } else if (computerCount > 0 && playerCount == 0) {
            if (computerCount == 1) score -= 1;
            else if (computerCount == 2) score -= 5;
        }
    }

    return score;
}

int minimax_optmised(int depth, int alpha, int beta, int isPlayer, int maxDepth){
    int score = evaluate_with_heuristics();
    if (score == 10 || score == -10) return score;
    if (!isMovesLeft()) return 0;
    if (depth >= maxDepth) return score;
    if (isPlayer){
        int max = INT_MIN;
        for (int i = 0; i < 3 && beta > alpha; i++){
            for (int j = 0; j < 3 && beta > alpha; j++){
                char boardChar = board[i][j];
                if (isdigit(boardChar)){
                    board[i][j] = player;
                    int eval = minimax_optmised(depth + 1, alpha, beta, !isPlayer, maxDepth);
                    max = MAX(max, eval);
                    alpha = MAX(alpha, eval);
                    board[i][j] = boardChar;
                }
            }
        }
        return max;
    } else {
        int min = INT_MAX;
        for (int i = 0; i < 3 && beta > alpha; i++){
            for (int j = 0; j < 3 && beta > alpha; j++){
                char boardChar = board[i][j];
                if (isdigit(boardChar)){
                    board[i][j] = computer;
                    int eval = minimax_optmised(depth + 1, alpha, beta, !isPlayer, maxDepth);
                    min = MIN(min, eval);
                    beta = MIN(beta, eval);
                    board[i][j] = boardChar;
                }
            }
        }
        return min;
    }
}

int getMaxDepth(){
    int maxDepth;
    if (difficulty == 1) maxDepth = 4;  // Easy
    else if (difficulty == 2) maxDepth = 6;  // Medium
    else maxDepth = 9;  // Hard (perfect play)
    return maxDepth;
}

int getMove(){
    int best = -1;
    int min = INT_MAX;

    int maxDepth = getMaxDepth();

    int moves[9];
    int moveCount = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (isdigit(board[i][j])){
                moves[moveCount++] = 3*i + j;
            }
        }
    }

    srand(time(NULL));
    int r = rand() % 100; // 0-99

    if (difficulty == 1){ 
        if (r < 70) return moves[rand() % moveCount];
        // 30% chance of picking best move
        
    } else if (difficulty == 2){ 
        if (r < 30) return moves[rand() % moveCount];
        // 70% changce of picking best move
    }

    for (int i = 0; i < moveCount; i++){
        int r = moves[i] / 3;
        int c = moves[i] % 3;
        board[r][c] = computer;
        int val = minimax_optmised(0, INT_MIN, INT_MAX, true, maxDepth);
        board[r][c] = moves[i] + '0';
        if (val < min){
            min = val;
            best = moves[i];
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
                } else if (!isdigit(input) or !isdigit(board[move/3][move%3])){
                    printf("\033[0;36mIllegal Move\033[0m\n");
                } else {
                    updateBoard(move, player);
                    illegal = false;
                }
            }
        } else {
            time_t timeTaken, currTime; //* For uniform response time 
            time(&timeTaken);
            int move = getMove();
            time(&currTime);
            if (difftime(currTime, timeTaken)<0.5){
                Sleep(500-difftime(currTime, timeTaken)*1000); //* (Better Feedback)
            }
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
