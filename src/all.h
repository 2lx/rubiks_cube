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

const uint COLOR_COUNT = 6;

const float COLOR_MATR[ COLOR_COUNT + 2 ][ 3 ] =
{
	{ 236 / 255.0, 240 / 255.0, 241 / 255.0 }, // Clouds
	{ 231 / 255.0,  76 / 255.0,  60 / 255.0 }, // Alizarin
	{ 241 / 255.0, 196 / 255.0,  15 / 255.0 }, // Sum flower
	{ 230 / 255.0, 126 / 255.0,  34 / 255.0 }, // Amethyst
	{  52 / 255.0, 152 / 255.0, 219 / 255.0 }, // Peter river
	{  46 / 255.0, 204 / 255.0, 113 / 255.0 }, // Emerald

	{  52 / 255.0,  73 / 255.0,  94 / 255.0 }, // Wet asphalt
	{ 149 / 255.0, 165 / 255.0, 166 / 255.0 } // Concrete
};

const float COLOR_DARKGRAY[ 3 ] = 	{  52 / 255.0,  73 / 255.0,  94 / 255.0 }; // Wet asphalt
const float COLOR_LIGHTGRAY[ 3 ] = 	{ 149 / 255.0, 165 / 255.0, 166 / 255.0 }; // Concrete

#endif // ALL_H_INCLUDED
