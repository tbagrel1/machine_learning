#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef unsigned char uchar;

#define WIDTH 96
#define HEIGHT 128
#define N (WIDTH * HEIGHT)

#define px(x, y) (image[(y) * WIDTH + (x)])
#define pxi(x, y, image) (image[(y) * WIDTH + (x)])
#define uchar_norm(x) ((uchar) ((x) <= 255 ? (x) >= 0 ? (x) : 0 : 255))
#define uchar_rd(x) uchar_norm(round((double) (x)))
#define int_rd(x) (int) (round((double) (x)))

#define in_canvas (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
#define in_circle ((d = dist(x, y, x_c, y_c)) <= r)
#define begin_circle_iter for (x = x_c - r; x <= x_c + r; ++x) { \
                              for (y = y_c - r; y <= y_c + r; ++y) { if (in_canvas) { if (in_circle) {
#define end_circle_iter       } } } \
                          }
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

uchar image[N];
double d;
int x, y;

double rd() {
    return (double) (rand()) / (double) (RAND_MAX);
}

double rdn() {
    return 2.0 * rd() - 1.0;
}

void load_image(char const *path) {
    FILE *image_file = fopen(path, "r");
    fread(image, 1, N, image_file);
    fclose(image_file);
}

void dump_image(char const *path) {
    FILE *image_file = fopen(path, "w");
    fwrite(image, 1, N, image_file);
    fclose(image_file);
}

void shrink(int red_w, int red_h) {
    uchar new_image[N];
    for (size_t i = 0; i < N; i++) { new_image[i] = 0; }
    double r_w = (double) (WIDTH - red_w) / (double) (WIDTH);
    double r_h = (double) (HEIGHT - red_h) / (double) (HEIGHT);
    int xs, ys;
    for (x = 0; x < WIDTH; ++x) {
        for (y = 0; y < HEIGHT; ++y) {
            xs = int_rd(r_w * x);
            ys = int_rd(r_h * y);
            pxi(xs, ys, new_image) = px(x, y);
        }
    }
    memcpy(image, new_image, N);
}

void move(int delta_x, int delta_y) {
    uchar new_image[N];
    for (size_t i = 0; i < N; i++) { new_image[i] = 0; }

    int x_s = max(0, -delta_x);
    int x_e = min(WIDTH - 1, WIDTH - 1 - delta_x);
    int y_s = max(0, -delta_y);
    int y_e = min(HEIGHT - 1, HEIGHT - 1 - delta_y);

    // printf("%d %d / %d %d\n", x_s, x_e, y_s, y_e);

    for (x = x_s; x <= x_e; ++x) {
        for (y = y_s; y <= y_e; ++y) {
            pxi(x + delta_x, y + delta_y, new_image) = px(x, y);
        }
    }
    memcpy(image, new_image, N);
}

void move_blur(int delta_x, int delta_y, double ghost_ratio) {
    uchar new_image[N];
    for (size_t i = 0; i < N; i++) { new_image[i] = 0; }

    int x_s = max(0, -delta_x);
    int x_e = min(WIDTH - 1, WIDTH - 1 - delta_x);
    int y_s = max(0, -delta_y);
    int y_e = min(HEIGHT - 1, HEIGHT - 1 - delta_y);

    // printf("%d %d / %d %d\n", x_s, x_e, y_s, y_e);

    for (x = x_s; x <= x_e; ++x) {
        for (y = y_s; y <= y_e; ++y) {
            pxi(x + delta_x, y + delta_y, new_image) = px(x, y);
        }
    }
    double nv;
    for (x = 0; x < WIDTH; ++x) {
        for (y = 0; y < HEIGHT; ++y) {
            nv = (1.0 - ghost_ratio) * px(x, y) + ghost_ratio * pxi(x, y, new_image);
            px(x, y) = uchar_rd(nv);
        }
    }
}

void shrinkc(int red_w, int red_h) {
    shrink(red_w, red_h);
    move(int_rd((double) (red_w) / 2.0), int_rd((double) (red_h) / 2.0));
}

double dist(double x_1, double y_1, double x_2, double y_2) {
    double d_x = x_2 - x_1;
    double d_y = y_2 - y_1;
    return sqrt(d_x * d_x + d_y * d_y);
}

void shade(int delta_s) {
    for (x = 0; x < WIDTH; ++x) {
        for (y = 0; y < HEIGHT; ++y) {
            px(x, y) = uchar_norm((int) (px(x, y)) + delta_s);
        }
    }
}

void rd_alter(int delta_max) {
    double nv;
    for (x = 0; x < WIDTH; ++x) {
        for (y = 0; y < HEIGHT; ++y) {
            nv = rdn() * delta_max + px(x, y);
            px(x, y) = uchar_rd(nv);
        }
    }
}

void soft_disc(int x_c, int y_c, int r, int delta) {
    double n = 0;
    double avg_v = 0;

    begin_circle_iter
        avg_v += px(x, y);
        ++n;
    end_circle_iter

    avg_v /= n;
    double target_v = (double) (uchar_norm(avg_v + delta));

    begin_circle_iter
        px(x, y) += (target_v - px(x, y)) * ((double) (r - d) / (double) (r));
    end_circle_iter
}

void alter_contrast(double ratio, double center) {
    double nv;
    for (x = 0; x < WIDTH; ++x) {
        for (y = 0; y < HEIGHT; ++y) {
            nv = (-center + px(x, y)) * ratio + center;
            px(x, y) = uchar_rd(nv);
        }
    }
}

int main(int argc, char const *argv[]) {
    srand(0);
    load_image("brain_base.rbi");
    soft_disc(30, 30, 30, 40);
    soft_disc(40, 50, 15, 50);
    shrinkc(10, 30);
    // move_blur(10, -5, 0.35);
    alter_contrast(0.6, 120);
    rd_alter(30);
    dump_image("brain_mod.rbi");
    return 0;
}
