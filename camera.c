#include "camera.h"

const float CAM_RADIUS = 20.0;
const float Z_HEIGHT   = 20.0 * 0.7;

/* coordenadas cilindricas (con r fijo) a cartesianas */
float x_from_angle(float radians) {
    return CAM_RADIUS * cos(radians);
}
float y_from_angle(float radians) {
    return CAM_RADIUS * sin(radians);
}


struct camera *new_camera() {
    struct camera *c;
    
    c = malloc(sizeof(*c));
    if (!c)
        return NULL;

    c->from = TOP_CAMERA;
    c->quadrant = 0;
    c->traslation_step = 0;
    c->traslation_direction = NO_TRASLATION;
    return c;
}

float camera_x(struct camera *c) {
    float x, f;

    if (c->quadrant == 0 || c->quadrant == 3)
        x = CAM_RADIUS;
    else
        x = -CAM_RADIUS;

    f = (float) c->traslation_step / (float) TRASLATION_STEPS;
    if      (c->quadrant == 0 && c->traslation_direction == RIGHT) x -= f * 2 * CAM_RADIUS;
    else if (c->quadrant == 1 && c->traslation_direction == LEFT)  x += f * 2 * CAM_RADIUS;
    else if (c->quadrant == 2 && c->traslation_direction == RIGHT) x += f * 2 * CAM_RADIUS;
    else if (c->quadrant == 3 && c->traslation_direction == LEFT)  x -= f * 2 * CAM_RADIUS;

    return x;
}

float camera_y(struct camera *c) {
    float y, f;

    if (c->quadrant == 0 || c->quadrant == 1)
        y = CAM_RADIUS;
    else
        y = -CAM_RADIUS;

    f = (float) c->traslation_step / (float) TRASLATION_STEPS;
    if      (c->quadrant == 0 && c->traslation_direction == LEFT)  y -= f * 2 * CAM_RADIUS;
    else if (c->quadrant == 1 && c->traslation_direction == RIGHT) y -= f * 2 * CAM_RADIUS;
    else if (c->quadrant == 2 && c->traslation_direction == LEFT)  y += f * 2 * CAM_RADIUS;
    else if (c->quadrant == 3 && c->traslation_direction == RIGHT) y += f * 2 * CAM_RADIUS;

    return y;
}

float camera_z(struct camera *c) {
    float z, f;
    if (c->from == TOP_CAMERA)
        z = Z_HEIGHT;
    else
        z = -Z_HEIGHT;

    f = (float) c->traslation_step / (float) TRASLATION_STEPS;
    if      (c->from == TOP_CAMERA    && c->traslation_direction == DOWN) z -= f * 2 * Z_HEIGHT;
    else if (c->from == BOTTOM_CAMERA && c->traslation_direction == UP)   z += f * 2 * Z_HEIGHT;

    return z;
}

void start_traslation(struct camera *c, int direction) {
    /* abortar si ya hay una traslacion en curso */
    if (c->traslation_direction != NO_TRASLATION)
        return;

    c->traslation_direction = direction;
    c->traslation_step = 0;

}

void update_traslation(struct camera *c) {
    /* abortar si estamos quietos */
    if (c->traslation_direction == NO_TRASLATION)
        return;

    ++c->traslation_step;

    /* termino la traslacion */
    if (c->traslation_step == TRASLATION_STEPS) {
        if      (c->traslation_direction == UP)    c->from = TOP_CAMERA;
        else if (c->traslation_direction == DOWN)  c->from = BOTTOM_CAMERA;
        else if (c->traslation_direction == LEFT)  c->quadrant--;
        else if (c->traslation_direction == RIGHT) c->quadrant++;
        c->traslation_direction = NO_TRASLATION;

        /* "mod 4 a mano", ya que el operador % funciona extran~o
         * con numeros negativos :( */
        if      (c->quadrant ==  4) c->quadrant = 0;
        else if (c->quadrant == -1) c->quadrant = 3;
    }
}

