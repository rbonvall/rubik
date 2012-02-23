#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <math.h>
#include <stdlib.h>

#define TRASLATION_STEPS 10

/* Representacion de una camara que siempre mira hacia el origen desde
 * ocho puntos fijos, pero haciendo giros continuos para trasladarse desde
 * un punto a otro */

struct camera {
    /* la camara puede mirar desde arriba (top) o desde abajo (bottom) */
    enum {TOP_CAMERA, BOTTOM_CAMERA} from;

    /* cuadrante del plano XY desde el       1 | 0 
     * que se esta mirando al origen        ---+---
     *                                       2 | 3  */
    int quadrant;

    int traslation_step;
    enum {NO_TRASLATION, UP, DOWN, LEFT, RIGHT} traslation_direction;
};

/* crea una nueva camara inicializada apropiadamente */
struct camera *new_camera();

/* entregan las coordenadas x, y y z, resp., de la
 * posicion exacta de la camara en el mundo */
float camera_x(struct camera *c);
float camera_y(struct camera *c);
float camera_z(struct camera *c);

/* activa una traslacion de la camara */
void start_traslation(struct camera *c, int direction);

/* actualiza el estado de una traslacion en curso */
void update_traslation(struct camera *c);

#endif

