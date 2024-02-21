#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define boardWidth 16
#define boardHeight 20
#define nextBoardSize 4
#define sleepDuration 100

typedef struct loc{
    int index, nextIndex;
    int pos;
    int rot, nextRot;
}loc;

loc current;
int score = 0;
int gameOn = 1;

char currentPiece[17];
int gameBoard[boardHeight][boardWidth] = {{0}};

char *piece_L[] = {
    "1000"
    "1000"
    "1100"
    "0000", 
    "1110"
    "1000"
    "0000"
    "0000",
    "1100"
    "0100"
    "0100"
    "0000",
    "0010"
    "1110"
    "0000"
    "0000"
};

char *piece_J[] = {
    "0100"
    "0100"
    "1100"
    "0000",
    "1000"
    "1110"
    "0000"
    "0000",
    "1100"
    "1000"
    "1000"
    "0000",
    "1110"
    "0010"
    "0000"
    "0000"
};

char *piece_T[] = {
    "1110"
    "0100"
    "0000"
    "0000",
    "0100"
    "1100"
    "0100"
    "0000",
    "0100"
    "0100"
    "1110"
    "0000",
    "1000"
    "1100"
    "1000"
    "0000"
};

char *piece_O[] = {
    "1100"
    "1100"
    "0000"
    "0000"
};

char *piece_I[]  = {
    "1000"
    "1000"
    "1000"
    "1000",
    "1111"
    "0000"
    "0000"
    "0000"
};

char *piece_S[] = {
    "1100"
    "0110"
    "0000"
    "0000", 
    "0100"
    "1100"
    "1000"
    "0000"
};

char *piece_Z[] = {
    "0110"
    "1100"
    "0000"
    "0000",
    "1000"
    "1100"
    "0100"
    "0000"
};

char **PIECES[] = {piece_I, piece_J, piece_L, piece_O, piece_S, piece_T, piece_Z};
int numRotations[] = {2, 4, 4, 1, 2, 4, 2};

int random(int start, int end){
    return (rand()%(end-start+1))-start;
}

void setCurrentPiece(){
    current.index = random(0, 6);
    current.nextIndex = random(0, 6);
    current.pos = 6;
    current.rot = random(0, numRotations[current.index] - 1);
    current.nextRot = random(0, numRotations[current.nextIndex] - 1);
    strcpy(currentPiece, PIECES[current.index][current.rot]);
}

//TEST
void printCurrentPiece(){
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if (currentPiece[i*4+j]=='1'){
                    printf("[]");
                } else {
                    printf("  ");
                }
            }
        printf("\n");
    }
}

//TEST
void printBoardData(){
    for (int i = 0; i < boardHeight; i++){
        for (int j = 0; j < boardWidth; j++){
            printf("%d ", gameBoard[i][j]);
        }
        printf("\n");
    }
}

void updateBoard(){
    int rows = 4, cols = 4;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (currentPiece[i*4+j]=='1'){
                int x = current.pos % boardWidth + j;
                int y = current.pos / boardWidth + i;
                gameBoard[y][x] = current.index+1;
            }
        }
    }
}


void printPieceChar(int color){
    switch(color){
        case 1:
            printf("\033[1;34m[]\033[0m");
            break;
        case 2:
            printf("\033[1;31m[]\033[0m");
            break;
        case 3: 
            printf("\033[1;32m[]\033[0m");
            break;
        case 4:
            printf("\033[1;33m[]\033[0m");
            break;
        case 5:
            printf("\033[1;35m[]\033[0m");
            break;
        case 6:
            printf("\033[1;36m[]\033[0m");
            break;
        case 7:
            printf("\033[1;37m[]\033[0m");
            break;
        default: 
            printf("  ");
            break;
    }
}

void drawNextPieceScreen(){
    for (int i = 0; i < 10; i++){
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < 4; i++){
         printf("|");
        for (int j = 0; j < 4; j++){
          if (PIECES[current.nextIndex][current.nextRot][4*i+j]=='1'){
                    printPieceChar(current.nextIndex+1);
                } else {
                    printf("  ");
                }
        }
        printf("|\n");
    }
    for (int i = 0; i < 10; i++){
        printf("-");
    }
    printf("   Score: %d\n", score);
}

void draw(){
    Sleep(sleepDuration);
    system("cls||clear");
    updateBoard();
    drawNextPieceScreen();
    printf("  ");
    for (int n = 0; n < boardWidth*2; n++){
        printf("^");
    }
    printf("\n");
    for (int i = 0; i < boardHeight; i++){
        printf("<!");
        for (int j = 0; j < boardWidth; j++){
            printPieceChar(gameBoard[i][j]);
        }
        printf("!>\n");
    }
    printf("  ");
    for (int n = 0; n < boardWidth*2; n++){
        printf("^");
    }
    printf("\n");
}

void deleteOldConfigurations(){
    int rows = 4, cols = 4;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (currentPiece[i*4+j]=='1'){
                int x = current.pos % boardWidth + j;
                int y = current.pos / boardWidth + i;
                gameBoard[y][x] = 0;
            }
        }
    }
}

void addScore(){
    for (int i = 0; i < boardHeight; i++){
        int isRowFill = 1;
        for (int j = 0; j < boardWidth; j++){
            if (gameBoard[i][j]==0){
                isRowFill = 0;
            }
        }
        if (isRowFill){
            score += 10;
            for (int n = i; n > 0; n--){
                for (int j = 0; j < boardWidth; j++){
                    gameBoard[n][j] = gameBoard[n-1][j];
                }
            }
            for (int n = 0; n < boardWidth; n++){
                gameBoard[0][n] = 0;
            }
        }
    }
}

void gameOver(){
    int boxInEachRowFilled = 1;
    for (int i = 0; i < boardHeight; i++){
        int boxInRowFilled = 0;
        for (int j = 0; j < boardWidth; j++){
            if (gameBoard[i][j]!=0){
                boxInRowFilled = 1;
            }
        }
        if (!boxInRowFilled){
            boxInEachRowFilled = 0;
        }
    }
    if (boxInEachRowFilled){
        gameOn = 0;;
        printf("\033[1;34mGame Over\033[0m");
    }
}

void stopAtBottom(){
    int hasReachedTheBottom = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (currentPiece[i*4+j]=='1'){
                int x = current.pos % boardWidth + j;;
                int y = current.pos / boardWidth + i;
                if (y == boardHeight-1 || (gameBoard[y+1][x]!=0 && currentPiece[4*(i+1)+j]!='1')){
                    hasReachedTheBottom = 1;
                    break;
                }
            }
        }
    }
    if (hasReachedTheBottom){
        current.pos = 6;
        current.index = current.nextIndex;
        current.nextIndex = random(0, 6);
        current.rot = current.nextRot;
        current.nextRot = random(0, numRotations[current.nextIndex] - 1);
        strcpy(currentPiece, PIECES[current.index][current.rot]);
        draw();
        addScore();
        gameOver();
    }
}

void moveDown(){
    deleteOldConfigurations();
    current.pos+=boardWidth;
    draw();
    stopAtBottom();
}

void moveRight(){
    deleteOldConfigurations();
    current.pos += 1;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (currentPiece[i*4+j]=='1'){
                int x = current.pos % boardWidth + j;
                int y = current.pos / boardWidth + i;
                if (x == boardWidth || gameBoard[y][x]!=0){
                    current.pos -= 1;
                }
            } 
        }
    }
    draw();
}

void rotate(){
    deleteOldConfigurations();
    current.rot = (current.rot + 1) % numRotations[current.index];
    strcpy(currentPiece, PIECES[current.index][current.rot]);
    draw();
}

void moveLeft(){
    deleteOldConfigurations();
    current.pos -= 1;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (currentPiece[i*4+j]=='1'){
                int x = current.pos % boardWidth + j;
                int y = current.pos / boardWidth + i;
                if (x == boardWidth-1 || gameBoard[y][x]!=0){
                    current.pos += 1;
                }
            } 
        }
    }
    draw();
}

void control(){
    if (kbhit()){
        switch(getch()){
            case 'W':
            case 'w':
                rotate();
                break;
            case 's':
            case 'S':
                moveDown();
                break;
            case 'A':
            case 'a':
                moveLeft();
                break;
            case 'D':
            case 'd':
                moveRight();
                break;
        }
    }
}

void main(){
    srand(time(NULL));
    setCurrentPiece();
    draw();
    while(gameOn){
        control();
        moveDown();
    }
}
