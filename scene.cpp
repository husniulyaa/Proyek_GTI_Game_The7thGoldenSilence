#include "scene.h"
#include "globals.h"
#include "helpers.h"
#include "puzzle.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <GL/glu.h>
#include <string.h>
#include <stdio.h>

static void beginTexture(GLuint texId)
{
    if (texId == 0) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

static void endSteelTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

static void drawPanelBorder(float halfW, float halfH, float z,
                            float r, float g, float b)
{
    glDisable(GL_LIGHTING);
    glColor3f(r, g, b);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-halfW, -halfH, z);
        glVertex3f( halfW, -halfH, z);
        glVertex3f( halfW,  halfH, z);
        glVertex3f(-halfW,  halfH, z);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(-halfW, halfH - 0.16f, z); glVertex3f(-halfW + 0.28f, halfH, z);
        glVertex3f( halfW, halfH - 0.16f, z); glVertex3f( halfW - 0.28f, halfH, z);
        glVertex3f(-halfW, -halfH + 0.16f, z); glVertex3f(-halfW + 0.28f, -halfH, z);
        glVertex3f( halfW, -halfH + 0.16f, z); glVertex3f( halfW - 0.28f, -halfH, z);
    glEnd();
    glEnable(GL_LIGHTING);
}

static void drawModulePanelBase(float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex3f(-1.1f,-1.1f,0.0f);
        glVertex3f( 1.1f,-1.1f,0.0f);
        glVertex3f( 1.1f, 1.1f,0.0f);
        glVertex3f(-1.1f, 1.1f,0.0f);
    glEnd();
    drawPanelBorder(1.1f, 1.1f, 0.012f, 0.635f, 0.482f, 0.361f);
}

static void updateManualBookClickArea()
{
    GLdouble model[16], proj[16];
    GLint viewport[4];
    GLdouble sx, sy, sz;
    float corners[4][3] = {
        {4.4f, 1.5f, 6.25f},
        {6.2f, 1.5f, 6.25f},
        {6.2f, 3.1f, 6.25f},
        {4.4f, 3.1f, 6.25f}
    };

    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, viewport);

    manualBookClickReady = false;
    int minX = w, minY = h, maxX = 0, maxY = 0;
    for (int i = 0; i < 4; i++) {
        if (gluProject(corners[i][0], corners[i][1], corners[i][2],
                       model, proj, viewport, &sx, &sy, &sz) == GL_TRUE && sz >= 0.0 && sz <= 1.0) {
            if ((int)sx < minX) minX = (int)sx;
            if ((int)sx > maxX) maxX = (int)sx;
            if ((int)sy < minY) minY = (int)sy;
            if ((int)sy > maxY) maxY = (int)sy;
            manualBookClickReady = true;
        }
    }

    if (manualBookClickReady) {
        manualBookClickX1 = minX;
        manualBookClickY1 = minY;
        manualBookClickX2 = maxX;
        manualBookClickY2 = maxY;
    }
}

static void drawTexturedQuad(float x1, float y1, float z1,
                             float x2, float y2, float z2,
                             float x3, float y3, float z3,
                             float x4, float y4, float z4,
                             float repeatS, float repeatT)
{
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,    0.0f);    glVertex3f(x1, y1, z1);
        glTexCoord2f(repeatS, 0.0f);    glVertex3f(x2, y2, z2);
        glTexCoord2f(repeatS, repeatT); glVertex3f(x3, y3, z3);
        glTexCoord2f(0.0f,    repeatT); glVertex3f(x4, y4, z4);
    glEnd();
}

static void drawVaultRoomShell()
{
    beginTexture(wallTextureID);
    glColor3f(0.863f, 0.843f, 0.788f);

    glNormal3f(1.0f, 0.0f, 0.0f);
    drawTexturedQuad(-20.0f, 0.0f, -10.0f, -20.0f, 0.0f, 20.0f,
                     -20.0f, 14.0f, 20.0f, -20.0f, 14.0f, -10.0f, 6.0f, 3.0f);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    drawTexturedQuad(20.0f, 0.0f, 20.0f, 20.0f, 0.0f, -10.0f,
                     20.0f, 14.0f, -10.0f, 20.0f, 14.0f, 20.0f, 6.0f, 3.0f);

    glNormal3f(0.0f, 0.0f, 1.0f);
    drawTexturedQuad(-20.0f, 0.0f, -10.0f, 20.0f, 0.0f, -10.0f,
                     20.0f, 14.0f, -10.0f, -20.0f, 14.0f, -10.0f, 8.0f, 3.0f);

    glNormal3f(0.0f, -1.0f, 0.0f);
    drawTexturedQuad(-20.0f, 14.0f, -10.0f, 20.0f, 14.0f, -10.0f,
                     20.0f, 14.0f, 20.0f, -20.0f, 14.0f, 20.0f, 8.0f, 6.0f);

    endSteelTexture();

    beginTexture(floorTextureID);
    glColor3f(0.863f, 0.843f, 0.788f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    drawTexturedQuad(-20.0f, 0.0f, 20.0f, 20.0f, 0.0f, 20.0f,
                     20.0f, 0.0f, -10.0f, -20.0f, 0.0f, -10.0f, 14.0f, 10.0f);
    endSteelTexture();
}

static void drawVaultShadow()
{
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glColor4f(0.0f, 0.0f, 0.0f, 0.34f);
        glBegin(GL_QUADS);
            glVertex3f(-10.8f, 0.012f, 2.2f);
            glVertex3f( 11.0f, 0.012f, 2.4f);
            glVertex3f( 12.5f, 0.012f, 7.8f);
            glVertex3f( -9.4f, 0.012f, 7.6f);
        glEnd();

        glColor4f(0.0f, 0.0f, 0.0f, 0.18f);
        glBegin(GL_QUADS);
            glVertex3f(-10.7f, 0.2f, 2.96f);
            glVertex3f( 10.7f, 0.2f, 2.96f);
            glVertex3f( 11.8f, 5.8f, 2.96f);
            glVertex3f( -9.6f, 5.8f, 2.96f);
        glEnd();

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

static void drawGoldBars()
{
    GLfloat goldSpec[] = {0.95f, 0.78f, 0.24f, 1.0f};
    GLfloat goldShine[] = {70.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, goldSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, goldShine);

    glPushMatrix();
        glTranslatef(0.0f, 1.05f, 5.72f);
        for (int layer = 0; layer < 3; layer++) {
            int count = 5 - layer * 2;
            for (int i = 0; i < count; i++) {
                glPushMatrix();
                    glColor3f(1.0f, 0.84f, 0.0f);
                    glTranslatef((i - (count - 1) * 0.5f) * 0.72f,
                                 layer * 0.32f,
                                 -0.12f * layer);
                    glScalef(0.62f, 0.26f, 0.36f);
                    glutSolidCube(1.0f);
                glPopMatrix();
            }
        }
    glPopMatrix();

    GLfloat defaultSpec[] = {0.15f, 0.14f, 0.12f, 1.0f};
    GLfloat defaultShine[] = {20.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, defaultShine);
}

static void drawVaultDoor()
{
    if (cekSemuaModulSelesai() && doorAngle < 96.0f)
        doorAngle += 0.55f;
    if (!cekSemuaModulSelesai() && doorAngle > 0.0f)
        doorAngle -= 1.5f;

    if (doorAngle > 12.0f)
        drawGoldBars();

    glPushMatrix();
        glTranslatef(-2.55f, 3.0f, 6.28f);
        glRotatef(-doorAngle, 0.0f, 1.0f, 0.0f);
        glTranslatef(2.55f, 0.0f, 0.0f);

        glPushMatrix(); Circle(2.5f, 0.3412f, 0.349f, 0.3569f, 360); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.08f); Circle(2.2f, 0.91f, 0.80f, 0.38f, 360); glPopMatrix();
        glPushMatrix();
            glColor3f(0.3412f, 0.349f, 0.3569f);
            glTranslatef(-2.0f, -0.3f, 0.16f);
            glBegin(GL_QUADS);
                glVertex3f(0.0f,0.0f,0.0f);
                glVertex3f(1.4f,0.0f,0.0f);
                glVertex3f(1.4f,0.4f,0.0f);
                glVertex3f(0.0f,0.4f,0.0f);
            glEnd();
        glPopMatrix();
    glPopMatrix();
}

 
// RENDER UTAMA BRANKAS + 7 PUZZLEs
void brankas()
{
    // --- Pastikan dinding & brankas tidak memancarkan cahaya sendiri ---
    GLfloat mat_no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_no_emission);

    drawVaultRoomShell();
    drawVaultShadow();

    // --- BRANKAS DEPAN dengan bukaan pintu ---
    glPushMatrix(); glColor3f(0.38f, 0.40f, 0.42f); glTranslatef(0,0,6);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(-10.0f,0.0f,0.0f); glVertex3f(-3.0f,0.0f,0.0f); glVertex3f(-3.0f,7.0f,0.0f); glVertex3f(-10.0f,7.0f,0.0f);
        glVertex3f( 3.0f,0.0f,0.0f); glVertex3f(10.0f,0.0f,0.0f); glVertex3f(10.0f,7.0f,0.0f); glVertex3f( 3.0f,7.0f,0.0f);
        glVertex3f(-3.0f,5.8f,0.0f); glVertex3f( 3.0f,5.8f,0.0f); glVertex3f( 3.0f,7.0f,0.0f); glVertex3f(-3.0f,7.0f,0.0f);
        glVertex3f(-3.0f,0.0f,0.0f); glVertex3f( 3.0f,0.0f,0.0f); glVertex3f( 3.0f,0.6f,0.0f); glVertex3f(-3.0f,0.6f,0.0f);
    glEnd();
    glPopMatrix();

    // --- BRANKAS BELAKANG ---
    glPushMatrix(); glColor3f(0.34f, 0.36f, 0.38f); glTranslatef(0,0,3);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_QUADS); glVertex3f(-10.0f,0.0f,0.0f); glVertex3f(10.0f,0.0f,0.0f); glVertex3f(10.0f,7.0f,0.0f); glVertex3f(-10.0f,7.0f,0.0f); glEnd();
    glPopMatrix();

    // --- BRANKAS KANAN ---
    glPushMatrix(); glColor3f(0.36f, 0.38f, 0.40f); glTranslatef(10,0,6);
    glPushMatrix(); glRotated(-90.0, 0, 1, 0);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS); glVertex3f(-3.0f,0.0f,0.0f); glVertex3f(0.0f,0.0f,0.0f); glVertex3f(0.0f,7.0f,0.0f); glVertex3f(-3.0f,7.0f,0.0f); glEnd(); glPopMatrix();
    glPopMatrix();

    // --- BRANKAS KIRI ---
    glPushMatrix(); glColor3f(0.36f, 0.38f, 0.40f); glTranslatef(-10,0,6);
    glPushMatrix(); glRotated(-90.0, 0, 1, 0);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_QUADS); glVertex3f(-3.0f,0.0f,0.0f); glVertex3f(0.0f,0.0f,0.0f); glVertex3f(0.0f,7.0f,0.0f); glVertex3f(-3.0f,7.0f,0.0f); glEnd(); glPopMatrix();
    glPopMatrix();

    // --- ATAP BRANKAS ---
    glPushMatrix(); glColor3f(0.42f, 0.44f, 0.46f); glTranslatef(0.0,7.0,0.0);
    glPushMatrix(); glRotated(90.0, 1, 0, 0);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_QUADS); glVertex3f(-10.0f,0.0f,0.0f); glVertex3f(10.0f,6.0f,0.0f); glVertex3f(10.0f,6.0f,0.0f); glVertex3f(-10.0f,0.0f,0.0f); glEnd(); glPopMatrix();
    glPopMatrix();

    // Dinding, atap, dan lantai ruangan digambar oleh drawVaultRoomShell()
    // agar tekstur baja terlihat konsisten.

    // --- JAM ANALOG ---
    glPushMatrix(); glTranslatef(-19.8f, 7.5f, 5.0f); glRotated(90.0, 0.0, 1.0, 0.0);
    drawAnalogClock(360.0f, (float)timeLeft);
    glPopMatrix();

    // --- LAMPU INDIKATOR ATAS ---
    glPushMatrix();
        glColor3f(lightR, lightG, lightB);
        glTranslatef(0.0,5.8,6.1);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS); glVertex3f(-2.5f,0.0f,0.0f); glVertex3f(2.5f,0.0f,0.0f); glVertex3f(2.5f,0.7f,0.0f); glVertex3f(-2.5f,0.7f,0.0f); glEnd();
    glPopMatrix();

    // --- STATUS BAR 7 PUZZLE ---
    drawPuzzleStatusBar();

    // --- PINTU BUNDAR EMAS TENGAH ---
    drawVaultDoor();

    // NYALAKAN EFEK TERANG KHUSUS UNTUK PUZZLE SAJA
    glNormal3f(0.0f, 0.0f, 1.0f);
    glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);

    // --- PANEL BACKGROUND ABU KANAN & KIRI ---
    glPushMatrix(); glColor3f(0.172f, 0.224f, 0.188f); glTranslatef(6.5,3.5,6.1);
    glBegin(GL_QUADS); glVertex3f(-2.5f,-2.5f,0.0f); glVertex3f(2.5f,-2.5f,0.0f); glVertex3f(2.5f,2.5f,0.0f); glVertex3f(-2.5f,2.5f,0.0f); glEnd(); glPopMatrix();

    glPushMatrix(); glColor3f(0.172f, 0.224f, 0.188f); glTranslatef(-6.5,3.5,6.1);
    glBegin(GL_QUADS); glVertex3f(-2.5f,-2.5f,0.0f); glVertex3f(2.5f,-2.5f,0.0f); glVertex3f(2.5f,2.5f,0.0f); glVertex3f(-2.5f,2.5f,0.0f); glEnd(); glPopMatrix();

    // --- PANEL KANAN ---
    glPushMatrix();
        glTranslatef(5.3f, 2.3f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.635f, 0.482f, 0.361f);
        glBegin(GL_QUADS); glVertex3f(-0.9f,-0.8f,0.01f); glVertex3f(0.9f,-0.8f,0.01f); glVertex3f(0.9f,0.8f,0.01f); glVertex3f(-0.9f,0.8f,0.01f); glEnd();
        drawPanelBorder(0.9f, 0.8f, 0.025f, 0.863f, 0.843f, 0.788f);
        glColor3f(0.172f, 0.224f, 0.188f); glLineWidth(2.5f);
        drawText3D("MANUAL", -0.8f, 0.15f, 0.02f, 0.0025f);
        drawText3D("BOOK",   -0.55f,-0.35f, 0.02f, 0.0025f);
    glPopMatrix();
    updateManualBookClickArea();

    glPushMatrix();
        glTranslatef(7.7f, 2.3f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f, 0.843f, 0.788f); glLineWidth(2.0f);
        drawText3D("COLOR", -0.7f, 0.82f, 0.01f, 0.002f);
        drawText3D("6", 0.4f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.85f, 0.1f, 0.1f); glTranslatef(-0.52f, 0.4f, 0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.45f,-0.28f,0.0f); glVertex3f(0.45f,-0.28f,0.0f); glVertex3f(0.45f,0.28f,0.0f); glVertex3f(-0.45f,0.28f,0.0f); glEnd();
            glColor3f(1.0f,1.0f,1.0f); drawText3D("RED", -0.32f,-0.1f,0.01f,0.002f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.1f, 0.8f, 0.1f); glTranslatef(0.52f, 0.4f, 0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.45f,-0.28f,0.0f); glVertex3f(0.45f,-0.28f,0.0f); glVertex3f(0.45f,0.28f,0.0f); glVertex3f(-0.45f,0.28f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D("GRN", -0.35f,-0.1f,0.01f,0.002f);
        glPopMatrix();
        drawAnswerBox(0.0f, -0.5f, 0.01f, 6, (activePuzzle==6 && !isGameOver && !isGameWon), puzzleDone[6]);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(7.7f, 4.7f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f, 0.843f, 0.788f); glLineWidth(2.0f);
        drawText3D("WIRE", -0.55f, 0.82f, 0.01f, 0.002f);
        drawText3D("5", 0.45f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.0f, 0.0f, 0.0f); glTranslatef(0.0f, 0.0f, 0.01f);
            glBegin(GL_QUADS);
                glVertex3f(-0.85f,-0.8f,0.0f); glVertex3f(-0.75f,-0.8f,0.0f); glVertex3f(-0.75f,0.65f,0.0f); glVertex3f(-0.85f,0.65f,0.0f);
                glVertex3f( 0.75f,-0.8f,0.0f); glVertex3f( 0.85f,-0.8f,0.0f); glVertex3f( 0.85f,0.65f,0.0f); glVertex3f( 0.75f,0.65f,0.0f);
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.0f,0.1f,0.9f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,0.45f,0.0f); glVertex3f(-0.1f,0.45f,0.0f); glVertex3f(-0.1f,0.6f,0.0f); glVertex3f(-0.75f,0.6f,0.0f);
            glVertex3f(0.1f,0.45f,0.0f); glVertex3f(0.75f,0.45f,0.0f); glVertex3f(0.75f,0.6f,0.0f); glVertex3f(0.1f,0.6f,0.0f); glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.9f,0.1f,0.1f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,0.05f,0.0f); glVertex3f(-0.1f,0.05f,0.0f); glVertex3f(-0.1f,0.2f,0.0f); glVertex3f(-0.75f,0.2f,0.0f);
            glVertex3f(0.1f,0.05f,0.0f); glVertex3f(0.75f,0.05f,0.0f); glVertex3f(0.75f,0.2f,0.0f); glVertex3f(0.1f,0.2f,0.0f); glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.2f,1.0f,0.2f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,-0.35f,0.0f); glVertex3f(-0.1f,-0.35f,0.0f); glVertex3f(-0.1f,-0.2f,0.0f); glVertex3f(-0.75f,-0.2f,0.0f);
            glVertex3f(0.1f,-0.35f,0.0f); glVertex3f(0.75f,-0.35f,0.0f); glVertex3f(0.75f,-0.2f,0.0f); glVertex3f(0.1f,-0.2f,0.0f); glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(1.0f,0.8f,0.0f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,-0.75f,0.0f); glVertex3f(-0.1f,-0.75f,0.0f); glVertex3f(-0.1f,-0.6f,0.0f); glVertex3f(-0.75f,-0.6f,0.0f);
            glVertex3f(0.1f,-0.75f,0.0f); glVertex3f(0.75f,-0.75f,0.0f); glVertex3f(0.75f,-0.6f,0.0f); glVertex3f(0.1f,-0.6f,0.0f); glEnd();
        glPopMatrix();
        drawAnswerBox(0.0f, -0.92f, 0.01f, 5, (activePuzzle==5 && !isGameOver && !isGameWon), puzzleDone[5]);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.3f, 4.7f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f, 0.843f, 0.788f); glLineWidth(2.0f);
        drawText3D("NUMS", -0.55f, 0.82f, 0.01f, 0.002f);
        drawText3D("7", 0.45f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(-0.52f,0.45f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.42f,-0.3f,0.0f); glVertex3f(0.42f,-0.3f,0.0f); glVertex3f(0.42f,0.3f,0.0f); glVertex3f(-0.42f,0.3f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D("38", -0.25f,-0.12f,0.01f,0.003f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(0.52f,0.45f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.42f,-0.3f,0.0f); glVertex3f(0.42f,-0.3f,0.0f); glVertex3f(0.42f,0.3f,0.0f); glVertex3f(-0.42f,0.3f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D("67", -0.25f,-0.12f,0.01f,0.003f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(-0.52f,-0.1f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.42f,-0.3f,0.0f); glVertex3f(0.42f,-0.3f,0.0f); glVertex3f(0.42f,0.3f,0.0f); glVertex3f(-0.42f,0.3f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D("10", -0.25f,-0.12f,0.01f,0.003f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(0.52f,-0.1f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.42f,-0.3f,0.0f); glVertex3f(0.42f,-0.3f,0.0f); glVertex3f(0.42f,0.3f,0.0f); glVertex3f(-0.42f,0.3f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D("24", -0.25f,-0.12f,0.01f,0.003f);
        glPopMatrix();
        drawAnswerBox(0.0f, -0.75f, 0.01f, 7, (activePuzzle==7 && !isGameOver && !isGameWon), puzzleDone[7]);
    glPopMatrix();

    // --- PANEL KIRI ---
    glPushMatrix();
        glTranslatef(-7.7f, 2.3f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f,0.843f,0.788f); drawText3D("WIRE", -0.55f, 0.82f, 0.01f, 0.002f);
        drawText3D("1", 0.45f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.0f,0.0f,0.0f); glTranslatef(0.0f,0.0f,0.01f);
            glBegin(GL_QUADS);
                glVertex3f(-0.85f,-0.8f,0.0f); glVertex3f(-0.75f,-0.8f,0.0f); glVertex3f(-0.75f,0.65f,0.0f); glVertex3f(-0.85f,0.65f,0.0f);
                glVertex3f( 0.75f,-0.8f,0.0f); glVertex3f( 0.85f,-0.8f,0.0f); glVertex3f( 0.85f,0.65f,0.0f); glVertex3f( 0.75f,0.65f,0.0f);
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.9f,0.1f,0.1f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,0.45f,0.0f); glVertex3f(-0.1f,0.45f,0.0f); glVertex3f(-0.1f,0.6f,0.0f); glVertex3f(-0.75f,0.6f,0.0f);
            glVertex3f(0.1f,0.45f,0.0f); glVertex3f(0.75f,0.45f,0.0f); glVertex3f(0.75f,0.6f,0.0f); glVertex3f(0.1f,0.6f,0.0f); glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.0f,0.1f,0.6f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,-0.1f,0.0f); glVertex3f(-0.1f,-0.1f,0.0f); glVertex3f(-0.1f,0.05f,0.0f); glVertex3f(-0.75f,0.05f,0.0f);
            glVertex3f(0.1f,-0.1f,0.0f); glVertex3f(0.75f,-0.1f,0.0f); glVertex3f(0.75f,0.05f,0.0f); glVertex3f(0.1f,0.05f,0.0f); glEnd();
        glPopMatrix();
        glPushMatrix();
            glColor3f(1.0f,0.8f,0.0f); glTranslatef(0.0f,0.0f,0.02f);
            glBegin(GL_QUADS); glVertex3f(-0.75f,-0.65f,0.0f); glVertex3f(-0.1f,-0.65f,0.0f); glVertex3f(-0.1f,-0.5f,0.0f); glVertex3f(-0.75f,-0.5f,0.0f);
            glVertex3f(0.1f,-0.65f,0.0f); glVertex3f(0.75f,-0.65f,0.0f); glVertex3f(0.75f,-0.5f,0.0f); glVertex3f(0.1f,-0.5f,0.0f); glEnd();
        glPopMatrix();
        drawAnswerBox(0.0f, -0.88f, 0.01f, 1, (activePuzzle==1 && !isGameOver && !isGameWon), puzzleDone[1]);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-5.3f, 2.3f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f, 0.843f, 0.788f);
        drawText3D("COLOR", -0.7f, 0.82f, 0.01f, 0.002f);
        drawText3D("3", 0.4f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.6f, 0.1f, 0.8f); glTranslatef(-0.52f, 0.35f, 0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.45f,-0.28f,0.0f); glVertex3f(0.45f,-0.28f,0.0f); glVertex3f(0.45f, 0.28f,0.0f); glVertex3f(-0.45f, 0.28f,0.0f); glEnd();
            glColor3f(1.0f, 1.0f, 1.0f); drawText3D("PUR", -0.32f, -0.1f, 0.01f, 0.002f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.1f, 0.8f, 0.1f); glTranslatef(0.52f, 0.35f, 0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.45f,-0.28f,0.0f); glVertex3f(0.45f,-0.28f,0.0f); glVertex3f(0.45f, 0.28f,0.0f); glVertex3f(-0.45f, 0.28f,0.0f); glEnd();
            glColor3f(0.0f, 0.0f, 0.0f); drawText3D("GRN", -0.35f, -0.1f, 0.01f, 0.002f);
        glPopMatrix();
        drawAnswerBox(0.0f, -0.55f, 0.01f, 3, (activePuzzle==3 && !isGameOver && !isGameWon), puzzleDone[3]);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-7.7f, 4.7f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f,0.843f,0.788f); drawText3D("ALPHA", -0.65f, 0.82f, 0.01f, 0.002f);
        drawText3D("4", 0.55f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(0.0f,0.35f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.9f,-0.35f,0.0f); glVertex3f(0.9f,-0.35f,0.0f); glVertex3f(0.9f,0.35f,0.0f); glVertex3f(-0.9f,0.35f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D("FE GC", -0.75f,-0.12f,0.01f,0.003f);
        glPopMatrix();
        drawAnswerBox(0.0f, -0.55f, 0.01f, 4, (activePuzzle==4 && !isGameOver && !isGameWon), puzzleDone[4]);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-5.3f, 4.7f, 6.2f);
        drawModulePanelBase(0.247f, 0.310f, 0.267f);
        glColor3f(0.863f,0.843f,0.788f); drawText3D("NUM", -0.48f, 0.82f, 0.01f, 0.002f);
        drawText3D("2", 0.3f, 0.82f, 0.01f, 0.002f);
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(0.0f,0.45f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.9f,-0.22f,0.0f); glVertex3f(0.9f,-0.22f,0.0f); glVertex3f(0.9f,0.22f,0.0f); glVertex3f(-0.9f,0.22f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D(" 73  65 ", -0.9f,-0.1f,0.01f,0.0022f);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.7f,0.7f,0.7f); glTranslatef(0.0f,0.0f,0.01f);
            glBegin(GL_QUADS); glVertex3f(-0.9f,-0.22f,0.0f); glVertex3f(0.9f,-0.22f,0.0f); glVertex3f(0.9f,0.22f,0.0f); glVertex3f(-0.9f,0.22f,0.0f); glEnd();
            glColor3f(0.0f,0.0f,0.0f); drawText3D(" 65  73 ", -0.9f,-0.1f,0.01f,0.0022f);
        glPopMatrix();
        drawAnswerBox(0.0f, -0.55f, 0.01f, 2, (activePuzzle==2 && !isGameOver && !isGameWon), puzzleDone[2]);
    glPopMatrix();

    // >>> MATIKAN EFEK TERANG AGAR UI/TEKS TETAP NORMAL <<<
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_no_emission);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Beam cahaya dari langit-langit ke brankas
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTranslatef(0.0f, 13.5f, 4.5f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

        glColor4f(1.0f, 0.95f, 0.75f, 0.1f);
        GLUquadric* q1 = gluNewQuadric();
        gluCylinder(q1, 0.1f, 7.0f, 13.5f, 32, 1);
        gluDeleteQuadric(q1);

        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    glPopMatrix();

     
    // OVERLAY 2D: HINT, TIMER, FEEDBACK, GAME STATUS
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST); glDisable(GL_LIGHTING);

    {
        glColor4f(0.172f, 0.224f, 0.188f, 0.92f);
        glBegin(GL_QUADS);
            glVertex2f(0,0); glVertex2f(w,0);
            glVertex2f(w,40); glVertex2f(0,40);
        glEnd();
        glColor4f(0.635f, 0.482f, 0.361f, 0.75f);
        glBegin(GL_LINES); glVertex2f(0,40); glVertex2f(w,40); glEnd();

        if (activePuzzle == 0) {
            drawText2D(w/2 - 300.0f, 14, "Arrow Keys: Look Around  |  W/A/S/D: Walk  |  [1-7]: Puzzle  |  [M]: Manual", 0.863f, 0.843f, 0.788f);
        } else {
            const char* pName[] = {"","WIRE KIRI","NUMBER KIRI","COLOUR KIRI","ALPHA","WIRE KANAN","COLOUR KANAN","NUMBER KANAN"};
            char activeInfo[64];
            sprintf(activeInfo, "Puzzle %d  |  %s  |  [Enter] Submit   [Esc] Batal   [Backspace] Hapus", activePuzzle, pName[activePuzzle]);
            drawText2D(12, 14, activeInfo, 0.863f, 0.843f, 0.788f);
        }
    }

    {
        glColor4f(0.172f, 0.224f, 0.188f, 0.88f);
        glBegin(GL_QUADS);
            glVertex2f(w-110, h-36); glVertex2f(w,    h-36);
            glVertex2f(w,    h);     glVertex2f(w-110, h);
        glEnd();
        drawText2D(w-100.0f, h-20, "HP:", 0.863f, 0.843f, 0.788f);
        for (int li = 0; li < 3; li++) {
            float hx = (w - 68.0f) + li * 22.0f;
            if (li < livesLeft) glColor3f(0.635f, 0.482f, 0.361f);
            else                glColor3f(0.247f, 0.310f, 0.267f);
            glBegin(GL_QUADS); glVertex2f(hx, h-28); glVertex2f(hx+16, h-28); glVertex2f(hx+16, h-8);  glVertex2f(hx, h-8); glEnd();
        }
    }

    if (feedbackTimer > 0 && feedbackMsg[0] != '\0') {
        bool isBenar = (strstr(feedbackMsg, "BENAR") != NULL);
        glColor4f(0.247f, 0.310f, 0.267f, 0.92f);
        glBegin(GL_QUADS); glVertex2f(w/2-200, h/2+5);  glVertex2f(w/2+200, h/2+5); glVertex2f(w/2+200, h/2+45); glVertex2f(w/2-200, h/2+45); glEnd();
        glColor4f(isBenar ? 0.635f : 0.50f, isBenar ? 0.482f : 0.22f, isBenar ? 0.361f : 0.18f, 0.95f);
        glBegin(GL_LINE_LOOP); glVertex2f(w/2-200, h/2+5);  glVertex2f(w/2+200, h/2+5); glVertex2f(w/2+200, h/2+45); glVertex2f(w/2-200, h/2+45); glEnd();
        drawText2D(w/2 - 180.0f, h/2 + 30.0f, feedbackMsg, 0.863f, 0.843f, 0.788f);
    }

    if (isGameWon) {
        glColor4f(0.172f, 0.224f, 0.188f, 0.93f);
        glBegin(GL_QUADS); glVertex2f(w/2-240,h/2-40); glVertex2f(w/2+240,h/2-40); glVertex2f(w/2+240,h/2+40); glVertex2f(w/2-240,h/2+40); glEnd();
        glColor4f(0.635f, 0.482f, 0.361f, 1.0f);
        glBegin(GL_LINE_LOOP); glVertex2f(w/2-240,h/2-40); glVertex2f(w/2+240,h/2-40); glVertex2f(w/2+240,h/2+40); glVertex2f(w/2-240,h/2+40); glEnd();
        drawText2D(w/2-210.0f, h/2+15.0f, "SEMUA PUZZLE SELESAI!  PINTU BRANKAS TERBUKA!", 0.863f, 0.843f, 0.788f);
        drawText2D(w/2-100.0f, h/2-15.0f, "Tekan [R] untuk main lagi", 0.863f, 0.843f, 0.788f);
    } else if (isGameOver) {
        glColor4f(0.172f, 0.080f, 0.070f, 0.93f);
        glBegin(GL_QUADS); glVertex2f(w/2-200,h/2-40); glVertex2f(w/2+200,h/2-40); glVertex2f(w/2+200,h/2+40); glVertex2f(w/2-200,h/2+40); glEnd();
        glColor4f(0.635f, 0.482f, 0.361f, 1.0f);
        glBegin(GL_LINE_LOOP); glVertex2f(w/2-200,h/2-40); glVertex2f(w/2+200,h/2-40); glVertex2f(w/2+200,h/2+40); glVertex2f(w/2-200,h/2+40); glEnd();
        const char* failMsg = (timeLeft <= 0) ? "WAKTU HABIS!  GAME OVER" : "NYAWA HABIS!  GAME OVER";
        drawText2D(w/2-150.0f, h/2+15.0f, failMsg, 0.863f, 0.843f, 0.788f);
        drawText2D(w/2-100.0f, h/2-15.0f, "Tekan [R] untuk main lagi", 0.863f, 0.843f, 0.788f);
    }

    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}
