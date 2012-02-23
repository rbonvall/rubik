#include <GL/glut.h>
#include <stdio.h>
#include "cube.h"
#include "camera.h"
#include "glext.h"
#include "bitmap.h"

struct cube *rubik;
struct camera *cam;
unsigned int face_texture;

/* Parámetros de control de las rotaciones del cubo */
const int ROTATION_STEPS = 10;
struct {
    int step;
    int slice;
    enum {NO_ROTATION, X_AXIS, Y_AXIS, Z_AXIS} axis;
    enum {BACKWARDS, FORWARDS} direction;
} rotation;

/* Colores, indexados por los posibles valores de las caras del cubo */
float faceColors[][3] = {
    {0.0, 0.6, 0.0},  /* GREEN */
    {1.0, 0.0, 0.0},  /* RED */
    {1.0, 1.0, 0.0},  /* YELLOW */
    {1.0, 1.0, 1.0},  /* WHITE */
    {1.0, 0.6, 0.0},  /* ORANGE */
    {0.0, 0.0, 0.6},  /* BLUE */
};

/* Parámetros de la entrada */
int teclas[128];
int mouseX, mouseY;
int key_being_pressed = 0;

/* Callbacks */
void teclaDown(unsigned char key, int x, int y);
void teclaUp(unsigned char key, int x, int y);
void mouse(int x, int y);
void init(void);
void display(void);
void idle(void);
void reshape(int w, int h);

/* Funciones auxiliares */
void switch_light1();
void switch_light2();
void set_cube_rotation(int i, int j, int k);
void set_color(int condition, float color[]);
void start_rotation(int axis, int direction, int slice);


int main(int argc, char *argv[]) {
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Tarea 5 -- Arevalo, Bonvallet");
    glutDisplayFunc(&display);
    glutReshapeFunc(&reshape);
    glutKeyboardFunc(&teclaDown);
    glutKeyboardUpFunc(&teclaUp);
    glutIdleFunc(&idle);
    glutPassiveMotionFunc(&mouse);

    init();
    glutMainLoop();
    return 0;
}


void init(void) {
    glClearColor (0.2, 0.2, 0.2, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    /* Crear el cubo y la camara */
    rubik = new_random_cube();
    //rubik = new_solved_cube();
    cam = new_camera();

    /* Iluminacion global */
    GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat colorAmbient[]   = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat colorDiffuse[]   = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat colorSpecular[]  = { 1.0, 1.0, 1.0, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, colorAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, colorDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, colorSpecular);

    /* Luz en (4, 4, 4) */
    GLfloat color_light1[]   = { 0.5, 0.4, 0.4, 1.0 };
    GLfloat position_light1[] = {4, 4, 4, 1};
    switch_light1();
    glLightfv(GL_LIGHT1, GL_POSITION, position_light1);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  color_light1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  colorDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, colorSpecular);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);

    /* Luz en -(4, 4, 4) */
    GLfloat color_light2[]   = { 0.4, 0.5, 0.5, 1.0 };
    GLfloat position_light2[] = {-4, -4, -4, 1};
    switch_light2();
    glLightfv(GL_LIGHT2, GL_POSITION, position_light2);
    glLightfv(GL_LIGHT2, GL_AMBIENT,  color_light2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  colorDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, colorSpecular);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.00);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 20.0);

    /* La especularidad y la emision de los objetos son siempre las mismas */
    GLfloat materialSpecular[] = { 0.1, 0.1, 0.1 };
    GLfloat materialEmission[] = { 0.0, 0.0, 0.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);

    /* Cargar textura de las caras de los cubitos */
    BITMAPINFO *info_img;
    glGenTextures(1, &face_texture);
    glBindTexture(GL_TEXTURE_2D, face_texture);
    GLubyte *img = LoadDIBitmap("face.bmp", &info_img);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            info_img->bmiHeader.biWidth, info_img->bmiHeader.biHeight,
            0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, face_texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    free(img);
    free(info_img);

}


/* Funcion set_color:
 * si se cumple la condicion, se activa el color de material dado en el
 * estado actual, y si no se cumple, se activa el color negro */
void set_color(int condition, float color[]) {
    static float black[] = {0.0, 0.0, 0.0, 0.0};
    if (condition) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, color);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    } else {
        glMaterialfv(GL_FRONT, GL_AMBIENT, black);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    }
}


/* Funcion set_cube_rotation:
 * modifica la matriz de rotacion si es que el cubito en la posición (i, j, k)
 * esta en una rebanada del cubo rubik que esta siendo girada */
void set_cube_rotation(int i, int j, int k) {
    float angle;

    angle = 90.0 * (float) rotation.step / (float) ROTATION_STEPS;
    if (rotation.direction == BACKWARDS)
        angle = -angle;

    if      (rotation.axis == X_AXIS && i == rotation.slice)
        glRotatef(angle, 1, 0, 0);
    else if (rotation.axis == Y_AXIS && j == rotation.slice)
        glRotatef(angle, 0, 1, 0);
    else if (rotation.axis == Z_AXIS && k == rotation.slice)
        glRotatef(angle, 0, 0, 1);
}


/* Interruptores para las luces 1 y 2 */
void switch_light1() {
    static int on = 0;
    if (on) glDisable(GL_LIGHT1);
    else    glEnable(GL_LIGHT1);
    on = !on;
}
void switch_light2() {
    static int on = 0;
    if (on) glDisable(GL_LIGHT2);
    else    glEnable(GL_LIGHT2);
    on = !on;
}


/* Callback de despliegue */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    /* La camara siempre mira al origen */
    gluLookAt(camera_x(cam), camera_y(cam), camera_z(cam),  0, 0, 0,  0, 0, 1);

    /* Ubicar la luz */
    //GLfloat posicionLuz[]   = { 0, 20.0, 0, 1.0 };
    //GLfloat direccionSpot[] = { 0, -1.0, 0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direccionSpot);

    /* Dibujar los 27 cubitos que forman el cubo rubik */
    /* Los cubitos estan indexados por las coordenadas i, j, k, que toman
     * valores entre -1 y 1, que es igual a la posicion de sus centros en la
     * escena.  La estructura de datos que representa las caras del cubo esta
     * indexada de 0 a 2, y por eso es necesario sumar 1 al acceder a ella. */
    int i, j, k;
    for (i = -1; i <= 1; ++i)
        for (j = -1; j <= 1; ++j)
            for (k = -1; k <= 1; ++k) {

                glPushMatrix();
                set_cube_rotation(i, j, k);
                glTranslatef(2 * i, 2 * j, 2 * k);

                /* cara izquierda */
                set_color(j == -1, faceColors[rubik->left[i + 1][k + 1]]);
                glNormal3i(0, -1, 0);
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(+1, -1, +1);
                    glTexCoord2i(1, 0); glVertex3i(-1, -1, +1);
                    glTexCoord2i(1, 1); glVertex3i(-1, -1, -1);
                    glTexCoord2i(0, 1); glVertex3i(+1, -1, -1);
                glEnd();

                /* cara derecha */
                set_color(j == 1, faceColors[rubik->right[i + 1][k + 1]]);
                glNormal3i(0, 1, 0);
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(+1, +1, +1);
                    glTexCoord2i(1, 0); glVertex3i(+1, +1, -1);
                    glTexCoord2i(1, 1); glVertex3i(-1, +1, -1);
                    glTexCoord2i(0, 1); glVertex3i(-1, +1, +1);
                glEnd();

                /* cara de arriba */
                set_color(k == 1, faceColors[rubik->top[i + 1][j + 1]]);
                glNormal3i(0, 0, 1);
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(+1, +1, +1);
                    glTexCoord2i(1, 0); glVertex3i(-1, +1, +1);
                    glTexCoord2i(1, 1); glVertex3i(-1, -1, +1);
                    glTexCoord2i(0, 1); glVertex3i(+1, -1, +1);
                glEnd();

                /* cara de abajo */
                set_color(k == -1, faceColors[rubik->bottom[i + 1][j + 1]]);
                glNormal3i(0, 0, -1);
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(+1, +1, -1);
                    glTexCoord2i(1, 0); glVertex3i(+1, -1, -1);
                    glTexCoord2i(1, 1); glVertex3i(-1, -1, -1);
                    glTexCoord2i(0, 1); glVertex3i(-1, +1, -1);
                glEnd();

                /* cara del frente */
                set_color(i == 1, faceColors[rubik->front[j + 1][k + 1]]);
                glNormal3i(1, 0, 0);
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(+1, +1, +1);
                    glTexCoord2i(1, 0); glVertex3i(+1, -1, +1);
                    glTexCoord2i(1, 1); glVertex3i(+1, -1, -1);
                    glTexCoord2i(0, 1); glVertex3i(+1, +1, -1);
                glEnd();

                /* cara de atras */
                set_color(i == -1, faceColors[rubik->back[j + 1][k + 1]]);
                glNormal3i(-1, 0, 0);
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(-1, +1, +1);
                    glTexCoord2i(1, 0); glVertex3i(-1, +1, -1);
                    glTexCoord2i(1, 1); glVertex3i(-1, -1, -1);
                    glTexCoord2i(0, 1); glVertex3i(-1, -1, +1);
                glEnd();

                glPopMatrix();
            }

    glutSwapBuffers();
}


void start_rotation(int axis, int direction, int slice) {
    rotation.step = 0;
    rotation.axis = axis;
    rotation.direction = direction;
    rotation.slice = slice;
}


void idle(void) {
    update_traslation(cam);
    glutPostRedisplay();

    if (rotation.axis != NO_ROTATION) {
        ++rotation.step;
        if (rotation.step == ROTATION_STEPS) {
            /* actualizar colores del cubo tras la rotacion */
            if      (rotation.axis == X_AXIS && rotation.direction == FORWARDS)  rotate_1_x(rubik, rotation.slice + 1);
            else if (rotation.axis == X_AXIS && rotation.direction == BACKWARDS) rotate_3_x(rubik, rotation.slice + 1);
            else if (rotation.axis == Y_AXIS && rotation.direction == FORWARDS)  rotate_1_y(rubik, rotation.slice + 1);
            else if (rotation.axis == Y_AXIS && rotation.direction == BACKWARDS) rotate_3_y(rubik, rotation.slice + 1);
            else if (rotation.axis == Z_AXIS && rotation.direction == FORWARDS)  rotate_1_z(rubik, rotation.slice + 1);
            else if (rotation.axis == Z_AXIS && rotation.direction == BACKWARDS) rotate_3_z(rubik, rotation.slice + 1);

            /* detener rotacion */
            rotation.axis = NO_ROTATION;
        }
        return;
    }

    /* prender o apagar luces */
    if (teclas['1']) switch_light1();
    if (teclas['2']) switch_light2();

    /* activar rotaciones de rebanadas del cubo */
    if      (teclas['z']) start_rotation(Y_AXIS, FORWARDS,  -1);
    else if (teclas['x']) start_rotation(Y_AXIS, FORWARDS,   0);
    else if (teclas['c']) start_rotation(Y_AXIS, FORWARDS,   1);
    else if (teclas['Z']) start_rotation(Y_AXIS, BACKWARDS, -1);
    else if (teclas['X']) start_rotation(Y_AXIS, BACKWARDS,  0);
    else if (teclas['C']) start_rotation(Y_AXIS, BACKWARDS,  1);

    else if (teclas['n']) start_rotation(X_AXIS, BACKWARDS, -1);
    else if (teclas['b']) start_rotation(X_AXIS, BACKWARDS,  0);
    else if (teclas['v']) start_rotation(X_AXIS, BACKWARDS,  1);
    else if (teclas['N']) start_rotation(X_AXIS, FORWARDS,  -1);
    else if (teclas['B']) start_rotation(X_AXIS, FORWARDS,   0);
    else if (teclas['V']) start_rotation(X_AXIS, FORWARDS,   1);

    else if (teclas['h']) start_rotation(Z_AXIS, FORWARDS,  -1);
    else if (teclas['g']) start_rotation(Z_AXIS, FORWARDS,   0);
    else if (teclas['f']) start_rotation(Z_AXIS, FORWARDS,   1);
    else if (teclas['H']) start_rotation(Z_AXIS, BACKWARDS, -1);
    else if (teclas['G']) start_rotation(Z_AXIS, BACKWARDS,  0);
    else if (teclas['F']) start_rotation(Z_AXIS, BACKWARDS,  1);

    /* activar traslaciones de la camara alrededor del cubo */
    if      (teclas['a']) start_traslation(cam, LEFT);
    else if (teclas['s']) start_traslation(cam, DOWN);
    else if (teclas['d']) start_traslation(cam, RIGHT);
    else if (teclas['w']) start_traslation(cam, UP);
}


void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.1, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void teclaDown(unsigned char key, int x, int y) { teclas[key] = 1; }
void teclaUp(unsigned char key, int x, int y)   { teclas[key] = 0; }
void mouse(int x, int y) { mouseX = x; mouseY = y; }

