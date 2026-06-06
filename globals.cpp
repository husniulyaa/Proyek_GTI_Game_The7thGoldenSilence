#include "globals.h"

// VARIABEL KAMERA & TAMPILAN
float angle = 0.0f, deltaAngle = 0.0f, ratio;
float pitchAngle = 0.0f, deltaPitch = 0.0f;
float x = 0.0f, y = 1.75f, z = 15.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float xCurr = 0.0f, zCurr = 0.0f;
int   deltaMove = 0, h = 480, w = 640;

// POV: 0 = First Person, 1 = Third Person
int cameraPOV = 0;

GameState gameState = STATE_MENU;

// TIMER
int timeLeft = 360;

// STATUS PUZZLE
bool puzzleDone[8] = { false };

int activePuzzle = 0;

char puzzleInput[8][16] = { "" };
int  puzzleLen[8]        = { 0  };

const char* puzzleAnswer[8] = {
    "",
    "b",
    "4745",
    "10",
    "21",
    "b",
    "11",
    "29"
};

// LOGIKA GAME
int  livesLeft   = 3;
bool isGameOver  = false;
bool isGameWon   = false;

// Lampu indikator
float lightR = 0.80f, lightG = 0.85f, lightB = 0.95f;

// Texture
GLuint wallTextureID = 0;
GLuint floorTextureID = 0;

// Pintu brankas
float doorAngle = 0.0f;

int manualBookClickX1 = 0, manualBookClickY1 = 0;
int manualBookClickX2 = 0, manualBookClickY2 = 0;
bool manualBookClickReady = false;

// Feedback
char feedbackMsg[64] = "";
int  feedbackTimer   = 0;