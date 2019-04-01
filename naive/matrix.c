#include "matrix.h"

/*
    called like:
    ```c
    matrix mat;
    allocate_matrix(&mat, 1, 2);
    ```
*/
int allocate_matrix(matrix **mat, int rows, int cols) {
    *mat = malloc(sizeof(matrix));
    (*mat)->dim.rows = rows;
    (*mat)->dim.cols = cols;
    (*mat)->data = malloc(sizeof(float *) * rows);
    for (int i = 0; i < rows; ++i) {
        (*mat)->data[i] = calloc(cols, sizeof(float));
    }
    return 0;
}

int allocate_matrix_s(matrix **mat, shape s) {
    return allocate_matrix(mat, s.rows, s.cols);
}

int eye(matrix **mat, shape s) {
    assert(allocate_matrix_s(mat, s) == 0);
    // Make the result an identity matrix
    for (int i = 0; i < s.rows; i++) {
        (*mat)->data[i][i] = 1;
    }
    return 0;
}

void free_matrix(matrix *mat) {
    for (int i = 0; i < mat->dim.rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

void dot_product(matrix *vec1, matrix *vec2, float *result) {
    assert(same_size(vec1, vec2) && vec1->dim.cols == 1);
    *result = 0;
    for (int i = 0; i < vec1->dim.rows; ++i) {
        *result += vec1->data[i][0] * vec2->data[i][0];
    }
}

void outer_product(matrix *vec1, matrix *vec2, matrix *dst) {
    assert(vec1->dim.cols == 1 && vec2->dim.cols == 1 && vec1->dim.rows == dst->dim.rows && vec2->dim.rows == dst->dim.cols);
    for (int i = 0; i < vec1->dim.rows; i++) {
        for (int j = 0; j < vec2->dim.rows; j++) {
            dst->data[i][j] = vec1->data[i][0] * vec2->data[j][0];
        }
    }
}

void matrix_power(matrix *mat, int pow, matrix *dst) {
    assert(mat != dst && same_size(mat, dst) && mat->dim.rows == mat->dim.cols);
    if (pow == 1) {
        copy(mat, dst);
        return;
    }
    if (pow == 2) {
        matrix_multiply(mat, mat, dst);
        return;
    }

    matrix* intermediate;
    eye(&intermediate, dst->dim);
    copy(intermediate, dst);
    for (int i = 0; i < pow; i++) {
        matrix_multiply(intermediate, mat, dst);
        copy(dst, intermediate);
    }
    free_matrix(intermediate);
}

void matrix_multiply(matrix *mat1, matrix *mat2, matrix *dst) {
    assert (mat1->dim.cols == mat2->dim.rows && dst->dim.rows == mat1->dim.rows && dst->dim.cols == mat2->dim.cols);
    for (int i = 0; i < mat1->dim.rows; i++)
        for (int j = 0; j < mat2->dim.cols; j++) {
            dst->data[i][j] = 0; //Ensures that the destination matrix is zeros initially. 
            for (int k = 0; k < mat1->dim.cols; k++)
                dst->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
        }
}

void matrix_scale(matrix *mat, float scalar, matrix *dst) {
    assert(same_size(mat, dst));
    for (int i = 0; i < mat->dim.rows; i++) {
        for (int j = 0; j < mat->dim.cols; j++) {
            dst->data[i][j] = scalar * mat->data[i][j];
        }
    }
}
void apply_func(matrix* mat, matrix* dst, float (*f)(float)) {
    assert(same_size(mat, dst));
    for (int i = 0; i < mat->dim.rows; i++) {
        for (int j = 0; j < mat->dim.cols; j++) {
            dst->data[i][j] = f(mat->data[i][j]);
        }
    }
}

void matrix_multiply_elementwise(matrix *mat1, matrix *mat2, matrix *dst) {
    assert(same_size(mat1, mat2) && same_size(mat1, dst));
    for (int i = 0; i < dst->dim.rows; i++) {
        for (int j = 0; j < dst->dim.cols; j++) {
            dst->data[i][j] = mat1->data[i][j] * mat2->data[i][j];
        }
    }
}

void matrix_add(matrix *mat1, matrix *mat2, matrix *dst) {
    assert(same_size(mat1, mat2) && same_size(mat1, dst));
    for (int i = 0; i < dst->dim.rows; i++) {
        for (int j = 0; j < dst->dim.cols; j++) {
            dst->data[i][j] = mat1->data[i][j] + mat2->data[i][j];
        }
    }
}

void matrix_transpose(matrix *m, matrix *dst) {
    assert(m->dim.rows == dst->dim.cols && m->dim.cols == dst->dim.rows);
    for (int i = 0; i < dst->dim.rows; i++) {
        for (int j = 0; j < dst->dim.cols; j++) {
            dst->data[i][j] = m->data[j][i];
        }
    }
}

void copy(matrix *src, matrix *dst) {
    assert(same_size(src, dst));
    for (int i = 0; i < src->dim.rows; i++) {
        for (int j = 0; j < src->dim.cols; j++) {
            dst->data[i][j] = src->data[i][j];
        }
    }
}


int get_rows(matrix *mat) {
    return mat->dim.rows;
}

int get_cols(matrix *mat) {
    return mat->dim.cols;
}

void get_matrix_as_array(float *arr, matrix *mat) {
    int i, j, k = 0;
    for (i = 0; i < mat->dim.rows; i++) {
        for (j = 0; j < mat->dim.cols; j++) {
            arr[k] = mat->data[i][j];
            k++;
        }
    }
}

matrix* arr_to_matrix(float *arr, int rows, int cols) {
    matrix *m;
    allocate_matrix(&m, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set_loc(m, i, j, arr[i*cols + j]);
        }
    }
    return m;
}

void set_loc(matrix *mat, int row, int col, float val) {
    assert (row < mat->dim.rows && col < mat->dim.cols && row >= 0 && col >= 0);
    mat->data[row][col] = val;
}

int same_size(matrix *mat1, matrix *mat2) {
    return mat1 && mat2 && mat1->dim.rows == mat2->dim.rows && mat1->dim.cols == mat2->dim.cols;
}

float get_loc(matrix *mat, int row, int col) {
    return mat->data[row][col];
}