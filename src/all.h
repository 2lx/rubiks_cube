#ifndef ALL_H_INCLUDED
#define ALL_H_INCLUDED

#define GLM_FORCE_RADIANS

#if defined(NDEBUG)
#define MY_DEBUG
#endif

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include "rcdefines.h"

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float SCREEN_WDH = ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT;
const float SCREEN_VERTMARGIN = 1 + RC::CUBIE_COUNT;
const float SCREEN_HORIZMARGIN = SCREEN_VERTMARGIN * SCREEN_WDH;

// Screen FPS
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

// rotation speed
const float ANGLE_DIFF = 11.0;

#endif // ALL_H_INCLUDED
