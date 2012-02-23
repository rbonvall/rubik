#include <stdlib.h>
#include "cube.h"

/* Iterate a variable from 0 to 2 */
#define FOREACH(var) for ((var) = 0; (var) < 3; ++(var))

/* Rotate a 3x3 matrix in place, counter-clockwise */
#define ROTATE_MATRIX(m)   \
    do {                   \
        int tmp;           \
        tmp = m[0][0];     \
        m[0][0] = m[0][2]; \
        m[0][2] = m[2][2]; \
        m[2][2] = m[2][0]; \
        m[2][0] = tmp;     \
        tmp = m[1][0];     \
        m[1][0] = m[0][1]; \
        m[0][1] = m[1][2]; \
        m[1][2] = m[2][1]; \
        m[2][1] = tmp;     \
    } while (0)


/* List of all possible movements */
void (*moves[])(struct cube *, int) = {
    &rotate_1_x, &rotate_1_y, &rotate_1_z,
    &rotate_2_x, &rotate_2_y, &rotate_2_z,
    &rotate_3_x, &rotate_3_y, &rotate_3_z,
};

void rotate_1_x(struct cube *c, int x) {
    /* top -> left -> bottom -> right */
    enum color tmp[3];
    int i;

    FOREACH(i)  tmp[i] = c->right[x][i];
    FOREACH(i)  c->right[x][i] = c->bottom[x][i];
    FOREACH(i)  c->bottom[x][i] = c->left[x][2 - i];
    FOREACH(i)  c->left[x][i] = c->top[x][i];
    FOREACH(i)  c->top[x][i] = tmp[2 - i];

    if (x == 0) {
        ROTATE_MATRIX(c->back);
    }
    else if (x == 2) {
        ROTATE_MATRIX(c->front);
    }
}

void rotate_1_y(struct cube *c, int y) {
    /* front -> bottom -> back -> top */
    enum color tmp[3];
    int i;

    FOREACH(i)  tmp[i] = c->top[i][y];
    FOREACH(i)  c->top[i][y] = c->back[y][i];
    FOREACH(i)  c->back[y][i] = c->bottom[2 - i][y];
    FOREACH(i)  c->bottom[i][y] = c->front[y][i];
    FOREACH(i)  c->front[y][i] = tmp[2 - i];

    if (y == 0) {
        ROTATE_MATRIX(c->left);
        ROTATE_MATRIX(c->left);
        ROTATE_MATRIX(c->left);
    }
    else if (y == 2) {
        ROTATE_MATRIX(c->right);
        ROTATE_MATRIX(c->right);
        ROTATE_MATRIX(c->right);
    }
}

void rotate_1_z(struct cube *c, int z) {
    /* left -> front -> right -> back */
    enum color tmp[3];
    int i;

    FOREACH(i)  tmp[i] = c->back[i][z];
    FOREACH(i)  c->back[i][z] = c->right[i][z];
    FOREACH(i)  c->right[i][z] = c->front[2 - i][z];
    FOREACH(i)  c->front[i][z] = c->left[i][z];
    FOREACH(i)  c->left[i][z] = tmp[2 - i];

    if (z == 0) {
        ROTATE_MATRIX(c->bottom);
    }
    else if (z == 2) {
        ROTATE_MATRIX(c->top);
    }
}

/* TODO: ¿valdrá la pena implementarlas de verdad? */
void rotate_2_x(struct cube *c, int x) {
    rotate_1_x(c, x);
    rotate_1_x(c, x);
}
void rotate_2_y(struct cube *c, int y) {
    rotate_1_y(c, y);
    rotate_1_y(c, y);
}
void rotate_2_z(struct cube *c, int z) {
    rotate_1_z(c, z);
    rotate_1_z(c, z);
}
void rotate_3_x(struct cube *c, int x) {
    rotate_2_x(c, x);
    rotate_1_x(c, x);
}
void rotate_3_y(struct cube *c, int y) {
    rotate_2_y(c, y);
    rotate_1_y(c, y);
}
void rotate_3_z(struct cube *c, int z) {
    rotate_2_z(c, z);
    rotate_1_z(c, z);
}


struct cube *new_solved_cube() {
    int i, j;
    struct cube *c = malloc(sizeof(*c));

    if (!c)
        return NULL;

    FOREACH(i) FOREACH(j) c->left[i][j] = GREEN;
    FOREACH(i) FOREACH(j) c->right[i][j] = BLUE;
    FOREACH(i) FOREACH(j) c->top[i][j] = YELLOW;
    FOREACH(i) FOREACH(j) c->bottom[i][j] = WHITE;
    FOREACH(i) FOREACH(j) c->front[i][j] = RED;
    FOREACH(i) FOREACH(j) c->back[i][j] = ORANGE;
            
    return c;
}


struct cube *new_random_cube() {
    int k, m, n;
    struct cube *c = new_solved_cube();

    if (!c)
        return NULL;

    for (n = 1; n < 30; ++n) {
        k = (rand() >> 4) % 3; /* Slice to be moved */
        m = (rand() >> 4) % 9; /* Movement to be made */

        moves[m](c, k);
    }

    return c;
}

#undef FOREACH
#undef ROTATE_MATRIX

