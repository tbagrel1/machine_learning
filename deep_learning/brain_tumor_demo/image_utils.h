#ifndef DEF_IMAGE_UTILS
#define DEF_IMAGE_UTILS

#include <stdbool.h>
#include <time.h>

#define WIDTH 96
#define HEIGHT 128
#define N (WIDTH * HEIGHT)

typedef unsigned char uchar;
typedef uchar pixel;
typedef pixel * Image;
typedef char const * Path;

extern int x;
extern int y;
extern double d;
extern int _temp;

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

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define int_round(x) ((int) (round((double) (x))))
#define pixel_norm(x) ((pixel) (_temp = int_round(x), (_temp <= 255 ? _temp >= 0 ? _temp : 0 : 255)))

Image new_image();
void delete_image(Image image);
Image image_from_path(Path path);
void image_to_path(Image image, Path path);

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

#endif  // DEF_IMAGE_UTILS
