#ifndef INPUT_H
#define INPUT_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>


// CALLBACK KAMERA
void orientMe(float ang);
void moveMeFlat(int i);

// CALLBACK RESHAPE
void Reshape(int w1, int h1);

// CALLBACK TIMER
void timer(int value);

// CALLBACK INPUT
void typeKey(unsigned char key, int mx, int my);
void releaseNormalKey(unsigned char key, int mx, int my);
void mouseButton(int button, int state, int mx, int my);
void mouseLook(int mx, int my);
void pressKey(int key, int mx, int my);
void releaseKey(int key, int mx, int my);

#endif // INPUT_H
