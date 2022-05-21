#include "snake.h"


bool addToSnake = false;
extern uint32_t backgroundColor;


struct Snake snake;
struct Apple apple;

void startPosition(){
    /* Initializes snake */
    snake.segments[0] = 120;
    snake.segments[1] = 120;
    snake.segments[2] = 100;
    snake.segments[3] = 120;
    snake.segments[4] = 80;
    snake.segments[5] = 120;
    snake.segments[6] = 60;
    snake.segments[7] = 120;
    snake.segments[8] = 40;
    snake.segments[9] = 120;
    snake.direction = 0;
    snake.color =  0x002ADF23;
    snake.headColor = 0x00179B12;
    snake.contourColor  = 0x0018AB18;
    snake.segments_len = 5;
    snake.addToSnake = false;

    /* Initializes apple */
    apple.x = 300;
    apple.y = 200;
    apple.color = 0x00DC1717;
    srand(time(NULL));  //initialize the randum number generator 
}

void (drawBackground)(){
  /* Draw Rectangle */
  vg_draw_rectangle(snake.segments[2*(snake.segments_len-1)], snake.segments[2*(snake.segments_len-1)+1], 20, 20, backgroundColor); //0x000057FF
}

void (drawSnake)(){
    uint32_t snakeEyesColor = 0x00090B09;
    int pos1X = snake.segments[0];
    int pos1Y = snake.segments[1];
    int pos2X = snake.segments[0];
    int pos2Y = snake.segments[1];
    for (int i = 0; i < snake.segments_len*2; i += 2){
        if (i == 0) { //draw the head of the snake
            vg_draw_rectangle(snake.segments[i], snake.segments[i+1], 20, 20, snake.headColor);
            switch (snake.direction)
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
        else {
            vg_draw_rectangle(snake.segments[i], snake.segments[i+1], 20, 20, snake.contourColor);
            vg_draw_rectangle(snake.segments[i]+2, snake.segments[i+1]+2, 15, 15, snake.color);
        }
    }
}

bool (canMove)(int dir){
    int snakeX, snakeY;
    snakeX = snake.segments[0];
    snakeY = snake.segments[1];
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
    if (snakeX == apple.x && snakeY == apple.y) {
        snake.addToSnake = true;
        return true;
    }
    for (int i = 2; i < snake.segments_len*2; i += 2) {
        if (snakeX == snake.segments[i] && snakeY == snake.segments[i+1]) return false;
    }
    return true;
}

void (moveSnake)(int dir){
    int snakeX, snakeY, snakeTailX, snakeTailY;
    if (!addToSnake) drawBackground();
    snakeX = snake.segments[0];
    snakeY = snake.segments[1];
    snakeTailX = snake.segments[snake.segments_len*2 -2];
    snakeTailY = snake.segments[snake.segments_len*2 -1];
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
    snake.segments[0] = snakeX;
    snake.segments[1] = snakeY;
    if (snake.addToSnake) printf("got apple\n");
    else printf("not got apple\n");
    if (snake.addToSnake){
        incrementSnake(snakeTailX, snakeTailY);
        snake.addToSnake = false;
    }
    printf("after apple\n");
}

void (moveSegments)(){
    for (int i = snake.segments_len*2-1; i > 1; i -= 2){
        snake.segments[i] = snake.segments[i-2];
        snake.segments[i-1] = snake.segments[i-3];
    }
}

void (incrementSnake)(int tailX, int tailY){
    snake.segments_len++;
    snake.segments[snake.segments_len*2-2] = tailX;
    snake.segments[snake.segments_len*2-1] = tailY;
    updateApple();
}

void (drawApple)(){
     vg_draw_rectangle(apple.x, apple.y, 20, 20, apple.color);
}

void updateApple(){
    apple.x = (rand() % 40) * 20;
    apple.y = (rand() % 30) * 20;
}

