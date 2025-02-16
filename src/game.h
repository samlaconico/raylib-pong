#pragma once
#include "raylib.h"

static Color colorArray[3] = {PINK, YELLOW, GREEN};

typedef struct Paddle
{
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    Color color;
    int player;
} Paddle;

typedef struct Ball
{
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    Color color;
    int direction;
} Ball;

typedef struct Wall
{
    Vector2 position;
    Vector2 size;
    Color color;
} Wall;

void InitGame(bool *e);
void movePaddle();
void Reset();
void UpdateGame();
void DrawGame();