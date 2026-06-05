#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "lighting.h"
#include "helpers.h"
#include "puzzle.h"
#include "scene.h"
#include "input.h"
#include "imageloader.h"

GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

static void initRendering()
{
    glEnable(GL_TEXTURE_2D);

    Image* wallImage = loadBMP("dindingGames.bmp");
    wallTextureID = loadTexture(wallImage);
    delete wallImage;

    Image* floorImage = loadBMP("lantaiGames.bmp");
    floorTextureID = loadTexture(floorImage);
    delete floorImage;

    glDisable(GL_TEXTURE_2D);
}

static void drawMenuCard(float cx, float cy, float bw, float bh,
                         float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(cx - bw/2, cy - bh/2);
        glVertex2f(cx + bw/2, cy - bh/2);
        glVertex2f(cx + bw/2, cy + bh/2);
        glVertex2f(cx - bw/2, cy + bh/2);
    glEnd();
}

static void drawStrokeText2D(float sx, float sy, const char* text,
                             float scale, float r, float g, float b)
{
    glPushMatrix();
        glColor3f(r, g, b);
        glTranslatef(sx, sy, 0.0f);
        glScalef(scale, scale, scale);
        for (int i = 0; text[i] != '\0'; i++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    glPopMatrix();
}

static void drawSmallText2D(float sx, float sy, const char* text, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(sx, sy);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}

static void drawConditionBox(float cx, float cy, float bw, float bh,
                             const char* title, const char* body)
{
    drawMenuCard(cx, cy, bw, bh, 0.247f, 0.310f, 0.267f, 0.92f);
    glColor4f(0.635f, 0.482f, 0.361f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(cx - bw/2, cy - bh/2);
        glVertex2f(cx + bw/2, cy - bh/2);
        glVertex2f(cx + bw/2, cy + bh/2);
        glVertex2f(cx - bw/2, cy + bh/2);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(cx - bw/2 + 10.0f, cy + bh/2 - 10.0f);
        glVertex2f(cx + bw/2 - 10.0f, cy + bh/2 - 10.0f);
    glEnd();

    drawSmallText2D(cx - bw/2 + 14.0f, cy + 10.0f, title, 0.863f, 0.843f, 0.788f);
    drawSmallText2D(cx - bw/2 + 14.0f, cy - 13.0f, body, 0.863f, 0.843f, 0.788f);
}

static void drawStartMenu()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glColor3f(0.172f, 0.224f, 0.188f); glVertex2f(0, 0);
        glColor3f(0.118f, 0.153f, 0.130f); glVertex2f(w, 0);
        glColor3f(0.247f, 0.310f, 0.267f); glVertex2f(w, h);
        glColor3f(0.172f, 0.224f, 0.188f); glVertex2f(0, h);
    glEnd();

    drawMenuCard(w/2.0f, h/2.0f + 4.0f, 560.0f, 360.0f,
                 0.172f, 0.224f, 0.188f, 0.88f);

    glColor4f(0.635f, 0.482f, 0.361f, 0.95f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(w/2.0f - 280.0f, h/2.0f - 176.0f);
        glVertex2f(w/2.0f + 280.0f, h/2.0f - 176.0f);
        glVertex2f(w/2.0f + 280.0f, h/2.0f + 184.0f);
        glVertex2f(w/2.0f - 280.0f, h/2.0f + 184.0f);
    glEnd();

    drawStrokeText2D(w/2.0f - 238.0f, h/2.0f + 126.0f,
                     "MASTER CRACK", 0.34f, 0.863f, 0.843f, 0.788f);
    drawText2D(w/2.0f - 178.0f, h/2.0f + 94.0f,
               "The 7th Golden Silence", 0.635f, 0.482f, 0.361f);
    drawSmallText2D(w/2.0f - 160.0f, h/2.0f + 70.0f,
                    "Pecahkan 7 teka-teki sebelum waktu habis.", 0.863f, 0.843f, 0.788f);

    drawConditionBox(w/2.0f - 135.0f, h/2.0f + 30.0f, 250.0f, 56.0f,
                     "KAMERA", "Mouse / Arrow Keys untuk melihat bebas");
    drawConditionBox(w/2.0f + 135.0f, h/2.0f + 30.0f, 250.0f, 56.0f,
                     "GERAK", "W/S maju-mundur, A/D menoleh");
    drawConditionBox(w/2.0f - 135.0f, h/2.0f - 42.0f, 250.0f, 56.0f,
                     "PUZZLE", "Tekan 1-7, ketik jawaban, Enter");
    drawConditionBox(w/2.0f + 135.0f, h/2.0f - 42.0f, 250.0f, 56.0f,
                     "MANUAL", "Klik Manual Book atau tekan M");

    drawMenuCard(w/2.0f, h/2.0f - 120.0f, 200.0f, 50.0f,
                 0.635f, 0.482f, 0.361f, 0.98f);

    glColor4f(0.863f, 0.843f, 0.788f, 0.95f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(w/2.0f - 100.0f, h/2.0f - 145.0f);
        glVertex2f(w/2.0f + 100.0f, h/2.0f - 145.0f);
        glVertex2f(w/2.0f + 100.0f, h/2.0f - 95.0f);
        glVertex2f(w/2.0f - 100.0f, h/2.0f - 95.0f);
    glEnd();
    
    drawText2D(w/2.0f - 72.0f, h/2.0f - 130.0f, "MULAI PERMAINAN", 0.172f, 0.224f, 0.188f);
    drawText2D(w/2.0f - 150.0f, h/2.0f - 165.0f,
               "Klik tombol atau tekan ENTER / SPACE", 0.863f, 0.843f, 0.788f);

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

 
// DISPLAY
void display()
{
    if (gameState == STATE_MENU) {
        drawStartMenu();
        glutSetWindowTitle("Master Crack: The 7th Golden Silence | Menu");
        return;
    }

    if (deltaMove) moveMeFlat(deltaMove);
    if (deltaAngle || deltaPitch) {
        angle += deltaAngle;
        pitchAngle += deltaPitch;
        orientMe(angle);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    KotakKayu();

    // Hitung sudah selesai berapa
    int solvedCount = 0;
    for (int i = 1; i <= 7; i++) if (puzzleDone[i]) solvedCount++;

    char title[128];
    sprintf(title, "Master Crack: The 7th Golden Silence | WAKTU: %02d:%02d | NYAWA: %d | SELESAI: %d/7 | P-AKTIF: %d",
            timeLeft/60, timeLeft%60, livesLeft, solvedCount, activePuzzle);
    glutSetWindowTitle(title);

    if (feedbackTimer > 0) feedbackTimer--;

    glutSwapBuffers();
    glFlush();
}

 
// MAIN
int main(int argc, char **argv)
{
    // Inisialisasi array puzzle input
    for (int i = 0; i < 8; i++) {
        puzzleInput[i][0] = '\0';
        puzzleLen[i] = 0;
        puzzleDone[i] = false;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Master Crack: The 7th Golden Silence - 7 Puzzle Escape Game");
    glutIgnoreKeyRepeat(1);

    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(typeKey);
    glutKeyboardUpFunc(releaseNormalKey);
    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseLook);
    glutMotionFunc(mouseLook);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);

    lighting();
    init();
    initRendering();
    glutTimerFunc(1000, timer, 0);

    glutMainLoop();
    return 0;
}
