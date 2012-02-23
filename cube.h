#if !defined(__CUBE__)
#define __CUBE__

enum color {GREEN, RED, YELLOW, WHITE, ORANGE, BLUE};

struct cube {
    enum color left[3][3];    /* x, z */
    enum color right[3][3];   /* x, z */
    enum color top[3][3];     /* x, y */
    enum color bottom[3][3];  /* x, y */
    enum color front[3][3];   /* y, z */
    enum color back[3][3];    /* y, z */
};

void rotate_1_x(struct cube *c, int x);
void rotate_1_y(struct cube *c, int y);
void rotate_1_z(struct cube *c, int z);

void rotate_2_x(struct cube *c, int x);
void rotate_2_y(struct cube *c, int y);
void rotate_2_z(struct cube *c, int z);

void rotate_3_x(struct cube *c, int x);
void rotate_3_y(struct cube *c, int y);
void rotate_3_z(struct cube *c, int z);

struct cube *new_solved_cube();
struct cube *new_random_cube();

#endif

