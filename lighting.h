#ifndef LIGHTING_H
#define LIGHTING_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

 
// VARIABEL PENCAHAYAAN (extern untuk akses lintas file)
extern const GLfloat light0_ambient[];
extern const GLfloat light0_diffuse[];
extern const GLfloat light0_specular[];
extern const GLfloat light_position1[];

extern const GLfloat light1_ambient[];
extern const GLfloat light1_diffuse[];
extern const GLfloat light1_specular[];
extern const GLfloat light_position2[];

extern const GLfloat light2_ambient[];
extern const GLfloat light2_diffuse[];
extern const GLfloat light2_specular[];
extern const GLfloat light_position3[];

extern const GLfloat mat_ambient[];
extern const GLfloat mat_diffuse[];
extern const GLfloat mat_specular[];
extern const GLfloat high_shininess[];

extern const GLfloat spot_position[];
extern const GLfloat spot_direction[];

 
// FUNGSI SETUP PENCAHAYAAN
void lighting();

 
// FUNGSI INISIALISASI OPENGL
void init();

#endif // LIGHTING_H