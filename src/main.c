#include "raylib.h"
#include "resource_dir.h"
#include "game.h"
#include "global.h"
#include "sound.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(60);

	InitWindow(GAME_WIDTH, GAME_HEIGHT, "New PONG");
	InitAudioDevice();

	bool exitGame = false;
	SearchAndSetResourceDir("resources");

	Shader shader = LoadShader(0, "fisheye.fs");
	RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
	SetTextureFilter(target.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

	SetExitKey(KEY_NULL);
	InitGame(&exitGame);

	while (!exitGame)
	{
		if (WindowShouldClose())
			exitGame = true;

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

	unloadAudio();
	UnloadRenderTexture(target);

	CloseWindow();
	return 0;
}
