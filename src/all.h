#ifndef ALL_H_INCLUDED
#define ALL_H_INCLUDED

#if defined(NDEBUG)
#define MY_DEBUG
#endif

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Screen FPS
const int SCREEN_FPS = 40;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

// rotation speed
const float ANGLE_DIFF = 11.0;

#endif // ALL_H_INCLUDED
