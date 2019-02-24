#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BINPGM_MAGIC_NB "P5"
#define BINPGM_MAGIC_LENGTH 2
#define ERROR_EXIT NULL
#define TEXT_BUFFER_SIZE 64

static PyObject* load_binpgm_cfunc(PyObject *self, PyObject *obj_image_path) {
    char *image_path = PyBytes_AsString(obj_image_path);
    if (image_path == NULL) {
        return ERROR_EXIT;
    }
    FILE *image_file = fopen(image_path, "r");
    if (image_file == NULL) {
        PyErr_SetString(PyExc_OSError, "Unable to read the specified file.");
        return ERROR_EXIT;
    }

    char magic_number_string[BINPGM_MAGIC_LENGTH + 2];
    magic_number_string[BINPGM_MAGIC_LENGTH + 1] = '\0';
    size_t chars_read;
    chars_read = fread(magic_number_string, sizeof(char), BINPGM_MAGIC_LENGTH + 1, image_file);

    // Handle magic number
    if (!(chars_read == BINPGM_MAGIC_LENGTH + 1 && strcmp(magic_number_string, BINPGM_MAGIC_NB"\n") == 0)) {
        // printf("%d vs %d\n", chars_read, BINPGM_MAGIC_LENGTH + 1);
        // printf(">>%s<<  >>%s<<\n", magic_number_string, BINPGM_MAGIC_NB"\n");
        // printf(": %d %d %d\n", magic_number_string[0], magic_number_string[1], magic_number_string[2]);
        PyErr_SetString(PyExc_OSError, "The specified file is not a BINARY PGM image.");
        return ERROR_EXIT;
    }

    // Handle comments
    int first_char;
    while (true) {
        first_char = fgetc(image_file);
        if (first_char == EOF) {
            PyErr_SetString(PyExc_OSError, "Invalid file format (1).");
            return ERROR_EXIT;
        }
        if (first_char != '#') {
            fseek(image_file, -1, SEEK_CUR);
            break;
        }
        while ((first_char = fgetc(image_file)) != '\n') {
            if (first_char == EOF) {
                PyErr_SetString(PyExc_OSError, "Invalid file format (3).");
                return ERROR_EXIT;
            }
        }
    }

    // Handle image size
    char text_buffer[TEXT_BUFFER_SIZE];
    size_t text_length;

    text_length = 0;
    while (
        text_length < TEXT_BUFFER_SIZE - 1 &&
        (first_char = fgetc(image_file)) != '\n') {
        if (first_char == EOF) {
            PyErr_SetString(PyExc_OSError, "Invalid file format (4).");
            return ERROR_EXIT;
        }
        text_buffer[text_length++] = (char) (first_char);
    }
    if (first_char != '\n') {
        PyErr_SetString(
            PyExc_OSError,
            "Invalid file format : long lines unsupported (1).");
        return ERROR_EXIT;
    }
    text_buffer[text_length] = '\0';

    long width, height;
    char *p_end = NULL;
    width = strtoul(text_buffer, &p_end, 10);
    if (p_end == text_buffer) {
        // Nothing was read
        PyErr_SetString(PyExc_OSError, "Invalid file format (5).");
        return ERROR_EXIT;
    }
    char *mid_text_buffer = p_end;
    if (*mid_text_buffer != ' ') {
        // Space not present between width and height
        // printf(">>%s<<\n", text_buffer);
        // printf(">>%c<< : %d\n", *mid_text_buffer, *mid_text_buffer);
        PyErr_SetString(PyExc_OSError, "Invalid file format (6).");
        return ERROR_EXIT;
    }
    mid_text_buffer++;
    height = strtoul(mid_text_buffer, &p_end, 10);
    if (p_end == mid_text_buffer || p_end != text_buffer + text_length) {
        // Nothing was read
        PyErr_SetString(PyExc_OSError, "Invalid file format (7).");
        return ERROR_EXIT;
    }

    // Handle pixel coding
    long pixel_max_value;
    text_length = 0;
    while (
        text_length < TEXT_BUFFER_SIZE - 1 &&
        (first_char = fgetc(image_file)) != '\n') {
        if (first_char == EOF) {
            PyErr_SetString(PyExc_OSError, "Invalid file format (8).");
            return ERROR_EXIT;
        }
        text_buffer[text_length++] = (char) (first_char);
    }
    if (first_char != '\n') {
        PyErr_SetString(
            PyExc_OSError,
            "Invalid file format : long lines unsupported. (2)");
        return ERROR_EXIT;
    }
    text_buffer[text_length] = '\0';

    pixel_max_value = strtoul(text_buffer, &p_end, 10);
    if (p_end == text_buffer || p_end != text_buffer + text_length) {
        // Nothing was read
        // printf(">>%s<<\n", text_buffer);
        // printf("%d %d\n", p_end == text_buffer, p_end != text_buffer + text_length);
        PyErr_SetString(PyExc_OSError, "Invalid file format (9).");
        return ERROR_EXIT;
    }

    // printf("### %d %d %d ###", width, height, pixel_max_value);

    size_t pixels_nb = width * height;
    size_t pixels_read;

    if (pixel_max_value > 255) {
        PyErr_SetString(PyExc_OSError, "Unsupported pixel coding.");
        return ERROR_EXIT;
    }

    unsigned char* pixel_map = malloc(pixels_nb * sizeof(unsigned char));
    pixels_read = fread(
        pixel_map, sizeof(unsigned char), pixels_nb, image_file);

    if (pixels_read != pixels_nb) {
        PyErr_SetString(PyExc_OSError, "Unable to read all pixels.");
        return ERROR_EXIT;
    }
    if (fgetc(image_file) != EOF) {
        PyErr_SetString(
            PyExc_OSError,
            "There's still something in the file after the read pixel map");
        return ERROR_EXIT;
    }

    npy_intp dims[3];
    dims[0] = width;
    dims[1] = height;
    dims[2] = 1;  // CHANNELS
    PyArrayObject *array = NULL;
    array = (PyArrayObject *) (PyArray_SimpleNew(3, dims, NPY_DOUBLE));

    double *array_cell;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            array_cell = PyArray_GETPTR3(array, x, y, 0);
            *array_cell = (double) (pixel_map[x * height + y]) / (double) (pixel_max_value);
            // *array_cell = pixel_map[x * height + y];
        }
    }

    return (PyObject *) (array);
}

static PyMethodDef binimage_methods[] = {
    {
        "load_binpgm",
        load_binpgm_cfunc,
        METH_O,
        "Returns the pixel matrix of the specified BINARY PGM image as a "
        "Numpy array of shape WIDTH * HEIGHT * CHANNELS (with CHANNELS == 1 "
        "here, because of the PGM format), with pixel values in [0, 1]."
    },
    { NULL, NULL, 0, NULL }  // Sentinel
};

static char binimage_doc[] =
    "Provides functions to deal with uncompressed binary image file formats.";

static struct PyModuleDef binimage_module = {
    PyModuleDef_HEAD_INIT,
    "binimage",
    binimage_doc,
    -1,
    binimage_methods
};

PyMODINIT_FUNC
PyInit_binimage(void) {
    import_array();
    return PyModule_Create(&binimage_module);
}
