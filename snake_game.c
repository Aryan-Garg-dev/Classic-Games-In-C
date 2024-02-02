#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

int posX, posY;
int snake[1200][2]={{24, 9}};
int snakeLength = 1;
int width = 60, height = 20;
char row[61];
int gameOver = 0;
int score = 0;
int fruitX = 0, fruitY = 0;
int direction = 0; //1 = top, 2 = right, -1 = bottom, -2 = left
int gameSpeed = 250;

int isSnake(int coordinates[2], int snakeBody[1200][2], int snakeLength, int startIndex) {
	for(int i = startIndex; i < snakeLength; i++) {
        if(snakeBody[i][0] == coordinates[0] && snakeBody[i][1] == coordinates[1]) {
            return 1;
		}
    }
    return 0;
}

void moveCell(int snake[1200][2], int pos, int dir) {
	int moveBy[2] = {0, 0};
	switch (dir) {
		case 1:
			moveBy[0] = 0;
			moveBy[1] = -1;
			break;
		case 2:
			moveBy[0] = 1;
			moveBy[1] = 0;
			break;
		case -1:
			moveBy[0] = 0;
			moveBy[1] = 1;
			break;
		case -2:
			moveBy[0] = -1;
			moveBy[1] = 0;
			break;
	}
	snake[pos][0] += moveBy[0];
	snake[pos][1] += moveBy[1];
}

void moveSnake() {
	for(int i = snakeLength; i >= 1; i--) {
		snake[i][0] = snake[i-1][0];
		snake[i][1] = snake[i-1][1];
	}
	moveCell(snake, 0, direction);
	if(isSnake(snake[0], snake, snakeLength, 1)) {
		gameOver = 1;
	}
}

void draw() {
	system("cls||clear");
	row[60] = '\0';
    printf("-------------Welcome To The Classic Snake Game--------------\n");
	for(posY = 0; posY < height; posY++) {
		int i = 0;
		for(posX = 0; posX < width; posX++) {
			int coord[2] = {posX, posY};
			if(isSnake(coord, snake, snakeLength, 0)) {
				row[i] = '@';
			} else if(posX == fruitX && posY == fruitY) {
				row[i] = '0';
			} else if((posX == 0 || posX == width-1) && (posY == 0 || posY == height-1)) {
				row[i] = '+';
			} else if((posX == 0 || posX == width-1) && (posY != 0 || posY != height-1)) {
				row[i] = '|';
			} else if((posX != 0 || posX != width-1) && (posY == 0 || posY == height-1)) {
				row[i] = '_';
			} else {
				row[i] = ' ';
			}
			i++;
		}
		printf("%s\n", row);
	}
	if(!gameOver) {
        printf("@->Snake  0->Food\n");
		printf("Score: %d\n", score);
		printf("Press W, A, S, or D to move the snake\n");
		printf("Press X to quit the game");
	}
}

void spawnFruit() {
    srand(time(0));
	fruitX = rand()%(width-3) + 2;
	fruitY = rand()%(height-3) + 2;
}

void startGame(){
    system("cls||clear");
    gameOver = 0;
    score = 0;
    spawnFruit();
}

void input() {
    if (kbhit()) {
		int newDirection = direction;
        switch (getch()) {
			case 'w':
			case 'W':
				newDirection = 1;
				break;
			case 'a':
			case 'A':
				newDirection = -2;
				break;
			case 's':
			case 'S':
				newDirection = -1;
				break;
			case 'd':
			case 'D':
				newDirection = 2;
				break;
			case 'x':
			case 'X':
				gameOver = 1;
				break;
        }
		if(newDirection != -direction || snakeLength == 1) {
			direction = newDirection;
		}
    }
}

void logic() {
	Sleep(gameSpeed);
	input();
	if(snake[0][0] == fruitX && snake[0][1] == fruitY) {
		snake[snakeLength][0] = snake[snakeLength-1][0];
		snake[snakeLength][1] = snake[snakeLength-1][1];
		moveCell(snake, snakeLength, -direction);
		score += 1;
		snakeLength += 1;
		while(snake[0][0] == fruitX && snake[0][1] == fruitY) {
			spawnFruit();
		}
        if (gameSpeed > 150){
            gameSpeed -= 10;
        } else if (gameSpeed > 100){
            gameSpeed -= 5;
        }
	}
	moveSnake();
	if(snake[0][0] == 0 || snake[0][0] == width-1 || snake[0][1] == 0 || snake[0][1] == height-1) {
		gameOver = 1;
	} else {
		draw();
	}
}

int main() {
    startGame();
	while(!gameOver) {
		logic();
	}
	draw();
	printf("Game Over!\n");
	printf("Final score: %d", score);

	return 0;
}

