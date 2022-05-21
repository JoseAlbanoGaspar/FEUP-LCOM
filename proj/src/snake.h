#include <lcom/lcf.h>
#include <time.h>
#include <stdlib.h>
#include "macros/i8042.h"


struct Snake {
    int direction; //0 right, 1 left, 2 up, 3 down
    uint32_t color;
    uint32_t headColor;
    uint32_t contourColor;
    int segments_len;
    int segments[2400];
    bool addToSnake;
};


struct Apple {
    int x;
    int y;
    uint32_t color;
};


void startPosition();

void (drawSnake)();

bool (canMove)(int dir);

void (moveSnake)(int dir);

void (drawBackground)();

void (moveSegments)();

void (incrementSnake)(int tailX, int tailY);

void (drawApple)();

void updateApple();

