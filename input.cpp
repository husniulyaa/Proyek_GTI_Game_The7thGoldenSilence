#include "input.h"
#include "globals.h"
#include "puzzle.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define PITCH_LIMIT 1.48353f
#define MOUSE_SENSITIVITY 0.0045f

static int lastMouseX = -1;
static int lastMouseY = -1;

static void updateView(){
    orientMe(angle);
    glutPostRedisplay();
}

static void startGameFromMenu()
{
    if (gameState == STATE_MENU) {
        gameState = STATE_PLAYING;
        orientMe(angle);
        glutPostRedisplay();
    }
}

// CALLBACK KAMERA
void applyCameraView()
{
    glLoadIdentity();

    float flatLen = sqrt(lx * lx + lz * lz);
    float flatX = lx;
    float flatZ = lz;

    if (flatLen < 0.0001f) {
        flatX = sin(angle);
        flatZ = -cos(angle);
        flatLen = 1.0f;
    }

    flatX /= flatLen;
    flatZ /= flatLen;

    if (cameraPOV == 0) {
        // First Person
        gluLookAt(
            x, y, z,
            x + lx, y + ly, z + lz,
            0.0f, 1.0f, 0.0f
        );
    }
    else {
        // Third Person
        gluLookAt(
            x - flatX * 8.0f,
            y + 3.2f,
            z - flatZ * 8.0f,
            x + flatX * 2.5f,
            y + 1.0f + ly,
            z + flatZ * 2.5f,
            0.0f, 1.0f, 0.0f
        );
    }
}

const char* getPOVName(){
    return (cameraPOV == 0) ? "First Person" : "Third Person";
}

void orientMe(float ang){
    if (pitchAngle > PITCH_LIMIT) pitchAngle = PITCH_LIMIT;
    if (pitchAngle < -PITCH_LIMIT) pitchAngle = -PITCH_LIMIT;

    lx = sin(ang) * cos(pitchAngle);
    ly = sin(pitchAngle);
    lz = -cos(ang) * cos(pitchAngle);

    applyCameraView();
}

void moveMeFlat(int i){
    xCurr = x + i * lx * 0.03f;
    zCurr = z + i * lz * 0.03f;

    if (xCurr < -19 || xCurr > 19) return;
    if (zCurr < -9  || zCurr > 20) return;
    if (xCurr > -11 && xCurr < 11 && zCurr > 2 && zCurr < 7) return;

    x = xCurr;
    z = zCurr;

    applyCameraView();
}

// CALLBACK RESHAPE
void Reshape(int w1, int h1){
    if (h1 == 0) h1 = 1;

    w = w1;
    h = h1;
    ratio = 1.0f * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    applyCameraView();
}

// CALLBACK TIMER
void timer(int value){
    if (gameState == STATE_MENU) {
        glutTimerFunc(1000, timer, 0);
    }
    else if (!isGameOver && !isGameWon && timeLeft > 0) {
        timeLeft--;
        glutTimerFunc(1000, timer, 0);
    }
    else if (timeLeft <= 0 && !isGameWon) {
        timeLeft = 0;
        isGameOver = true;
        lightR = 0.95f;
        lightG = 0.60f;
        lightB = 0.60f;
    }

    if (feedbackTimer > 0) feedbackTimer--;

    glutPostRedisplay();
}

// RESET GAME
void resetGame(){
    timeLeft = 360;
    livesLeft = 3;
    isGameOver = false;
    isGameWon = false;
    activePuzzle = 0;
    doorAngle = 0.0f;

    // Langsung balik ke kondisi awal bermain, bukan menu
    gameState = STATE_PLAYING;

    // Reset kamera
    cameraPOV = 0;
    x = 0.0f;
    y = 1.75f;
    z = 15.0f;
    angle = 0.0f;
    pitchAngle = 0.0f;

    deltaMove = 0;
    deltaAngle = 0.0f;
    deltaPitch = 0.0f;

    // Reset lampu
    lightR = 0.80f;
    lightG = 0.85f;
    lightB = 0.95f;

    // Reset feedback
    feedbackMsg[0] = '\0';
    feedbackTimer = 0;

    // Reset puzzle
    for (int i = 0; i < 8; i++) {
        puzzleDone[i] = false;
        puzzleInput[i][0] = '\0';
        puzzleLen[i] = 0;
    }

    orientMe(angle);
    glutTimerFunc(1000, timer, 0);
    glutPostRedisplay();
}

// KEYBOARD
void typeKey(unsigned char key, int mx, int my){
    if (gameState == STATE_MENU) {
        if (key == 13 || key == ' ') {
            startGameFromMenu();
        }
        return;
    }

    if (activePuzzle == 0 && !isGameOver && !isGameWon) {
        // V -> ganti POV First Person / Third Person
        if (key == 'v' || key == 'V') {
            cameraPOV = (cameraPOV + 1) % 2;
            applyCameraView();
            glutPostRedisplay();
            return;
        }

        if (key == 'a' || key == 'A') {
            deltaAngle = -0.003f;
            return;
        }

        if (key == 'd' || key == 'D') {
            deltaAngle = 0.003f;
            return;
        }

        if (key == 'w' || key == 'W') {
            deltaMove = 1;
            return;
        }

        if (key == 's' || key == 'S') {
            deltaMove = -1;
            return;
        }
    }

    // M -> Manual Book
    if (key == 'm' || key == 'M') {
        openManualBook();
        return;
    }

    // ESC -> keluar dari puzzle aktif
    if (key == 27) {
        activePuzzle = 0;
        glutPostRedisplay();
        return;
    }

    // Pilih puzzle 1-7 hanya jika belum ada puzzle aktif
    if (activePuzzle == 0 && key >= '1' && key <= '7') {
        int idx = key - '0';

        if (!puzzleDone[idx]) {
            activePuzzle = idx;
        }

        glutPostRedisplay();
        return;
    }

    // R -> Restart
    if (key == 'r' || key == 'R') {
        resetGame();
        return;
    }

    if (isGameOver || isGameWon || timeLeft <= 0) return;
    if (activePuzzle == 0 || activePuzzle > 7) return;

    char* curInput = puzzleInput[activePuzzle];
    int* curLen = &puzzleLen[activePuzzle];

    if (key == 8) {
        // Backspace
        if (*curLen > 0) {
            (*curLen)--;
            curInput[*curLen] = '\0';
        }
    }
    else if (key == 13) {
        // Enter -> Submit
        bool correct = false;
        const char* ans = puzzleAnswer[activePuzzle];

        char lowerInput[16];
        strncpy(lowerInput, curInput, 15);
        lowerInput[15] = '\0';

        for (int i = 0; lowerInput[i]; i++) {
            lowerInput[i] = tolower(lowerInput[i]);
        }

        char lowerAns[16];
        strncpy(lowerAns, ans, 15);
        lowerAns[15] = '\0';

        for (int i = 0; lowerAns[i]; i++) {
            lowerAns[i] = tolower(lowerAns[i]);
        }

        if (strcmp(lowerInput, lowerAns) == 0) {
            correct = true;
            puzzleDone[activePuzzle] = true;

            lightR = 0.60f;
            lightG = 0.95f;
            lightB = 0.65f;

            sprintf(feedbackMsg, "BENAR! Puzzle %d Selesai!", activePuzzle);
            feedbackTimer = 150;

            int next = activePuzzle + 1;

            while (next <= 7 && puzzleDone[next]) {
                next++;
            }

            activePuzzle = (next <= 7) ? next : 0;

            checkAllPuzzleSolved();
        }
        else {
            livesLeft--;

            lightR = 0.95f;
            lightG = 0.60f;
            lightB = 0.60f;

            sprintf(feedbackMsg, "SALAH! Nyawa tersisa: %d", livesLeft);
            feedbackTimer = 150;

            if (livesLeft <= 0) {
                isGameOver = true;
            }
        }

        *curLen = 0;
        curInput[0] = '\0';
    }
    else if ((key >= '0' && key <= '9') ||
             (key >= 'a' && key <= 'z') ||
             (key >= 'A' && key <= 'Z')) {
        if (*curLen < 12) {
            curInput[*curLen] = key;
            (*curLen)++;
            curInput[*curLen] = '\0';
        }
    }

    if (feedbackTimer <= 0 && !isGameWon && !isGameOver) {
        lightR = 0.80f;
        lightG = 0.85f;
        lightB = 0.95f;
    }

    glutPostRedisplay();
}

void releaseNormalKey(unsigned char key, int mx, int my){
    if (key == 'a' || key == 'A' || key == 'd' || key == 'D') {
        deltaAngle = 0.0f;
    }

    if (key == 'w' || key == 'W' || key == 's' || key == 'S') {
        deltaMove = 0;
    }
}

void mouseButton(int button, int state, int mx, int my){
    if (gameState == STATE_PLAYING && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int myGL = h - my;

        if (manualBookClickReady &&
            mx >= manualBookClickX1 && mx <= manualBookClickX2 &&
            myGL >= manualBookClickY1 && myGL <= manualBookClickY2) {
            openManualBook();
            return;
        }
    }

    if (gameState != STATE_MENU || button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
        return;
    }

    int myGL = h - my;
    float left = w / 2.0f - 90.0f;
    float right = w / 2.0f + 90.0f;
    float bottom = h / 2.0f - 149.0f;
    float top = h / 2.0f - 103.0f;

    if (mx >= left && mx <= right && myGL >= bottom && myGL <= top) {
        startGameFromMenu();
    }
}

void pressKey(int key, int mx, int my){
    if (gameState == STATE_MENU) {
        if (key == GLUT_KEY_F1) {
            startGameFromMenu();
        }
        return;
    }

    switch (key) {
        case GLUT_KEY_LEFT:
            deltaAngle = -0.003f;
            break;

        case GLUT_KEY_RIGHT:
            deltaAngle = 0.003f;
            break;

        case GLUT_KEY_UP:
            deltaPitch = 0.003f;
            break;

        case GLUT_KEY_DOWN:
            deltaPitch = -0.003f;
            break;
    }
}

void releaseKey(int key, int mx, int my){
    switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            deltaAngle = 0.0f;
            break;

        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            deltaPitch = 0.0f;
            break;
    }
}

void mouseLook(int mx, int my){
    if (gameState != STATE_PLAYING || activePuzzle != 0 || isGameOver || isGameWon) {
        lastMouseX = mx;
        lastMouseY = my;
        return;
    }

    if (lastMouseX < 0 || lastMouseY < 0) {
        lastMouseX = mx;
        lastMouseY = my;
        return;
    }

    int dx = mx - lastMouseX;
    int dy = my - lastMouseY;
    lastMouseX = mx;
    lastMouseY = my;
    angle += dx * MOUSE_SENSITIVITY;
    pitchAngle -= dy * MOUSE_SENSITIVITY;

    updateView();
}