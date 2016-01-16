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

const uint COLOR_COUNT = 6;

const float COLOR_MATR[ COLOR_COUNT ][ 3 ] =
{
	{  52 / 255.0, 152 / 255.0, 219 / 255.0 }, // Peter river
	{  46 / 255.0, 204 / 255.0, 113 / 255.0 }, // Emerald
	{ 155 / 255.0,  89 / 255.0, 182 / 255.0 }, // Amethyst
	{ 241 / 255.0, 196 / 255.0,  15 / 255.0 }, // Sum flower
	{ 231 / 255.0,  76 / 255.0,  60 / 255.0 }, // Alizarin
	{ 236 / 255.0, 240 / 255.0, 241 / 255.0 }  // Clouds
};

const float COLOR_DARKGRAY[ 3 ] = 	{  52 / 255.0,  73 / 255.0,  94 / 255.0 }; // Wet asphalt
const float COLOR_LIGHTGRAY[ 3 ] = 	{ 149 / 255.0, 165 / 255.0, 166 / 255.0 }; // Concrete

#endif // ALL_H_INCLUDED
