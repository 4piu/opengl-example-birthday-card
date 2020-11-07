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
#define FLORAL_ANIMATION_SPEED .008
#define FLORAL_MOUSE_SENSITIVITY 15

#define EGG_A 150.0
#define EGG_B 100.0
#define EGG_K 0.002
#define EGG_HIGHLIGHT_SIZE 50

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

void use_percentage_cs();

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

void draw_ellipse(float, float, float, float, float, float[4]);

void display_egg();

void display_background();

void display_pointer();

#endif //BIRTHDAY_CARD_MAIN_H