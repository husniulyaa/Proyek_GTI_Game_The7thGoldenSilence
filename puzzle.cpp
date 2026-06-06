#include "puzzle.h"
#include "globals.h"
#include "helpers.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>

 
// CEK SEMUA PUZZLE SELESAI
bool cekSemuaModulSelesai(){
    for (int i = 1; i <= 7; i++)
        if (!puzzleDone[i]) return false;
    return true;
}

void checkAllPuzzleSolved(){
    if (!cekSemuaModulSelesai()) return;
    isGameWon      = true;
    lightR = 0.60f; lightG = 0.95f; lightB = 0.65f;
}

 
// BUKA MANUAL BOOK
void openManualBook(){
#ifdef _WIN32
    ShellExecuteA(NULL, "open",
        "https://www.notion.so/CONFIDENTIAL-NO-DUPLICATE-3381c0a7f09e800786edca457cb8a9a6",
        NULL, NULL, SW_SHOWNORMAL);
#else
    system("xdg-open \"https://www.notion.so/CONFIDENTIAL-NO-DUPLICATE-3381c0a7f09e800786edca457cb8a9a6\" &");
#endif
}

 
// STATUS BAR 7 PUZZLE DI ATAS PINTU
void drawPuzzleStatusBar(){
    float startX = -2.85f;
    float py = 5.8f, pz = 6.15f;
    float pw = 0.72f, ph = 0.5f;
    float gap = 0.09f;

    glDisable(GL_LIGHTING);
    const char* labels[7] = {"P1","P2","P3","P4","P5","P6","P7"};
    for (int i = 0; i < 7; i++) {
        glColor3f(0.0f, 0.0f, 0.0f);
        drawText3D(labels[i], startX + i*(pw+gap) - 0.05f, py+0.55f, pz, 0.0016f);

        if      (puzzleDone[i+1])           glColor3f(0.0f, 0.85f, 0.0f);
        else if (activePuzzle == (i+1))     glColor3f(1.0f, 0.85f, 0.0f);
        else                                glColor3f(0.5f, 0.5f, 0.5f);

        float cx = startX + i*(pw+gap);
        glBegin(GL_QUADS);
            glVertex3f(cx,    py,    pz);
            glVertex3f(cx+pw, py,    pz);
            glVertex3f(cx+pw, py+ph, pz);
            glVertex3f(cx,    py+ph, pz);
        glEnd();
    }
    glEnable(GL_LIGHTING);
}
