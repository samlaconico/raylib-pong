#include "raylib.h"

Sound sounds[3];
Sound losingSound;
Sound startSound;

int current;

void audioInit();
void playRandomSound();
void playLosingSound();
void playStartSound();
void unloadAudio();
void playSound(int i);