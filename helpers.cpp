#include "helpers.h"
#include "globals.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// HELPER: LINGKARAN
void Circle(float radius, float r, float g, float b, int t)
{
    glColor3f(r, g, b);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= t; i++) {
            float sudut = i * 3.14159f / 180.0f;
            glVertex2f(cos(sudut)*radius, sin(sudut)*radius);
        }
    glEnd();
}

// HELPER: TEKS 3D
void drawText3D(const char* text, float tx, float ty, float tz, float scale)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(tx, ty, tz);
    glScalef(scale, scale, scale);
    for (int i = 0; text[i] != '\0'; i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// HELPER: TEKS 2D
void drawText2D(float sx, float sy, const char* text, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(sx, sy);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// HELPER: KOTAK INPUT
void drawAnswerBox(float bx, float by, float bz,
                   int puzzleIdx, bool isActive, bool isDone)
{
    if (isDone)        glColor3f(0.320f, 0.400f, 0.340f);
    else if (isActive) glColor3f(0.635f, 0.482f, 0.361f);
    else               glColor3f(0.172f, 0.224f, 0.188f);

    glBegin(GL_QUADS);
        glVertex3f(bx-0.75f, by-0.3f, bz);
        glVertex3f(bx+0.75f, by-0.3f, bz);
        glVertex3f(bx+0.75f, by+0.3f, bz);
        glVertex3f(bx-0.75f, by+0.3f, bz);
    glEnd();

    if (isDone) {
        glColor3f(0.863f, 0.843f, 0.788f);
        drawText3D("OK", bx-0.15f, by-0.1f, bz+0.01f, 0.003f);
    } else {
        glColor3f(0.863f, 0.843f, 0.788f);
        char buf[32];
        strcpy(buf, puzzleInput[puzzleIdx]);
        if (isActive && (glutGet(GLUT_ELAPSED_TIME)/400) % 2 == 0)
            strcat(buf, "_");
        drawText3D(buf, bx-0.65f, by-0.1f, bz+0.01f, 0.0025f);
    }
}

// JAM ANALOG
void drawAnalogClock(float totalTimeSecs, float timeRemainingSecs)
{
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glNormal3f(0.0f, 0.0f, 1.0f);

        glPushMatrix(); Circle(3.2f, 0.05f, 0.05f, 0.05f, 360); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.05f); Circle(3.0f, 1.0f, 1.0f, 1.0f, 360); glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.10f); glLineWidth(2.0f); glBegin(GL_LINES);
            for (int i = 0; i < 60; i++) {
                float aRad = (i * 6.0f) * 3.14159f / 180.0f;
                float rOuter = 2.9f, rInner = (i % 12 == 0) ? 2.4f : 2.75f;
                if (i % 12 == 0) glColor3f(0.0f, 0.0f, 0.0f); else glColor3f(0.6f, 0.6f, 0.6f);
                glVertex2f(cos(aRad)*rInner, sin(aRad)*rInner);
                glVertex2f(cos(aRad)*rOuter, sin(aRad)*rOuter);
            }
            glEnd();
        glPopMatrix();

        float timeElapsedSecs  = totalTimeSecs - timeRemainingSecs;
        float elapsedAngleDeg  = (timeElapsedSecs / totalTimeSecs) * 360.0f;
        if (elapsedAngleDeg > 360.0f) elapsedAngleDeg = 360.0f;

        glColor3f(0.7f, 0.95f, 0.7f);
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.12f); glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);
            int arcSegments = (int)(360.0f - elapsedAngleDeg);
            for (int s = 0; s <= arcSegments; s++) {
                float curA = elapsedAngleDeg + (float)s;
                float aRad = (90.0f - curA) * 3.14159f / 180.0f;
                glVertex2f(cos(aRad)*2.4f, sin(aRad)*2.4f);
            }
            glEnd();
        glPopMatrix();

        glColor3f(0.0f, 0.0f, 0.0f); glLineWidth(3.0f);
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
            float angleDeg = 90.0f - (i * 72.0f);
            float aRad     = angleDeg * 3.14159f / 180.0f;
            float numX = cos(aRad)*2.0f, numY = sin(aRad)*2.0f;
            char numStr[4];
            if (i == 0) sprintf(numStr, (timeRemainingSecs > 0) ? "5" : "0");
            else        sprintf(numStr, "%d", 5-i);
            float scale = 0.01f, textWidth = 0;
            for (int c = 0; numStr[c] != '\0'; c++)
                textWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, numStr[c]) * scale;
            glTranslatef(numX - textWidth/2.0f, numY - 0.12f, 0.15f);
            glScalef(scale, scale, scale);
            for (int c = 0; numStr[c] != '\0'; c++)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, numStr[c]);
            glPopMatrix();
        }

        glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.18f); Circle(0.18f, 0.0f, 0.0f, 0.0f, 360); glPopMatrix();

        float secAngleDeg = timeElapsedSecs * 6.0f;
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.20f); glLineWidth(5.0f);
            glPushMatrix();
                glRotatef(-elapsedAngleDeg, 0.0f, 0.0f, 1.0f); glColor3f(1.0f, 0.5f, 0.0f);
                glBegin(GL_LINES); glVertex2f(0.0f, -0.3f); glVertex2f(0.0f, 2.1f); glEnd();
            glPopMatrix();
            glLineWidth(2.0f);
            glPushMatrix();
                glRotatef(-secAngleDeg, 0.0f, 0.0f, 1.0f); glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_LINES); glVertex2f(0.0f, -0.3f); glVertex2f(0.0f, 2.5f); glEnd();
            glPopMatrix();
        glPopMatrix();

        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    glPopMatrix();
}
