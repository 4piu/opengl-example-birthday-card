#include "main.h"
#include "utilities.h"

// global vars
Point mouse_pointer = {0, 0};
Point ref_point = {0, 0};
floral bk_decorations[FLORAL_COUNT];
enum Color {
    PINK, BLUE, YELLOW, GREEN, WHITE
} bk_color = YELLOW;
enum EggStatus {
    A, B, C, D, E
} egg_status = A;
bool hover_on_egg = false;
float beam_rotation = 0;
float scale_factor = 1;

// implementations

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(APP_WIDTH, APP_HEIGHT);  // set window size
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - APP_WIDTH) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - APP_HEIGHT) / 2); // center the window
    glutCreateWindow("Happy Birthday!");    // window title
    glutSetCursor(GLUT_CURSOR_NONE);    // hide cursor

    // set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape_handler);
    glutTimerFunc(0, timer_handler, 0);
    glutKeyboardFunc(keyboard_handler);
    glutSpecialFunc(special_handler);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);
    glutPassiveMotionFunc(motion_handler);

    // msaa
    glEnable(GL_MULTISAMPLE);

    // enable blend (rgba and smooth support)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // init global vars
    init_floral();

    glutMainLoop(); // m4k3 17 r41n
    return 0;
}

void display() {
//    glClear(GL_COLOR_BUFFER_BIT);
    use_absolute_cs();

    display_background();
    if (egg_status == E) {
        display_beam();
    }
    display_egg();
    if (egg_status == E) {
        display_chick();
        display_text();
    }
    display_pointer();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int, int) {
    switch (key) {
        case 'q':
        case 'Q':
            exit(0);
        case 'r':
        case 'R':
            reset_status();
            break;
        case 'b':
        case 'B':
            if (bk_color != PINK) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) - 1);
            } else {
                bk_color = WHITE;
            }
            break;
        case 'n':
        case 'N':
            if (bk_color != WHITE) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) + 1);
            } else {
                bk_color = PINK;
            }
            break;
    }
}

void special_handler(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_LEFT:
            if (bk_color != PINK) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) - 1);
            } else {
                bk_color = WHITE;
            }
            break;
        case GLUT_KEY_DOWN:
        case GLUT_KEY_RIGHT:
            if (bk_color != WHITE) {
                bk_color = static_cast<Color>(static_cast<int>(bk_color) + 1);
            } else {
                bk_color = PINK;
            }
            break;
    }
}

void mouse_handler(int btn, int state, int x, int y) {
    // egg click event
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && hover_on_egg) {
        if (egg_status != E) {
            egg_status = static_cast<EggStatus>(static_cast<int>(egg_status) + 1);
        }
    }
}

void motion_handler(int x, int y) {
    // track mouse pointer position
    mouse_pointer.x =
            (float) x / (float) glutGet(GLUT_WINDOW_WIDTH) * APP_WIDTH;
    mouse_pointer.y =
            (float) y / (float) glutGet(GLUT_WINDOW_HEIGHT) * APP_HEIGHT;

    // change center ref point
    ref_point.x = (float) x / (float) glutGet(GLUT_WINDOW_WIDTH) - .5;
    ref_point.y = (float) y / (float) glutGet(GLUT_WINDOW_HEIGHT) - .5;

    // monitor cursor hover on egg
    hover_on_egg = x > (EGG_X - EGG_B) * scale_factor &&
                   x < (EGG_X + EGG_B) * scale_factor &&
                   y > (EGG_Y - EGG_A) * scale_factor &&
                   y < (EGG_Y + EGG_A) * scale_factor;
}

void reshape_handler(int w, int h) {
    int new_w = w, new_h = h;
    if ((float) w / (float) h > (float) APP_WIDTH / (float) APP_HEIGHT) {
        new_w = (int) ((float) h * (float) APP_WIDTH / (float) APP_HEIGHT);
    } else if ((float) w / (float) h < (float) APP_WIDTH / (float) APP_HEIGHT) {
        new_h = (int) ((float) w / (float) APP_WIDTH * (float) APP_HEIGHT);
    }
    glViewport((w - new_w) / 2, (h - new_h) / 2, new_w, new_h); // set viewport
    scale_factor = (float) new_w / APP_WIDTH;
    glutReshapeWindow(new_w, new_h);    // force Shape window to specified ratio
}

void timer_handler(int) {
    animate_floral();
    animate_beam();
    glutPostRedisplay();    // redraw content
    glutTimerFunc(1000 / 60, timer_handler, 0);
}

void display_background() {
    glBegin(GL_POLYGON);
    switch (bk_color) {
        case PINK:
            glColor4f(255 / 255.0, 196 / 255.0, 216 / 255.0, 1);
            glVertex2f(0, 0);
            glVertex2f(APP_WIDTH, 0);
            glColor4f(255 / 255.0, 220 / 255.0, 231 / 255.0, 1);
            glVertex2f(APP_WIDTH, APP_HEIGHT);
            glVertex2f(0, APP_HEIGHT);
            break;
        case BLUE:
            glColor4f(132 / 255.0, 209 / 255.0, 250 / 255.0, 1);
            glVertex2f(0, 0);
            glVertex2f(APP_WIDTH, 0);
            glColor4f(201 / 255.0, 243 / 255.0, 255 / 255.0, 1);
            glVertex2f(APP_WIDTH, APP_HEIGHT);
            glVertex2f(0, APP_HEIGHT);
            break;
        case YELLOW:
            glColor4f(245 / 255.0, 228 / 255.0, 152 / 255.0, 1);
            glVertex2f(0, 0);
            glVertex2f(APP_WIDTH, 0);
            glColor4f(255 / 255.0, 249 / 255.0, 214 / 255.0, 1);
            glVertex2f(APP_WIDTH, APP_HEIGHT);
            glVertex2f(0, APP_HEIGHT);
            break;
        case GREEN:
            glColor4f(189 / 255.0, 217 / 255.0, 152 / 255.0, 1);
            glVertex2f(0, 0);
            glVertex2f(APP_WIDTH, 0);
            glColor4f(233 / 255.0, 252 / 255.0, 199 / 255.0, 1);
            glVertex2f(APP_WIDTH, APP_HEIGHT);
            glVertex2f(0, APP_HEIGHT);
            break;
        case WHITE:
            glColor4f(235 / 255.0, 235 / 255.0, 235 / 255.0, 1);
            glVertex2f(0, 0);
            glVertex2f(APP_WIDTH, 0);
            glColor4f(255 / 255.0, 255 / 255.0, 255 / 255.0, 1);
            glVertex2f(APP_WIDTH, APP_HEIGHT);
            glVertex2f(0, APP_HEIGHT);
            break;
    }
    glEnd();

    floral *ptr = bk_decorations;
    floral *end_ptr = bk_decorations + sizeof(bk_decorations) / sizeof(bk_decorations[0]);
    while (ptr < end_ptr) {
        draw_floral(ptr);
        ptr++;
    }

}

void display_pointer() {
    if (hover_on_egg) {
        glColor4f(255 / 255.0, 0 / 255.0, 255 / 255.0, 1);
    } else {
        glColor4f(255 / 255.0, 255 / 255.0, 255 / 255.0, 1);
    }
    glPointSize(10 * scale_factor);
    glBegin(GL_POINTS);
    glVertex2f(mouse_pointer.x, mouse_pointer.y);
    glEnd();
}

void draw_floral(floral *f) {
    glPushMatrix(); // save previous cs
    glTranslatef(f->position.x - ref_point.x * FLORAL_MOUSE_SENSITIVITY,
                 f->position.y - ref_point.y * FLORAL_MOUSE_SENSITIVITY, 0);  // move to new cs
    glRotatef(f->rotation, 0, 0, 1); // rotate cs

    glColor4f(f->color[0], f->color[1], f->color[2], f->alpha);
    glEnable(GL_POLYGON_SMOOTH);    // smooth
    switch (f->shape) {
        case floral::SQUARE:
            glRectf(-f->size / 2, -f->size / 2, f->size / 2, f->size / 2);
            break;
        case floral::TRIANGLE:
            glBegin(GL_TRIANGLES);
            glVertex2f(-f->size * std::sqrt(3) / 2, -f->size / 2);
            glVertex2f(f->size * std::sqrt(3) / 2, -f->size / 2);
            glVertex2f(0, f->size);
            break;
        case floral::CIRCLE:
            glBegin(GL_POLYGON);
            for (float i = 0; i < 2 * PI; i += PI / 360) {
                glVertex2f(f->size / 2 * cosf(i), f->size / 2 * sinf(i));
            }
            break;
    }
    glEnd();
    glDisable(GL_POLYGON_SMOOTH);

    glPopMatrix();  // restore previous cs
}

void use_absolute_cs() {
    glMatrixMode(GL_PROJECTION); // change matrix mode
    glLoadIdentity();   // reset projection matrix
    gluOrtho2D(0, APP_WIDTH, APP_HEIGHT, 0); // use custom coordination system
    glMatrixMode(GL_MODELVIEW);
}

void init_floral() {
    floral *ptr = bk_decorations;
    floral *end_ptr = bk_decorations + sizeof(bk_decorations) / sizeof(bk_decorations[0]);
    while (ptr < end_ptr) {
        // random position
        ptr->position.x = random_int(FLORAL_MARGIN, APP_WIDTH - FLORAL_MARGIN);
        ptr->position.y = random_int(FLORAL_MARGIN, APP_HEIGHT - FLORAL_MARGIN);
        // random size
        ptr->size = random_float(FLORAL_SIZE_MIN, FLORAL_SIZE_MAX);
        // random rotation
        ptr->rotation = random_float(-180, 180);
        // random color
        switch (random_int(0, 4)) {
            case 0:
                ptr->color[0] = 224 / 255.0;
                ptr->color[1] = 27 / 255.0;
                ptr->color[2] = 57 / 255.0;
                break;
            case 1:
                ptr->color[0] = 235 / 255.0;
                ptr->color[1] = 100 / 255.0;
                ptr->color[2] = 28 / 255.0;
                break;
            case 2:
                ptr->color[0] = 227 / 255.0;
                ptr->color[1] = 180 / 255.0;
                ptr->color[2] = 9 / 255.0;
                break;
            case 3:
                ptr->color[0] = 18 / 255.0;
                ptr->color[1] = 130 / 255.0;
                ptr->color[2] = 46 / 255.0;
                break;
            case 4:
                ptr->color[0] = 9 / 255.0;
                ptr->color[1] = 52 / 255.0;
                ptr->color[2] = 145 / 255.0;
                break;
        }
        // random alpha
        ptr->alpha = random_float(.2, 1);
        // random Shape
        ptr->shape = static_cast<floral::Shape>(rand() % (floral::SQUARE + 1));
        ptr->fading = random_bool();
        ptr++;
    }
}

void animate_floral() {
    floral *ptr = bk_decorations;
    floral *end_ptr = bk_decorations + sizeof(bk_decorations) / sizeof(bk_decorations[0]);
    while (ptr < end_ptr) {
        if (ptr->fading) {
            ptr->alpha -= FLORAL_ANIMATION_SPEED;
            if (ptr->alpha < 0) {
                ptr->alpha = 0;
                ptr->fading = false;
                // random position on faded
                ptr->position.x = random_int(FLORAL_MARGIN, APP_WIDTH - FLORAL_MARGIN);
                ptr->position.y = random_int(FLORAL_MARGIN, APP_HEIGHT - FLORAL_MARGIN);
            }
        } else {
            ptr->alpha += FLORAL_ANIMATION_SPEED;
            if (ptr->alpha > 1) {
                ptr->alpha = 1;
                ptr->fading = true;
            }
        }
        ptr++;
    }
}

void display_egg() {
    // draw shadow
    for (int i = EGG_SHADOW_SIZE; i > 5; i -= 2) {
        draw_ellipse(EGG_X - ref_point.x * 30,
                     EGG_Y + 150,
                     0,
                     i,
                     i * .3,
                     new float[4]{.2, .2, .2, 0.02});
    }
    glEnd();

    // draw egg
    glPushMatrix();
    glTranslatef(EGG_X, EGG_Y, 0);

    glColor4f(237 / 255.0, 196 / 255.0, 166 / 255.0, 1);
    glBegin(GL_POLYGON);
    for (int y = -EGG_A; y < EGG_A + 1; y++) {
        glVertex2f(sqrt((1 - (double) y * y / EGG_A / EGG_A) * EGG_B * EGG_B / (1 - EGG_K * y)), y);
    }
    for (int y = EGG_A; y > -EGG_A - 1; y--) {
        glVertex2f(-sqrt((1 - (double) y * y / EGG_A / EGG_A) * EGG_B * EGG_B / (1 - EGG_K * y)), y);
    }
    glEnd();

    // draw outline
    glColor4f(224 / 255.0, 180 / 255.0, 155 / 255.0, 1);
    glLineWidth(1.5 * scale_factor); // must call before glBegin
    glEnable(GL_LINE_SMOOTH);   // smooth
    glBegin(GL_LINE_STRIP);
    for (int y = -EGG_A; y < EGG_A + 1; y++) {
        glVertex2f(sqrt((1 - (double) y * y / EGG_A / EGG_A) * EGG_B * EGG_B / (1 - EGG_K * y)), y);
    }
    for (int y = EGG_A; y > -EGG_A - 1; y--) {
        glVertex2f(-sqrt((1 - (double) y * y / EGG_A / EGG_A) * EGG_B * EGG_B / (1 - EGG_K * y)), y);
    }
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glPopMatrix();

    // display egg crack
    if (egg_status != A) {
        display_egg_crack();
    }

    // draw highlight
    if (egg_status != E) {
        for (int i = EGG_HIGHLIGHT_SIZE; i > 5; i -= 4) {
            draw_ellipse(EGG_X + ref_point.x * 80,
                         EGG_Y - 60,
                         0 - ref_point.x * 40,
                         i * (1 - abs(ref_point.x) * .9),
                         i,
                         new float[4]{.95, .95, .95, 0.04});
        }
    }
}

void draw_ellipse(float x, float y, float r, int a, int b, float *color) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(r, 0, 0, 1);

    glColor4f(color[0], color[1], color[2], color[3]);

    glBegin(GL_POLYGON);
    for (int _x = -a; _x < a; _x++) {
        glVertex2f(_x, sqrt((1 - (double) _x * _x / a / a) * b * b));
    }
    for (int _x = a; _x > -a; _x--) {
        glVertex2f(_x, -sqrt((1 - (double) _x * _x / a / a) * b * b));
    }
    glEnd();

    glPopMatrix();
}

void display_egg_crack() {
    glColor4f(28 / 255.0, 20 / 255.0, 12 / 255.0, 1);
    glEnable(GL_LINE_SMOOTH);
    switch (egg_status) {
        case E:
        case D:
            glBegin(GL_LINE_STRIP);
            glVertex2f(395, 536);
            glVertex2f(422, 511);
            glVertex2f(453, 522);
            glEnd();
            glBegin(GL_LINE_STRIP);
            glVertex2f(442, 518);
            glVertex2f(456, 501);
            glEnd();
        case C:
            glBegin(GL_LINE_STRIP);
            glVertex2f(311, 506);
            glVertex2f(330, 531);
            glVertex2f(362, 523);
            glVertex2f(395, 536);
            glEnd();
            glBegin(GL_LINE_STRIP);
            glVertex2f(362, 523);
            glVertex2f(373, 510);
            glEnd();
        case B:
            glBegin(GL_LINE_STRIP);
            glVertex2f(265, 498);
            glVertex2f(279, 508);
            glVertex2f(311, 506);
            glEnd();
            glBegin(GL_LINE_STRIP);
            glVertex2f(290, 508);
            glVertex2f(295, 523);
            glEnd();
        default:;
    }
    glDisable(GL_LINE_SMOOTH);
}

void display_chick() {
    glColor4f(235 / 255.0, 209 / 255.0, 101 / 255.0, 1);    // yellow

    // draw body
    // must arrange the point wisely or the triangles may overlap
    glBegin(GL_POLYGON);

    glVertex2f(360, 420); // first point in the middle of the polygon

    // crack at the bottom of the polygon
    glVertex2f(264, 496);
    glVertex2f(278, 507);
    glVertex2f(311, 505);
    glVertex2f(330, 529);
    glVertex2f(361, 521);
    glVertex2f(395, 535);
    glVertex2f(422, 510);
    glVertex2f(442, 517);
    glVertex2f(457, 499);

    // curvy dome of the polygon
    glVertex2f(459, 493);
    glVertex2f(460, 483);
    glVertex2f(459, 472);
    glVertex2f(456, 453);
    glVertex2f(454, 443);
    glVertex2f(450, 433);
    glVertex2f(446, 424);
    glVertex2f(441, 414);
    glVertex2f(434, 405);
    glVertex2f(427, 398);
    glVertex2f(418, 389);
    glVertex2f(408, 383);
    glVertex2f(396, 377);
    glVertex2f(386, 374);
    glVertex2f(375, 371);
    glVertex2f(365, 370);
    glVertex2f(353, 370);
    glVertex2f(342, 372);
    glVertex2f(333, 374);
    glVertex2f(322, 378);
    glVertex2f(310, 384);
    glVertex2f(302, 390);
    glVertex2f(294, 397);
    glVertex2f(287, 406);
    glVertex2f(282, 413);
    glVertex2f(277, 421);
    glVertex2f(273, 429);
    glVertex2f(270, 437);
    glVertex2f(268, 443);
    glVertex2f(266, 451);
    glVertex2f(265, 458);
    glVertex2f(264, 466);
    glVertex2f(263, 473);
    glVertex2f(263, 483);
    glVertex2f(263, 489);

    // manually close the outline
    glVertex2f(264, 496);

    glEnd();

    // draw ahoge
    glBegin(GL_POLYGON);
    glVertex2f(361, 340);
    glVertex2f(357, 347);
    glVertex2f(356, 352);
    glVertex2f(355, 359);
    glVertex2f(355, 363);
    glVertex2f(356, 369);
    glVertex2f(358, 372);
    glVertex2f(360, 364);
    glVertex2f(362, 357);
    glVertex2f(365, 350);
    glVertex2f(368, 345);
    glVertex2f(374, 340);
    glVertex2f(380, 336);
    glVertex2f(387, 332);
    glVertex2f(394, 330);
    glVertex2f(387, 328);
    glVertex2f(380, 329);
    glVertex2f(373, 331);
    glVertex2f(367, 335);
    glEnd();

    // draw feet
    draw_ellipse(312, 623, -20, 10, 16,
                 new float[4]{235 / 255.0, 209 / 255.0, 101 / 255.0, 1});
    draw_ellipse(412, 628, 20, 10, 16,
                 new float[4]{235 / 255.0, 209 / 255.0, 101 / 255.0, 1});

    glColor4f(74 / 255.0, 51 / 255.0, 33 / 255.0, 1);   // brown
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(6 * scale_factor);

    // left foot
    glBegin(GL_LINE_STRIP);
    glVertex2f(319, 640);
    glVertex2f(310, 623);
    glVertex2f(297, 615);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(310, 623);
    glVertex2f(315, 609);
    glEnd();
    // right foot
    glBegin(GL_LINE_STRIP);
    glVertex2f(412, 646);
    glVertex2f(420, 629);
    glVertex2f(436, 620);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(420, 629);
    glVertex2f(419, 613);
    glEnd();

    glDisable(GL_LINE_SMOOTH);

    // draw face

    // draw eyes
    glEnable(GL_POLYGON_SMOOTH);
    draw_ellipse(326 + ref_point.x * CHICK_ANIMATION_SENSITIVITY,
                 423 + ref_point.y * CHICK_ANIMATION_SENSITIVITY,
                 10 - ref_point.x * 10,
                 5, 8, new float[4]{0, 0, 0, 1});
    draw_ellipse(385 + ref_point.x * CHICK_ANIMATION_SENSITIVITY,
                 423 + ref_point.y * CHICK_ANIMATION_SENSITIVITY,
                 10 - ref_point.x * 10,
                 5, 8, new float[4]{0, 0, 0, 1});
    glDisable(GL_POLYGON_SMOOTH);

    // draw beak
    glPushMatrix();
    glTranslatef(ref_point.x * CHICK_ANIMATION_SENSITIVITY, ref_point.y * CHICK_ANIMATION_SENSITIVITY, 0);

    glEnable(GL_LINE_SMOOTH);
    glColor4f(0, 0, 0, 1);
    glLineWidth(2.5 * scale_factor);
    glBegin(GL_LINE_STRIP);
    glVertex2f(354, 448);
    glVertex2f(346, 455);
    glVertex2f(355, 461);
    glVertex2f(354, 454);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glPopMatrix();

    // draw little wings

    glColor4f(0, 0, 0, 1);
    glLineWidth(2.5 * scale_factor);
    glEnable(GL_LINE_SMOOTH);
    // left
    glBegin(GL_LINE_STRIP);
    glVertex2f(278, 494);
    glVertex2f(276, 487);
    glVertex2f(275, 480);
    glVertex2f(275, 473);
    glVertex2f(276, 469);
    glVertex2f(280, 467);
    glVertex2f(285, 471);
    glVertex2f(290, 477);
    glVertex2f(295, 485);
    glVertex2f(301, 493);
    glEnd();
    // right
    glBegin(GL_LINE_STRIP);
    glVertex2f(420, 494);
    glVertex2f(428, 485);
    glVertex2f(437, 479);
    glVertex2f(445, 476);
    glVertex2f(446, 487);
    glVertex2f(445, 495);
    glVertex2f(444, 503);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    // draw cheek
    for (int size = 14; size > 4; size -= 2) {
        draw_ellipse(297 + ref_point.x * CHICK_ANIMATION_SENSITIVITY,
                     444 + ref_point.y * CHICK_ANIMATION_SENSITIVITY,
                     0, size, size * .5,
                     new float[4]{240 / 255.0, 68 / 255.0, 115 / 255.0, 0.05});
    }
    for (int size = 14; size > 4; size -= 2) {
        draw_ellipse(410 + ref_point.x * CHICK_ANIMATION_SENSITIVITY,
                     444 + ref_point.y * CHICK_ANIMATION_SENSITIVITY,
                     0, size, size * .5,
                     new float[4]{240 / 255.0, 68 / 255.0, 115 / 255.0, 0.05});
    }
}

void reset_status() {
    egg_status = A;
}

void display_text() {
    // use glutStrokeCharacter support cs temporarily
    glMatrixMode(GL_PROJECTION); // change matrix mode
    glLoadIdentity();   // reset projection matrix
    gluOrtho2D(0, APP_WIDTH, 0, APP_HEIGHT); // use custom coordination system
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glPointSize(10 * scale_factor);
    glLineWidth(10 * scale_factor);

    glPushMatrix();
    glTranslatef(110, 680, 0);
    const char *happy = "HAPPY";
    glColor4f(0, 0, 0, 1);
    while (*happy != '\0') {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *happy);
        happy++;
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(24, 120, 0);
    glScalef(.8, .8, 0);
    const char *birthday = "BIRTHDAY";
    glColor4f(0, 0, 0, 1);
    while (*birthday != '\0') {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *birthday);
        birthday++;
    }
    glPopMatrix();

    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);

    use_absolute_cs();  // restore previous cs
}

void display_beam() {
    glPushMatrix();
    glTranslatef(APP_WIDTH / 2.0, APP_HEIGHT / 2.0, 0);
    glRotatef(beam_rotation, 0, 0, 1);
    glColor4f(1, 1, 1, .3);
    for (int i = 0; i < 360 / BEAM_WIDTH_DEG; i++) {
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0);
        glVertex2f(APP_HEIGHT * tan(BEAM_WIDTH_DEG / 2.0 * PI / 180), APP_HEIGHT);
        glVertex2f(-APP_HEIGHT * tan(BEAM_WIDTH_DEG / 2.0 * PI / 180), APP_HEIGHT);
        glEnd();
        glRotatef(BEAM_WIDTH_DEG * 2, 0, 0, 1);
    }
    glPopMatrix();
}

void animate_beam() {
    beam_rotation += BEAM_ANIMATE_SPEED;
    if (beam_rotation >= 360) beam_rotation -= 360;
}
