#ifndef BIRTHDAY_CARD_MAIN_H
#define BIRTHDAY_CARD_MAIN_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <random>

// macros
#define PI 3.14159265358979323846

#define APP_WIDTH 720
#define APP_HEIGHT 960

#define FLORAL_COUNT 200
#define FLORAL_MARGIN -20
#define FLORAL_SIZE_MIN 8
#define FLORAL_SIZE_MAX 12
#define FLORAL_ANIMATION_SPEED .01
#define FLORAL_MOUSE_SENSITIVITY 15

#define EGG_X APP_WIDTH / 2.0
#define EGG_Y APP_HEIGHT / 2.0 + 50
#define EGG_A 150
#define EGG_B 100
#define EGG_K 0.002
#define EGG_HIGHLIGHT_SIZE 80
#define EGG_SHADOW_SIZE 120

#define CHICK_ANIMATION_SENSITIVITY 10

#define BEAM_WIDTH_DEG 12
#define BEAM_ANIMATE_SPEED 0.1

#define CURSOR_SCALE .4

#define POOF_SPEED 2.5

// type def

typedef struct {
    GLfloat x, y;
} Point;

typedef struct {
    Point position;
    float size;
    float rotation;
    float color[3];
    float alpha;
    enum Shape {
        TRIANGLE, CIRCLE, SQUARE
    } shape;
    bool fading;
} floral;

// function declarations

void use_absolute_cs();

void display();

void reshape_handler(int, int);

void timer_handler(int);

void keyboard_handler(unsigned char, int, int);

void special_handler(int, int, int);

void mouse_handler(int, int, int, int);

void motion_handler(int, int);

void init_floral();

void draw_floral(floral *);

void animate_floral();

void draw_ellipse(float, float, float, int, int, float[4]);

void display_egg_crack();

void display_chick();

void display_egg();

void display_background();

void display_cursor();

void reset_status();

void display_text();

void display_beam();

void animate_beam();

void count_frame();

void animate_eyes();

void display_poof();

void animate_poof();

#endif //BIRTHDAY_CARD_MAIN_H
