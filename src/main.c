#include "raylib.h"
#include "resource_dir.h"
#include "game.h"
#include "global.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(60);
	InitWindow(GAME_WIDTH, GAME_HEIGHT, "New PONG");
	SearchAndSetResourceDir("resources");

	Shader shader = LoadShader(0, "fisheye.fs");
	RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
	SetTextureFilter(target.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

	InitGame();

	while (!WindowShouldClose())
	{
		UpdateGame();

		BeginTextureMode(target);
		ClearBackground(BLACK);
		DrawGame();
		EndTextureMode();

		BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(shader);
		DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
		EndShaderMode();
		EndDrawing();
	}

	UnloadRenderTexture(target);

	CloseWindow();
	return 0;
}
