#include "lighting.h"

 
// VARIABEL PENCAHAYAAN
const GLfloat light0_ambient[]  = {0.045f, 0.058f, 0.049f, 1.0f};
const GLfloat light0_diffuse[]  = {0.635f, 0.482f, 0.361f, 1.0f};
const GLfloat light0_specular[] = {0.22f, 0.18f, 0.13f, 1.0f};
const GLfloat light_position1[] = {4.0f, 12.0f, 8.0f, 1.0f};

const GLfloat light1_ambient[]  = {0.035f, 0.044f, 0.038f, 1.0f};
const GLfloat light1_diffuse[]  = {0.247f, 0.310f, 0.267f, 1.0f};
const GLfloat light1_specular[] = {0.00f, 0.00f, 0.00f, 1.0f};
const GLfloat light_position2[] = {-8.0f, 5.0f, 4.0f, 1.0f};

const GLfloat light2_ambient[]  = {0.040f, 0.034f, 0.026f, 1.0f};
const GLfloat light2_diffuse[]  = {0.635f, 0.482f, 0.361f, 1.0f};
const GLfloat light2_specular[] = {0.35f, 0.28f, 0.20f, 1.0f};
const GLfloat light_position3[] = {0.0f, 14.0f, -8.0f, 1.0f};

const GLfloat mat_ambient[]    = {0.172f, 0.224f, 0.188f, 1.0f};
const GLfloat mat_diffuse[]    = {0.247f, 0.310f, 0.267f, 1.0f};
const GLfloat mat_specular[]   = {0.635f, 0.482f, 0.361f, 1.0f};
const GLfloat high_shininess[] = {20.0f};

// Spotlight dari langit-langit, tepat di atas brankas
const GLfloat spot_position[]  = {0.0f, 13.5f, 4.5f, 1.0f};
const GLfloat spot_direction[] = {0.0f, -1.0f, 0.3f};

 
// FUNGSI SETUP PENCAHAYAAN
void lighting()
{
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Global ambient sangat rendah = ruangan gelap
    GLfloat global_ambient[] = {0.075f, 0.095f, 0.080f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    // LIGHT2: Spotlight kuat dari atas ke brankas
    GLfloat spot_pos[]  = {0.0f, 13.5f, 4.5f, 1.0f};
    GLfloat spot_dir[]  = {0.0f, -1.0f, 0.25f};
    GLfloat spot_diff[] = {0.92f, 0.78f, 0.56f, 1.0f};
    GLfloat spot_amb[]  = {0.02f, 0.02f, 0.015f, 1.0f};
    GLfloat spot_spec[] = {0.75f, 0.58f, 0.42f, 1.0f};

    glLightfv(GL_LIGHT2, GL_POSITION,       spot_pos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_dir);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF,    22.0f);
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT,  50.0f);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,        spot_diff);
    glLightfv(GL_LIGHT2, GL_AMBIENT,        spot_amb);
    glLightfv(GL_LIGHT2, GL_SPECULAR,       spot_spec);
    glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION,  0.3f);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION,    0.02f);
    glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.002f);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

 
// FUNGSI INISIALISASI OPENGL
void init()
{
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.172f, 0.224f, 0.188f, 1.0f);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
}
