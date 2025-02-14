#include "raylib.h"

Sound sounds[3];
Sound losingSound;
Sound startSound;

void audioInit();
void playRandomSound();
void playLosingSound();
void playStartSound();
void unloadAudio();