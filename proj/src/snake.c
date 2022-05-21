#include "snake.h"

int snakeX = 100;
int snakeY = 100;
int snakeTailX = 80;
int snakeTailY = 80;
int direction = 0; //0 right, 1 left, 2 up, 3 down
uint32_t snakeColor =  0x002ADF23;
uint32_t snakeHeadColor = 0x00179B12;
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
}

void (drawSnake)(){
    uint32_t snakeEyesColor = 0x00090B09;
    int pos1X = segments[0];
    int pos1Y = segments[1];
    int pos2X = segments[0];
    int pos2Y = segments[1];
    for (int i = 0; i < segments_len*2; i += 2){
        if (i == 0) { //draw the head of the snake
            vg_draw_rectangle(segments[i], segments[i+1], 20, 20, snakeHeadColor);
            switch (direction)
            {
            case 0: //RIGHT
                pos1X += 12; 
                pos1Y += 5;
                pos2X += 12;
                pos2Y += 12;
                break;
            case 1: //LEFT
                pos1X += 4;
                pos1Y += 5;
                pos2X += 4;
                pos2Y += 12;
                break;
            case 2: //UP
                pos1X += 5;
                pos1Y += 4;
                pos2X += 12;
                pos2Y += 4;
                break;
            case 3: // DOWN
                pos1X += 5;
                pos1Y += 12;
                pos2X += 12;
                pos2Y += 12;
                break;
            default:
                break;
            }
            vg_draw_rectangle(pos1X, pos1Y, 4, 4, snakeEyesColor);
            vg_draw_rectangle(pos2X, pos2Y, 4, 4, snakeEyesColor);
        }
        else vg_draw_rectangle(segments[i], segments[i+1], 20, 20, snakeColor);
    }
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
    case 0: //RIGHT
        if (snakeX + 20 < 800) snakeX += 20;
        else snakeX = 0;
        break;
    case 1: //LEFT
        if (snakeX - 20 >= 0) snakeX -= 20;
        else snakeX = 780;
        break;
    case 2: //UP
        if (snakeY - 20 >= 0) snakeY -= 20;
        else snakeY = 580;
        break;
    case 3: //DOWN
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

