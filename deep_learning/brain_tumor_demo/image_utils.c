#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "image_utils.h"

int x = 0;
int y = 0;
double d = 0.0;
int _temp = 0;

static pixel _temp_image[N];

Image new_image() {
    return (Image) (malloc(N * sizeof(pixel)));
}

void delete_image(Image image) {
    free(image);
}

Image image_from_path(Path path) {
    Image res = new_image();
    FILE *image_file = fopen(path, "r");
    fread(res, sizeof(pixel), N, image_file);
    fclose(image_file);
    return res;
}

void image_to_path(Image image, Path path) {
    FILE *image_file = fopen(path, "w");
    fwrite(image, sizeof(pixel), N, image_file);
    fclose(image_file);
}

void shrink(double ratio_w, double ratio_h) {

}
