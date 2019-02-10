#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "image_utils.h"

#define PATH_SIZE 256
#define CMD_SIZE 1024

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
    int medium_nb = int_rd(2, 5);
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

    char gen_id[PATH_SIZE];
    char out_csv_path[PATH_SIZE];
    char out_gen_dir[PATH_SIZE];
    char out_batch_dir[PATH_SIZE];
    char out_brain_path[PATH_SIZE];
    char out_pgm_gen_dir[PATH_SIZE];
    char out_pgm_batch_dir[PATH_SIZE];
    char out_pgm_brain_path[PATH_SIZE];

    char pgm_conversion_cmd[CMD_SIZE];

    bool blurred;
    char type;

    // Generate gen_id
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(
        gen_id, "%04d%02d%02d%02d%02d%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec
    );

    // Generate gen_dir(s)
    sprintf(out_gen_dir, "../output/%s", gen_id);
    sprintf(out_pgm_gen_dir, "../output_pgm/%s", gen_id);
    // Create gen_dir(s)
    mkdir(out_gen_dir, 0777);
    if (generate_pgm) mkdir(out_pgm_gen_dir, 0777);

    // Generate and open csv records file
    sprintf(out_csv_path, "%s.csv", out_gen_dir);
    FILE *out_csv_file = fopen(out_csv_path, "w");

    for (int batch_id = 0; batch_id < batch_nb; ++batch_id) {

        // Generate batch_dir(s)
        sprintf(out_batch_dir, "%s/%06d", out_gen_dir, batch_id);
        sprintf(out_pgm_batch_dir, "%s/%06d", out_pgm_gen_dir, batch_id);
        // Create batch_dir(s)
        mkdir(out_batch_dir, 0777);
        if (generate_pgm) mkdir(out_pgm_batch_dir, 0777);

        for (int sample_id = 0; sample_id < batch_size; ++sample_id) {

            // Generate brain_path(s)
            sprintf(out_brain_path, "%s/%06d", out_batch_dir, sample_id);
            sprintf(out_pgm_brain_path, "%s/%06d.pgm", out_pgm_batch_dir, sample_id);
            // Generate PGM conversion command
            sprintf(pgm_conversion_cmd, "cat ../resources/header.pgm %s > %s", out_brain_path, out_pgm_brain_path);

            // Read base image
            image__from_path(image, "../resources/brain_base.rbi");

            // Choose a case and alter the image
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

            // Write output image
            image__to_path(image, out_brain_path);

            // Write the corresponding record in the csv file
            fprintf(
                out_csv_file, "%s%06d%06d,%s,%06d,%06d,%c,%d\n",
                gen_id, batch_id, sample_id,
                gen_id,
                batch_id,
                sample_id,
                type,
                blurred ? 1 : 0
            );

            if (generate_pgm) system(pgm_conversion_cmd);
        }
    }

    fclose(out_csv_file);

    image__delete(temp);
    image__delete(image);

    printf("output/%s.csv\noutput/%s/*\n", gen_id, gen_id);
}

bool parse_long(char *str, long *p_res) {
    char *ptr = NULL;
    *p_res = strtol(str, &ptr, 10);
    if (*ptr != '\0') {
        return false;
    }
    return true;
}

bool extract_bool(char *str) {
    if (str[0] == '1' || strcmp(str, "true") == 0) {
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
    srand(time(NULL));

    // Args validation

    if (argc != 4) {
        puts("Invalid number of args. USAGE:\n\n"
             "brain_tumor_factory  BATCH_NB  BATCH_SIZE  GENERATE_PGM\n\n"
             "    BATCH_NB:       number of batches  (<= 1 000 000)\n"
             "    BATCH_SIZE:     number of samples in each batch  (<= 1 000 000)\n"
             "    GENERATE_PGM:   should we create PGM copy of each binary image for you? (0/false | 1/true)\n");

        return 1;
    }

    long l_batch_nb;
    bool valid_batch_nb = parse_long(argv[1], &l_batch_nb);
    if (!valid_batch_nb || l_batch_nb > 1000000) {
        puts("Invalid BATCH_NB. Try again.\n");
    }

    long l_batch_size;
    bool valid_batch_size = parse_long(argv[2], &l_batch_size);
    if (!valid_batch_size || l_batch_size > 1000000) {
        puts("Invalid BATCH_SIZE. Try again.\n");
    }

    bool generate_pgm = extract_bool(argv[3]);

    // Processing

    create_batches((int) (l_batch_nb), (int) (l_batch_size), generate_pgm);

    return 0;
}
