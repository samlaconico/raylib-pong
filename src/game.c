#include "raylib.h"
#include "game.h"
#include "global.h"
#include "raymath.h"
#include <stdio.h>
#include "stdlib.h"

enum STATE
{
    GAME,
    TITLE
};

static Wall wall[2] = {0};
static Paddle paddleLeft = {0};
static Paddle paddleRight = {0};
static Ball ball = {0};

int score[2] = {0};
int currentColorPaddleLeft = 0;
int currentColorPaddleRight = 0;

bool start = false;

enum STATE state = TITLE;
int turn = 0;
float hue;

void InitGame()
{
    // initialize top and bottom walls
    wall[0].size = (Vector2){GAME_WIDTH, 30};
    wall[1].size = (Vector2){GAME_WIDTH, 30};
    wall[0].color = WHITE;
    wall[1].color = WHITE;
    wall[0].position = (Vector2){0, 0};
    wall[1].position = (Vector2){0, GAME_HEIGHT - wall[1].size.y};

    // initialize left paddle
    paddleLeft.size = (Vector2){20, 120};
    paddleLeft.position = (Vector2){25, (GAME_HEIGHT / 2) - (paddleLeft.size.y / 2)};
    paddleLeft.color = colorArray[currentColorPaddleLeft];

    // initialize right paddle
    paddleRight.size = (Vector2){20, 120};
    paddleRight.position = (Vector2){GAME_WIDTH - 50, (GAME_HEIGHT / 2) - (paddleRight.size.y / 2)};
    paddleRight.color = colorArray[currentColorPaddleRight];

    // initialize ball
    ball.position = (Vector2){GAME_WIDTH / 2, GAME_HEIGHT / 2};
    ball.size = (Vector2){10, 10};
    ball.color = colorArray[GetRandomValue(0, 2)];

    score[0] = 0; // player 1 score
    score[1] = 0; // player 2 score
}

void Reset()
{
    start = false;

    paddleLeft.velocity = (Vector2){0, 0};
    paddleRight.velocity = (Vector2){0, 0};

    paddleLeft.position = (Vector2){25, (GAME_HEIGHT / 2) - (paddleLeft.size.y / 2)};
    paddleRight.position = (Vector2){GAME_WIDTH - 50, (GAME_HEIGHT / 2) - (paddleRight.size.y / 2)};

    ball.velocity = (Vector2){0, 0};
    ball.position = (Vector2){GAME_WIDTH / 2, GAME_HEIGHT / 2};
    ball.color = colorArray[GetRandomValue(0, 2)];
}

void UpdateGame()
{
    switch (state)
    {
    case GAME:
        if (start)
        {
            // controls for left paddle
            if (IsKeyDown(KEY_W))
            {
                paddleLeft.velocity.y -= 2;
            }
            if (IsKeyDown(KEY_S))
            {
                paddleLeft.velocity.y += 2;
            }
            if (IsKeyPressed(KEY_D))
            {
                currentColorPaddleLeft += 1;
                paddleLeft.color = colorArray[abs(currentColorPaddleLeft % 3)];
            }
            if (IsKeyPressed(KEY_A))
            {
                currentColorPaddleLeft -= 1;
                paddleLeft.color = colorArray[abs(currentColorPaddleLeft % 3)];
            }

            // controls for right paddle
            if (IsKeyDown(KEY_UP))
            {
                paddleRight.velocity.y -= 2;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                paddleRight.velocity.y += 2;
            }
            if (IsKeyPressed(KEY_LEFT))
            {
                currentColorPaddleRight += 1;
                paddleRight.color = colorArray[abs(currentColorPaddleRight % 3)];
            }
            if (IsKeyPressed(KEY_RIGHT))
            {
                currentColorPaddleRight -= 1;
                paddleRight.color = colorArray[abs(currentColorPaddleRight % 3)];
            }
        }

        if (IsKeyPressed(KEY_R))
        {
            Reset();
        }

        if (IsKeyPressed(KEY_SPACE) && ball.velocity.x == 0 && ball.velocity.y == 0)
        {
            start = true;
            if (turn == 0)
            {
                ball.velocity = (Vector2){-5, 0};
            }
            else
            {
                ball.velocity = (Vector2){5, 0};
            }
        }

        paddleLeft.velocity.y = Clamp(paddleLeft.velocity.y, -8, 8);
        paddleRight.velocity.y = Clamp(paddleRight.velocity.y, -8, 8);

        paddleLeft.velocity.y *= .94;
        paddleRight.velocity.y *= .94;

        // paddle collision with walls
        if (paddleLeft.position.y + paddleLeft.velocity.y > (wall[0].position.y + wall[0].size.y) && (paddleLeft.position.y + paddleLeft.size.y) + paddleLeft.velocity.y < (wall[1].position.y))
            paddleLeft.position.y += paddleLeft.velocity.y;

        // paddle collision with walls
        if (paddleRight.position.y + paddleRight.velocity.y > (wall[0].position.y + wall[0].size.y) && (paddleRight.position.y + paddleRight.size.y) + paddleRight.velocity.y < (wall[1].position.y))
            paddleRight.position.y += paddleRight.velocity.y;

        // ball collision with walls
        if (ball.position.y + ball.velocity.y <= wall[0].position.y + wall[0].size.y || ball.position.y + ball.size.y + ball.velocity.y >= wall[1].position.y)
        {
            if (ball.position.y > GAME_HEIGHT / 2)
            {
                wall[1].color = ball.color;
                ball.color = colorArray[GetRandomValue(0, 2)];
            }
            else
            {
                wall[0].color = ball.color;
                ball.color = colorArray[GetRandomValue(0, 2)];
            }

            ball.velocity.y *= -1;
        }

        // ball collision with left paddle
        if (ball.position.x + ball.velocity.x <= paddleLeft.position.x + paddleLeft.size.x && (ball.position.y >= paddleLeft.position.y && ball.position.y <= paddleLeft.position.y + paddleLeft.size.y) && ball.position.x > (paddleLeft.position.x + paddleLeft.size.x) && ColorIsEqual(paddleLeft.color, ball.color))
        {
            ball.velocity.x = GetRandomValue(5, 8);
            ball.velocity.y = -Clamp((paddleLeft.position.y + (paddleLeft.size.y / 2)) - (ball.position.y), -5, 5);
            ball.color = colorArray[GetRandomValue(0, 2)];
        }

        // ball collision with right paddle
        if (ball.position.x + ball.velocity.x >= paddleRight.position.x && (ball.position.y >= paddleRight.position.y && ball.position.y <= paddleRight.position.y + paddleRight.size.y) && ColorIsEqual(paddleRight.color, ball.color))
        {
            ball.velocity.x = GetRandomValue(5, 8) * -1;
            ball.velocity.y = -Clamp((paddleRight.position.y + (paddleRight.size.y / 2)) - (ball.position.y), -5, 5);
            ball.color = colorArray[GetRandomValue(0, 2)];
        }

        // ball out of bounds
        if (ball.position.x < 0 || ball.position.x > GAME_WIDTH)
        {
            if (ball.position.x < 0)
            {
                score[1] += 1;
                turn = 0;
            }
            else
            {
                score[0] += 1;
                turn = 1;
            }
            Reset();
        }

        // update ball position
        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;
        break;

    case TITLE:

        hue += GetFrameTime() * 100;

        if (hue >= 360)
            hue = 0;

        if (IsKeyDown(KEY_SPACE))
        {
            state = GAME;
        }

        break;
    }
}

void DrawGame()
{

    switch (state)
    {
    case GAME:

        for (int i = 0; i < sizeof(wall) / sizeof(wall[1]); i++)
        {
            DrawRectangle(wall[i].position.x, wall[i].position.y, wall[i].size.x, wall[i].size.y, wall[i].color);
        }

        DrawRectangle(paddleLeft.position.x, paddleLeft.position.y, paddleLeft.size.x, paddleLeft.size.y, paddleLeft.color);
        DrawRectangle(paddleRight.position.x, paddleRight.position.y, paddleRight.size.x, paddleRight.size.y, paddleRight.color);

        DrawRectangle(ball.position.x, ball.position.y, ball.size.x, ball.size.y, ball.color);

        DrawText(TextFormat("%d", score[0]), (GAME_WIDTH / 2) - 100, 100, 100, WHITE);
        DrawText(TextFormat("%d", score[1]), (GAME_WIDTH / 2) + 50, 100, 100, WHITE);

        if (!start)
        {
            DrawTextPro(GetFontDefault(), "PRESS SPACE TO PLAY", (Vector2){(GAME_WIDTH / 2) - (MeasureText("PRESS SPACE TO PLAY", 40) / 2) - 10, GAME_HEIGHT / 3}, (Vector2){0, 0}, sin(GetTime()) * 3, 40, 5, WHITE);
        }

        // DrawLine(GAME_WIDTH / 2, 0, (GAME_WIDTH / 2) + 1, GAME_HEIGHT, RED);
        break;

    case TITLE:
        DrawRectangleGradientH(0, 0, GAME_WIDTH, GAME_HEIGHT, ColorFromHSV(hue, 1, 1), ColorFromHSV(hue + 330, 1, 1));
        DrawTextPro(GetFontDefault(), "NEW PONG", (Vector2){(GAME_WIDTH / 2), 325}, (Vector2){MeasureText("NEWPONG", 100) / 2, 0}, sin(GetTime()) * 3, 100, 5, WHITE);
        DrawTextPro(GetFontDefault(), "Press Space", (Vector2){(GAME_WIDTH / 2) - (MeasureText("Press Space", 50) / 2), 425}, (Vector2){0, 0}, 0, 50, 5, WHITE);
        break;
    }
}
