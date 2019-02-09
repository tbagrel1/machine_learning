#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "image_utils.h"

int x = 0;
int y = 0;
double d = 0.0;

void image__swap(Image *p_image_1, Image *p_image_2) {
    Image temp = *p_image_2;
    *p_image_2 = *p_image_1;
    *p_image_1 = temp;
}

Image image__new() {
    return (Image) (calloc(N, sizeof(pixel)));
}

void image__clear(Image image) {
    for (size_t i = 0; i < N; i++) {
        image[i] = (pixel) (0);
    }
}

void image__delete(Image image) {
    free(image);
}

void image__from_path(Image image, Path path) {
    FILE *image_file = fopen(path, "r");
    fread(image, sizeof(pixel), N, image_file);
    fclose(image_file);
}

void image__to_path(Image image, Path path) {
    FILE *image_file = fopen(path, "w");
    fwrite(image, sizeof(pixel), N, image_file);
    fclose(image_file);
}

void _shrink_topleft(Image *p_image, Image *p_temp, double ratio_w, double ratio_h) {
    image__clear(*p_temp);

    int n_x, n_y;
    BEGIN_CANVAS_ITER
        n_x = int_round(ratio_w * x);
        n_y = int_round(ratio_h * y);
        px(n_x, n_y, *p_temp) = px(x, y, *p_image);
    END_CANVAS_ITER

    image__swap(p_image, p_temp);
}

void move(Image *p_image, Image *p_temp, int delta_x, int delta_y) {
    image__clear(*p_temp);

    int x_s = max(0, -delta_x);
    int x_ee = min(WIDTH, WIDTH - delta_x);
    int y_s = max(0, -delta_y);
    int y_ee = min(HEIGHT, HEIGHT - delta_y);

    BEGIN_RECT_ITER(x_s, x_ee, y_s, y_ee)
        px(x + delta_x, y + delta_y, *p_temp) = px(x, y, *p_image);
    END_RECT_ITER

    image__swap(p_image, p_temp);
}

void shrink(Image *p_image, Image *p_temp, double ratio_w, double ratio_h) {
    int delta_x = int_round((1.0 - ratio_w) / 2.0 * WIDTH);
    int delta_y = int_round((1.0 - ratio_h) / 2.0 * HEIGHT);

    _shrink_topleft(p_image, p_temp, ratio_w, ratio_h);
    move(p_image, p_temp, delta_x, delta_y);
}

void ghost_blur(Image *p_image, Image *p_temp, int delta_x, int delta_y, double ghost_ratio) {
    move(p_image, p_temp, delta_x, delta_y);
    // Depends on move implementation
    // after this line, p_temp contains the moved image, and p_image the
    // original one

    BEGIN_CANVAS_ITER
        px(x, y, *p_image) = pixel_norm(
            (1.0 - ghost_ratio) * px(x, y, *p_image) +
            ghost_ratio * px(x, y, *p_temp)
        );
    END_CANVAS_ITER
}

void alter_shade(Image *p_image, Image *p_temp, int delta_s) {
    BEGIN_CANVAS_ITER
        px(x, y, *p_image) = pixel_norm(
            (int) (px(x, y, *p_image)) + delta_s
        );
    END_CANVAS_ITER
}

void alter_rdc(Image *p_image, Image *p_temp, int delta_max) {
    BEGIN_CANVAS_ITER
        px(x, y, *p_image) = pixel_norm(
            px(x, y, *p_image) + rdc() * delta_max
        );
    END_CANVAS_ITER
}

void alter_contrast(Image *p_image, Image *p_temp, double ratio, double center) {
    BEGIN_CANVAS_ITER
        px(x, y, *p_image) = pixel_norm(
            ((double) (px(x, y, *p_image)) - center) * ratio + center
        );
    END_CANVAS_ITER
}

void soft_disc(Image *p_image, Image *p_temp, int x_c, int y_c, int r, int delta_avg) {
    int n = 0;
    double avg = 0.0;

    BEGIN_CIRCLE_ITER(x_c, y_c, r)
        avg += px(x, y, *p_image);
        ++n;
    END_CIRCLE_ITER

    avg /= n;
    double target = (double) (pixel_norm(avg + delta_avg));

    BEGIN_CIRCLE_ITER(x_c, y_c, r)
        px(x, y, *p_image) = pixel_norm(
            px(x, y, *p_image) + (target - px(x, y, *p_image)) * ((double) (r - d) / (double) (r))
        );
    END_CIRCLE_ITER
}
