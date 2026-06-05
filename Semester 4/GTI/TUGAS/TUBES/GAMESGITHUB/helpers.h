#ifndef HELPERS_H
#define HELPERS_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

// HELPER: LINGKARAN
void Circle(float radius, float r, float g, float b, int t);

// HELPER: TEKS 3D
void drawText3D(const char* text, float tx, float ty, float tz, float scale);

 
// HELPER: TEKS 2D
void drawText2D(float sx, float sy, const char* text, float r, float g, float b);

// HELPER: KOTAK INPUT
void drawAnswerBox(float bx, float by, float bz,
                   int puzzleIdx, bool isActive, bool isDone);

 
// JAM ANALOG
void drawAnalogClock(float totalTimeSecs, float timeRemainingSecs);

#endif // HELPERS_H
