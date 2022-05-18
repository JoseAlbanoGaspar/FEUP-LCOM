#include "snake.h"

int snakeX = 100;
int snakeY = 100;
int snakeTailX = 80;
int snakeTailY = 80;
int direction = 0; //0 right, 1 left, 2 up, 3 down
uint32_t snakeColor =  0x0008F300;
int segments_len = 5;
int segments[2400];
bool addToSnake = false;

void startPosition(){
    segments[0] = 120;
    segments[1] = 120;
    segments[2] = 100;
    segments[3] = 120;
    segments[4] = 80;
    segments[5] = 120;
    segments[6] = 60;
    segments[7] = 120;
    segments[8] = 40;
    segments[9] = 120;
}

void (drawBackground)(){
  /* Draw Rectangle */
  vg_draw_rectangle(segments[2*(segments_len-1)], segments[2*(segments_len-1)+1], 20, 20, 0x000057FF); //0x000057FF

  //printf("0: %d /1: %d /2: %d /3: %d\n", segments[2*(segments_len-1)], segments[2*(segments_len-1)+1], segments[0], segments[1]);
}

void (drawSnake)(){
    for (int i = 0; i < segments_len*2; i += 2){
        vg_draw_rectangle(segments[i], segments[i+1], 20, 20, snakeColor);
    }
    //printf("0: %d /1: %d /2: %d /3: %d\n", segments[0], segments[1], segments[2], segments[3]);
}

bool (canMove)(){
    return true;
}

void (moveSnake)(int dir){
    if (!addToSnake) drawBackground();
    snakeX = segments[0];
    snakeY = segments[1];
    snakeTailX = segments[segments_len*2 -2];
    snakeTailY = segments[segments_len*2 -1];
    moveSegments();
    switch (dir)
    {
    case 0:
        if (snakeX + 20 < 800) snakeX += 20;
        else snakeX = 0;
        break;
    case 1:
        if (snakeX - 20 >= 0) snakeX -= 20;
        else snakeX = 780;
        break;
    case 2:
        if (snakeY - 20 >= 0) snakeY -= 20;
        else snakeY = 580;
        break;
    case 3:
        if (snakeY + 20 < 600) snakeY += 20;
        else snakeY = 0;
        break;
    default:
        break;
    }
    segments[0] = snakeX;
    segments[1] = snakeY;
    if (addToSnake){
        incrementSnake(snakeTailX, snakeTailY);
        addToSnake = false;
    }
}

void (moveSegments)(){
    for (int i = segments_len*2-1; i > 1; i -= 2){
        segments[i] = segments[i-2];
        segments[i-1] = segments[i-3];
        //printf("i: %d i: %d i+1: %d\n", i , segments[i], segments[i+1]);
    }
}

void (incrementSnake)(int tailX, int tailY){
    segments_len++;
    segments[segments_len*2-2] = tailX;
    segments[segments_len*2-1] = tailY;
}

