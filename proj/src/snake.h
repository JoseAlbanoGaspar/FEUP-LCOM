#pragma once
#include <lcom/lcf.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "macros/i8042.h"
#include "macros/constants.h"
#include "vbe.h"


struct Snake {
    int direction; //0 right, 1 left, 2 up, 3 down
    int segments_len;
    int segments[2400];
    bool addToSnake;
    bool canChangeDir;
    bool alive;
    int score;
};


struct Enemy {
    int x;
    int y;
    int lastX;
    int lastY;
    bool active;
};


struct Apple {
    int x;
    int y;
};


void (startPosition)(uint16_t vbe_mode);

void (drawSnake)();

void (changeDirection)(uint16_t scancode);

bool (canMove)(int dir);

void (moveSnake)(int dir);

void (drawBackground)();

void (drawNumbers)(int value,int x, int y, int digit_len);

void (drawScore)();

void (moveSegments)();

void (incrementSnake)(int tailX, int tailY);

void (drawApple)();

void (updateApple)();

void (spawnEnemy)();

void (moveEnemy)();

void (eraseEnemyTrail)();

void (damageSnake)();

void (drawEnemy)();

void (killEnemy)();


