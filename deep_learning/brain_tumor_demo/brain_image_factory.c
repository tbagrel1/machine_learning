#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "image_utils.h"

void tumor(Image *p_image, Image *p_temp, int x_c, int y_c, int r_c_area, int r_min, int r_max, int n_min, int n_max, int delta_min, int delta_max) {
    int n = int_rd(n_min, n_max);
    Image temp = image__new();
    int r, x_cc, y_cc, delta;
    for (int i = 0; i < n; ++i) {
        memcpy(temp, *p_image, N * sizeof(pixel));

        r = int_rd(r_min, r_max);
        delta = int_rd(delta_min, delta_max);
        point_circle_rd(x_c, y_c, r_c_area, &x_cc, &y_cc);

        soft_disc(&temp, p_temp, x_cc, y_cc, r, delta);

        BEGIN_CANVAS_ITER
            if (px(x, y, *p_image) < px(x, y, temp)) {
                px(x, y, *p_image) = px(x, y, temp);
            }
        END_CANVAS_ITER
    }
    image__delete(temp);
}

void small_tumor(Image *p_image, Image *p_temp, int x_c, int y_c) {
    tumor(p_image, p_temp, x_c, y_c, 3, 2, 3, 1, 3, 30, 60);
}

void medium_tumor(Image *p_image, Image *p_temp, int x_c, int y_c) {
    tumor(p_image, p_temp, x_c, y_c, 5, 4, 6, 2, 3, 30, 50);
}

void big_tumor(Image *p_image, Image *p_temp, int x_c, int y_c) {
    tumor(p_image, p_temp, x_c, y_c, 10, 6, 10, 3, 5, 15, 45);
}

void brain_pos_rd(int *p_x, int *p_y) {
    do {
        *p_x = int_rd(9, 86);
        *p_y = int_rd(25, 102);
    } while (!in_circle(*p_x, *p_y, 47, 63, 38));
}

void case_a(Image *p_image, Image *p_temp) {
    int big_nb = int_rd(1, 2);
    int medium_nb = int_rd(0, 2);
    int x, y;
    for (int i = 0; i < big_nb; ++i) {
        brain_pos_rd(&x, &y);
        big_tumor(p_image, p_temp, x, y);
    }
    for (int i = 0; i < medium_nb; ++i) {
        brain_pos_rd(&x, &y);
        medium_tumor(p_image, p_temp, x, y);
    }
}

void case_b(Image *p_image, Image *p_temp) {
    int medium_nb = int_rd(2, 4);
    int small_nb = int_rd(0, 3);
    int x, y;
    for (int i = 0; i < medium_nb; ++i) {
        brain_pos_rd(&x, &y);
        medium_tumor(p_image, p_temp, x, y);
    }
    for (int i = 0; i < small_nb; ++i) {
        brain_pos_rd(&x, &y);
        small_tumor(p_image, p_temp, x, y);
    }
}

void case_c(Image *p_image, Image *p_temp) {
    int small_nb = int_rd(10, 18);
    for (int i = 0; i < small_nb; ++i) {
        brain_pos_rd(&x, &y);
        small_tumor(p_image, p_temp, x, y);
    }
}

void case_z(Image *p_image, Image *p_temp) {
}

bool alter_full(Image *p_image, Image *p_temp) {
    shrink(p_image, p_temp, rdn(0.75, 1.0), rdn(0.75, 1.0));
    move(p_image, p_temp, int_rd(-8, 8), int_rd(-10, 10));
    bool blurred = false;
    if (rd() < 0.03) {
        ghost_blur(p_image, p_temp, int_rd(2, 4), int_rd(3, 5), 0.2 + 0.2 * rd());
        blurred = true;
    }
    alter_contrast(p_image, p_temp, rdn(0.65, 1.35), int_rdn(110, 142));
    alter_shade(p_image, p_temp, int_rdn(-35, 35));
    alter_rdc(p_image, p_temp, int_rdna(1, 15));
    return blurred;
}

char type_rd() {
    double v = rd();
    if (v < 0.05) {
        return 'a';
    }
    if (v < 0.25) {
        return 'b';
    }
    if (v < 0.50) {
        return 'c';
    }
    return 'z';
}

void create_batches(int batch_nb, int batch_size, bool generate_pgm) {
    Image image = image__new();
    Image temp = image__new();

    int id = rand() % 1000000;
    char out_dir_pgm[128];
    memset(out_dir_pgm, 0, 128);
    char out_dir[128];
    memset(out_dir, 0, 128);
    char out_dat[128];
    memset(out_dat, 0, 128);
    sprintf(out_dir, "../output/gen_%06d", id);
    sprintf(out_dir_pgm, "../output_pgm/gen_%06d", id);
    sprintf(out_dat, "%s.csv", out_dir);
    mkdir(out_dir, 0777);
    if (generate_pgm) {
        mkdir(out_dir_pgm, 0777);
    }

    FILE *out_dat_file = fopen(out_dat, "w");

    bool blurred;
    char type;
    char out_name[128];
    char out_batch[128];
    char out_name_pgm[128];
    char out_batch_pgm[128];
    char cmd[1024];
    for (int batch_id = 0; batch_id < batch_nb; ++batch_id) {
        memset(out_batch, 0, 128);
        memset(out_batch_pgm, 0, 128);
        sprintf(out_batch, "%s/%06d", out_dir, batch_id);
        sprintf(out_batch_pgm, "%s/%06d", out_dir_pgm, batch_id);
        mkdir(out_batch, 0777);
        if (generate_pgm) {
            mkdir(out_batch_pgm, 0777);
        }
        for (int sample_id = 0; sample_id < batch_size; ++sample_id) {
            memset(out_name, 0, 128);
            memset(out_name_pgm, 0, 128);
            sprintf(out_name, "%s/%06d", out_batch, sample_id);
            sprintf(out_name_pgm, "%s/%06d.pgm", out_batch_pgm, sample_id);

            image__from_path(image, "../resources/brain_base.rbi");

            type = type_rd();
            switch (type) {
                case 'a':
                    case_a(&image, &temp);
                    break;
                case 'b':
                    case_b(&image, &temp);
                    break;
                case 'c':
                    case_c(&image, &temp);
                    break;
                default:  // 'z'
                    case_z(&image, &temp);
                    break;
            }
            blurred = alter_full(&image, &temp);

            image__to_path(image, out_name);
            fprintf(out_dat_file, "%06d%06d,%06d,%06d,%c,%d\n",
                batch_id, sample_id, batch_id, sample_id, type, blurred ? 1 : 0);
            if (generate_pgm) {
                memset(cmd, 0, 1024);
                printf("cat ../resources/header.pgm %s > %s", out_name, out_name_pgm);
                sprintf(cmd, "cat ../resources/header.pgm %s > %s", out_name, out_name_pgm);
                system(cmd);
            }
        }
    }

    fclose(out_dat_file);

    image__delete(temp);
    image__delete(image);

    printf("gen_%06d", id);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Only %d args supplied (instead of 3). Try again.\n", argc - 1);
        return 1;
    }
    srand(time(NULL));
    create_batches(atoi(argv[1]), atoi(argv[2]),
                   atoi(argv[3]) == 1 ? true : false);
    return 0;
}
