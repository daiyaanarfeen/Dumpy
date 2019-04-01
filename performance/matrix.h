#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct shape {
    int rows;
    int cols;
} shape;

typedef struct matrix {
    shape dim;
    float** data;
} matrix;

int allocate_matrix(matrix **mat, int rows, int cols);
int allocate_matrix_s(matrix **mat, shape s);
int eye(matrix **mat, shape s);
void free_matrix(matrix *mat);
void dot_product(matrix *vec1, matrix *vec2, float *result);
void outer_product(matrix *vec1, matrix *vec2, matrix *dst);
void matrix_power(matrix *mat, int pow, matrix *dst);
void matrix_multiply(matrix *mat1, matrix *mat2, matrix *dst);
void matrix_scale(matrix *mat, float scalar, matrix *dst);
void apply_func(matrix* mat, matrix* dst, float (*f)(float));
void matrix_add(matrix *mat1, matrix *mat2, matrix *dst);
void matrix_multiply_elementwise(matrix *mat1, matrix *mat2, matrix *dst);
void matrix_transpose(matrix *m, matrix *dst);
void copy(matrix *src, matrix *dst);
int same_size(matrix *mat1, matrix *mat2);
void set_loc(matrix *mat, int row, int col, float val);
int get_rows(matrix *mat);
int get_cols(matrix *mat);
void get_matrix_as_array(float *arr, matrix *mat);
matrix* arr_to_matrix(float *arr, int rows, int cols);
float get_loc(matrix *mat, int row, int col);
void matrix_transpose(matrix* m, matrix* dst);

