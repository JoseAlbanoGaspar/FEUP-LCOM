#include "snake.h"


bool addToSnake = false;
extern uint16_t h_res;
extern uint16_t v_res;
int arena_x = 0;
int arena_y = 0;
uint16_t video_mode;

struct Snake snake;
struct Apple apple;

void (startPosition)(uint16_t vbe_mode){
    /* Initializes snake */
    for (int i = 0; i < 9; i+=2) snake.segments[i] = 120 - (i*10);
    for (int j = 1; j < 10; j+=2) snake.segments[j] = 120;
    snake.direction = 0;
    snake.segments_len = 5;
    snake.addToSnake = false;
    snake.canChangeDir = true;
    arena_x = (int) h_res;
    arena_y = ((int) v_res) - 80;
    video_mode = vbe_mode;
 
    /* Initializes apple */
    apple.x = 300;
    apple.y = 200;
    srand(time(NULL));  //initialize the randum number generator 
}

void (drawBackground)(){
  /* Draw Rectangle */
  uint32_t color = 0x0;
  if (video_mode == 0x115 || video_mode == 0x14C) color = ARENA_BACKGROUND_COLOR;
  else if (video_mode == 0x110) color = ARENA_BACKGROUND_COLOR_110;
  else if (video_mode == 0x105) color = ARENA_BACKGROUND_COLOR_105;
  else if (video_mode == 0x11A) color = ARENA_BACKGROUND_COLOR_11A;
  
  vg_draw_rectangle(snake.segments[2*(snake.segments_len-1)], snake.segments[2*(snake.segments_len-1)+1], 20, 20, color); //0x000057FF
}

void (drawSnake)(){
    int pos1X = snake.segments[0];
    int pos1Y = snake.segments[1];
    int pos2X = snake.segments[0];
    int pos2Y = snake.segments[1];
    uint32_t snake_head_color = 0x0, snake_eye_color = 0x0, snake_contour_color = 0x0, snake_color = 0x0;
    if (video_mode == 0x115 || video_mode == 0x14C) {
        snake_head_color = SNAKE_HEAD_COLOR;
        snake_eye_color = SNAKE_EYE_COLOR;
        snake_contour_color = SNAKE_CONTOUR_COLOR;
        snake_color = SNAKE_COLOR;
    }
    else if (video_mode == 0x110) {
        snake_head_color = SNAKE_HEAD_COLOR_110;
        snake_eye_color = SNAKE_EYE_COLOR_110;
        snake_contour_color = SNAKE_CONTOUR_COLOR_110;
        snake_color = SNAKE_COLOR_110;
    }
    else if (video_mode == 0x105) {
        snake_head_color = SNAKE_HEAD_COLOR_105;
        snake_eye_color = SNAKE_EYE_COLOR_105;
        snake_contour_color = SNAKE_CONTOUR_COLOR_105;
        snake_color = SNAKE_COLOR_105;
    }
    else if (video_mode == 0x11A) {
        snake_head_color = SNAKE_HEAD_COLOR_11A;
        snake_eye_color = SNAKE_EYE_COLOR_11A;
        snake_contour_color = SNAKE_CONTOUR_COLOR_11A;
        snake_color = SNAKE_COLOR_11A;
    }
    for (int i = 0; i < snake.segments_len*2; i += 2){
        if (i == 0) { //draw the head of the snake 
            vg_draw_rectangle(snake.segments[i], snake.segments[i+1], 20, 20, snake_head_color);
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
            vg_draw_rectangle(pos1X, pos1Y, 4, 4, snake_eye_color);
            vg_draw_rectangle(pos2X, pos2Y, 4, 4, snake_eye_color);
        }
        else {
            vg_draw_rectangle(snake.segments[i], snake.segments[i+1], 20, 20, snake_contour_color);
            vg_draw_rectangle(snake.segments[i]+2, snake.segments[i+1]+2, 15, 15, snake_color);
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
        if (snakeX + 20 < arena_x) snakeX += 20;
        else snakeX = 0;
        break;
    case 1: //LEFT
        if (snakeX - 20 >= 0) snakeX -= 20;
        else snakeX = arena_x-20;
        break;
    case 2: //UP
        if (snakeY - 20 >= 0) snakeY -= 20;
        else snakeY = arena_y-20;
        break;
    case 3: //DOWN
        if (snakeY + 20 < arena_y) snakeY += 20;
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
    snake.canChangeDir = true;
    if (!addToSnake) drawBackground();
    snakeX = snake.segments[0];
    snakeY = snake.segments[1];
    snakeTailX = snake.segments[snake.segments_len*2 -2];
    snakeTailY = snake.segments[snake.segments_len*2 -1];
    moveSegments();
    switch (dir)
    {
    case 0: //RIGHT
        if (snakeX + 20 < arena_x) snakeX += 20;
        else snakeX = 0;
        break;
    case 1: //LEFT
        if (snakeX - 20 >= 0) snakeX -= 20;
        else snakeX = arena_x-20;
        break;
    case 2: //UP
        if (snakeY - 20 >= 0) snakeY -= 20;
        else snakeY = arena_y-20;
        break;
    case 3: //DOWN
        if (snakeY + 20 < arena_y) snakeY += 20;
        else snakeY = 0;
        break;
    default:
        break;
    }
    snake.segments[0] = snakeX;
    snake.segments[1] = snakeY;
    if (snake.addToSnake){
        incrementSnake(snakeTailX, snakeTailY);
        snake.addToSnake = false;
    }
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
    uint32_t apple_color = 0x0;
    if (video_mode == 0x115 || video_mode == 0x14C) apple_color = APPLE_COLOR;
    else if (video_mode == 0x110) apple_color = APPLE_COLOR_110;
    else if (video_mode == 0x105) apple_color = APPLE_COLOR_105;
    else if (video_mode == 0x11A) apple_color = APPLE_COLOR_11A;  
    vg_draw_rectangle(apple.x, apple.y, 20, 20, apple_color);
}

void updateApple(){
    apple.x = (rand() % (arena_x/20)) * 20;
    apple.y = (rand() % (arena_y/20)) * 20;
    for (int i = 0; i < snake.segments_len*2; i+=2){
        if (apple.x == snake.segments[i] && apple.y == snake.segments[i+1]) updateApple();
    }
}

void (changeDirection)(uint16_t scancode){
    switch (scancode)
    {
    case RIGHT_ARROW:
        if (snake.direction == 1) break;
        if (snake.canChangeDir) snake.direction = 0;
        snake.canChangeDir = false;
        break;
    case LEFT_ARROW:
        if (snake.direction == 0) break;
        if (snake.canChangeDir) snake.direction = 1;
        snake.canChangeDir = false;
        break;
    case UP_ARROW:
        if (snake.direction == 3) break;
        if (snake.canChangeDir) snake.direction = 2;
        snake.canChangeDir = false;
        break;
    case DOWN_ARROW:
        if (snake.direction == 2) break;
        if (snake.canChangeDir) snake.direction = 3;
        snake.canChangeDir = false;
        break;
    default:
        break;
    }

}

