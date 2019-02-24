#ifndef DEF_IMAGE_UTILS
#define DEF_IMAGE_UTILS

#include <stdbool.h>
#include <time.h>
#include <math.h>

#define WIDTH 96
#define HEIGHT 128
#define N (WIDTH * HEIGHT)
extern char const pgm_header[];

typedef unsigned char uchar;
typedef uchar pixel;
typedef pixel * Image;
typedef char const * Path;

extern int x;
extern int y;
extern double d;

static int _temp = 0;

#define px(x, y, image) ((image)[(y) * WIDTH + (x)])

#define BEGIN_CIRCLE_ITER(x_c, y_c, r)  for (x = x_c - r; x <= x_c + r; ++x) { \
                                            for (y = y_c - r; y <= y_c + r; ++y) { \
                                                if (in_canvas(x, y) && in_circle(x, y, x_c, y_c, r)) {
#define END_CIRCLE_ITER                         } \
                                            } \
                                        }
#define BEGIN_CANVAS_ITER               for (x = 0; x < WIDTH; ++x) { \
                                            for (y = 0; y < HEIGHT; ++y) {
#define END_CANVAS_ITER                     } \
                                        }
#define BEGIN_RECT_ITER(x_s, x_ee, y_s, y_ee) \
                                        for (x = x_s; x < x_ee; ++x) { \
                                            for (y = y_s; y < y_ee; ++y) {
#define END_RECT_ITER                     } \
                                        }
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define int_round(x) ((int) (round((double) (x))))
#define pixel_norm(x) ((pixel) (_temp = int_round(x), (_temp <= 255 ? _temp >= 0 ? _temp : 0 : 255)))
#define canvas_x_norm(x) (_temp = int_round(x), (_temp <= WIDTH - 1 ? _temp >= 0 ? _temp : 0 : WIDTH - 1))
#define canvas_y_norm(y) (_temp = int_round(y), (_temp <= HEIGHT - 1 ? _temp >= 0 ? _temp : 0 : HEIGHT - 1))

Image image__new();
void image__clear(Image image);
void image__swap(Image *p_image_1, Image *p_image_2);
void image__delete(Image image);
void image__from_path(Image image, Path path);
void image__to_path(Image image, Path path);
void image__to_pgm_to_path(Image image, Path path);
void move(Image *p_image, Image *p_temp, int delta_x, int delta_y);
void shrink(Image *p_image, Image *p_temp, double ratio_w, double ratio_h);
void ghost_blur(Image *p_image, Image *p_temp, int delta_x, int delta_y, double ghost_ratio);
void alter_shade(Image *p_image, Image *p_temp, int delta_s);
void alter_contrast(Image *p_image, Image *p_temp, double ratio, double center);
void alter_rdc(Image *p_image, Image *p_temp, int delta_max);
void soft_disc(Image *p_image, Image *p_temp, int x_c, int y_c, int r, int delta_avg);

static inline bool in_canvas(int x, int y) {
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

static inline double dist(double x_1, double y_1, double x_2, double y_2) {
    double d_x = x_2 - x_1;
    double d_y = y_2 - y_1;
    return sqrt(d_x * d_x + d_y * d_y);
}

static inline bool in_circle(int x, int y, int x_c, int y_c, int r) {
    return ((d = dist(x, y, x_c, y_c)) <= r);
}

static inline double rd() {
    return (double) (rand()) / (double) (RAND_MAX);
}

static inline double rdc() {
    return 2.0 * rd() - 1.0;
}


#define RDN_N 8
static inline double rdn(double a, double b) {
    double v = 0.0;
    for (int i = 0; i < RDN_N; ++i) {
        if (rd() >= 0.5) {
            v += 1.0;
        }
    }
    return ((b - a) / (double) (RDN_N)) * v + a;
}

static inline int int_rdn(int a, int b) {
    double v = rdn((double) (a), (double) (b));
    return int_round(v);
}

static inline double rdna(double c, double b) {
    double v = rdn(c - b, b);
    if (v < c) {
        return v + 2 * (c - v);
    }
    return v;
}

static inline int int_rdna(int c, int b) {
    double v = rdna((double) (c), (double) (b));
    return int_round(v);
}

static inline int int_rd(int v_min, int v_max) {
    return (int) (v_min + rd() * (v_max - v_min));
}

static inline void point_circle_rd(int x_c, int y_c, int r, int *p_x, int *p_y) {
    double arg = rd() * M_PI;
    double rr = rd() * r;
    *p_x = canvas_x_norm(x_c + rr * cos(arg));
    *p_y = canvas_y_norm(y_c + rr * sin(arg));
}

#endif  // DEF_IMAGE_UTILS
