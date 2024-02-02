#include <stdio.h>
#include <stdlib.h>

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
const char circle = 'O';
const char cross = 'X';
int circleTurn = 0; //0 -> X's turn, 1 -> O's Turn 
int gameOn = 1;
char board[3][3];

void setUpBoard(char matrix[3][3]){
    int counter = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            matrix[i][j] = (char) counter + 48;
            counter++;
        }
    }
}

void echo(char matrix[3][3]){
    for (int j = 0; j < 3; j++){
        printf("[%c", matrix[j][0]);
        for (int i = 1; i < 3; i++){
            printf(", %c", matrix[j][i]);
        }
        printf("]\n");
    }
    printf("\n");
}

void checkWin(){
    char check = circleTurn ? circle : cross;
    int checkTurns = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j]==check){
                checkTurns++;
            }
        }
    }
    int checkMoves[checkTurns];
    int counter=0, index=0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j]==check){
                checkMoves[index] = counter;
                index++;
            }
            counter++;
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
            printf("%c wins\n\n", check);
            gameOn = 0;
            break;
        }
    }
}

void updateBoard(int move){
    int r = move / 3; 
    int c = move % 3;
    if (circleTurn){
        board[r][c] = circle;
    } else {
        board[r][c] = cross;
    }
}

int main(){
    system("cls||clear");
    int move;
    int count = 0;
    setUpBoard(board);
    echo(board);
    while (gameOn){
        if (circleTurn){
            printf("O: ");
        } else {
            printf("X: ");
        }
        scanf("%d", &move);
        updateBoard(move);
        echo(board);
        count++;
        system("cls||clear");
        checkWin();
        if (count == 9 && gameOn){
            printf("There is a DRAW!\n\n");
            gameOn = 0;
        }
        echo(board);
        circleTurn = (circleTurn)? 0: 1;
    }
    return 0;
}
