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
const int INIT_SCREEN_WIDTH = 800;
const int INIT_SCREEN_HEIGHT = 600;

// Screen FPS
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

#endif // ALL_H_INCLUDED
