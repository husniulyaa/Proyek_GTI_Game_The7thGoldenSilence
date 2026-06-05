#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

// VARIABEL KAMERA & TAMPILAN
extern float angle, deltaAngle, ratio;
extern float pitchAngle, deltaPitch;
extern float x, y, z;
extern float lx, ly, lz;
extern float xCurr, zCurr;
extern int   deltaMove, h, w;

// VARIABEL STATUS GAME
enum GameState {
    STATE_MENU,
    STATE_PLAYING
};

extern GameState gameState;

// VARIABEL TIMER
extern int timeLeft;

// VARIABEL STATUS PUZZLE (7 PUZZLE)
extern bool puzzleDone[8]; // index 1-7

// activePuzzle: 0=belum pilih, 1..7=puzzle aktif
extern int activePuzzle;

// Input tiap puzzle (index 1-7)
extern char puzzleInput[8][16];
extern int  puzzleLen[8];

// JAWABAN 7 PUZZLE
extern const char* puzzleAnswer[8];

// VARIABEL LOGIKA GAME
extern int  livesLeft;
extern bool isGameOver;
extern bool isGameWon;

// Lampu indikator
extern float lightR, lightG, lightB;

// Texture ID utama scene
extern GLuint wallTextureID;
extern GLuint floorTextureID;

// Animasi pintu brankas
extern float doorAngle;

extern int manualBookClickX1, manualBookClickY1;
extern int manualBookClickX2, manualBookClickY2;
extern bool manualBookClickReady;

// Feedback
extern char feedbackMsg[64];
extern int  feedbackTimer;

#endif // GLOBALS_H
