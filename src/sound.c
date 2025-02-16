#include "raylib.h"

Sound sounds[3] = {0};
Sound losingSound;
Sound startSound;

int current;

void audioInit()
{
    sounds[0] = LoadSound("sound1.wav");
    sounds[1] = LoadSound("sound2.wav");
    sounds[2] = LoadSound("sound3.wav");

    losingSound = LoadSound("lose.wav");
    startSound = LoadSound("start.wav");

    current = 0;
}

void playRandomSound()
{
    int n = GetRandomValue(0, 2);

    while (n == current)
    {
        n = GetRandomValue(0, 2);
    }

    current = n;

    PlaySound(sounds[n]);
}

void playLosingSound()
{
    PlaySound(losingSound);
}

void playStartSound()
{
    PlaySound(startSound);
}

void unloadAudio()
{
    int n = sizeof(sounds) / sizeof(sounds[0]);

    for (int i = 0; i < n; i++)
    {
        UnloadSound(sounds[i]);
    }

    UnloadSound(losingSound);
    UnloadSound(startSound);
}