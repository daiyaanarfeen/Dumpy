#include "shared.h"

// Initial AlmostEqualULPs version - fast and simple, but
// some limitations. 
// http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm
int AlmostEqualUlps(float A, float B, int maxUlps) {
    // assert(sizeof(float) == sizeof(int));
    // if (A == B) {
    //     return 1;
    // }
    int intDiff = abs(*(int*)&A - *(int*)&B);
    if (intDiff <= maxUlps) {
        return 1;
    }
    printf("ULPS DIFFERENT BY %i\n", intDiff);
    return 0;
}

matrix** read_matrices_from_file(const char* filename) {
    FILE *f = fopen(filename, "r");
    int count, rows, cols;
    float *arr;
    fscanf(f, "%i\n", &count);
    matrix** matrices = malloc(count * sizeof(matrix*));
    for (int i = 0; i < count; i++) {
        fscanf(f, "%i %i", &rows, &cols);
        arr = malloc(rows * cols * sizeof(float));
        for (int j = 0; j < rows*cols; j++) {
            fscanf(f, "%a", &(arr[j]));
        }
        matrices[i] = arr_to_matrix(arr, rows, cols);
        free(arr);
    }
    fclose(f);
    return matrices;
}
float sample_function(float x){
    return x * 2 - 3;
}

int perform_all_ops(matrix** matrices, int count, matrix ***matrix_ans_pointer) {
    matrix *m1, *m2;
    float scalar_ans;
    int k = 0;
    int num_results = count * 4 + (count * (count-1)/2) * 5;
    matrix** matrix_ans = malloc(num_results * sizeof(matrix*));
    
    for (int i = 0; i < count; i++) {
        m1 = matrices[i];

        // matrix exponentiation if matrix is square
        if (m1->dim.rows == m1->dim.cols) {
            allocate_matrix_s(&(matrix_ans[k]), m1->dim);
            matrix_power(m1, DEFAULT_MATRIX_POW, matrix_ans[k]);
            k++;
        }

        //matrix transposition
        allocate_matrix(&(matrix_ans[k]), m1->dim.cols, m1->dim.rows);
        matrix_transpose(m1, matrix_ans[k]);
        k++;


        // matrix scaling
        allocate_matrix_s(&(matrix_ans[k]), m1->dim);
        matrix_scale(m1, DEFAULT_MATRIX_SCALE, matrix_ans[k]);
        k++;

        //apply function
        allocate_matrix_s(&(matrix_ans[k]), m1->dim);
        apply_func(m1, matrix_ans[k], sample_function);
        k++;

        for (int j = i+1; j < count; j++) {
            m2 = matrices[j];
            
            //outer product
            if (m1->dim.cols == 1 && m2->dim.cols == 1) {
                allocate_matrix(&(matrix_ans[k]), m1->dim.rows, m2->dim.rows);
                outer_product(m1, m2, matrix_ans[k]);
                k++;
            }

            //matrix multiply elementwise
            if (same_size(m1, m2)) {
                allocate_matrix_s(&(matrix_ans[k]), m1->dim);
                matrix_multiply_elementwise(m1, m2, matrix_ans[k]);
                k++;
            }

            //matrix multiply
            if (m1->dim.cols == m2->dim.rows) {
                allocate_matrix(&(matrix_ans[k]), m1->dim.rows, m2->dim.cols);
                matrix_multiply(m1, m2, matrix_ans[k]);
                k++;
            }


            if (same_size(m1, m2)) {
                //matrix addition
                allocate_matrix(&(matrix_ans[k]), m1->dim.rows, m1->dim.cols);
                matrix_add(m1, m2, matrix_ans[k]);
                k++;

                //matrix dot product, write result as 1x1 matrix
                if (m1->dim.cols == 1) {
                    allocate_matrix(&(matrix_ans[k]), 1, 1);
                    dot_product(m1, m2, &scalar_ans);
                    set_loc(matrix_ans[k], 0, 0, scalar_ans);
                    k++;
                }
            }
        }
    }
    matrix_ans = realloc(matrix_ans, k * sizeof(matrix*));
    *matrix_ans_pointer = matrix_ans;
    return k;
}

int check_equality(matrix** write_matrices, matrix** read_matrices, int count) {
    float rv, wv;
    matrix *mw, *mr;
    float *mw_arr, *mr_arr;
    int i, num_elements;
    for (int c = 0; c < count; c++) {
        mw = write_matrices[c];
        mr = read_matrices[c];
        assert(same_size(mw, mr));
        num_elements = mw->dim.rows * mw->dim.cols;
        mw_arr = malloc(num_elements * sizeof(float)); 
        mr_arr = malloc(num_elements * sizeof(float)); 
        get_matrix_as_array(mw_arr, mw);
        get_matrix_as_array(mr_arr, mr);
        for (i = 0; i < num_elements; i++) {
            wv = mw_arr[i];
            rv = mr_arr[i];
            if (wv != rv && !AlmostEqualUlps(wv, rv, ACCEPTABLE_ULPS)) {
                printf("%s %i of matrix %i\n", "NOT EQUAL AT", i, c);
                printf("Should be: %f, Instead is: %f\n", wv, rv);
                return 0;
            }
        }
        free(mw_arr);
        free(mr_arr);
    }   
    return 1;
}

void print_matrix(matrix *mat) {
    printf("\n");
    for (int i = 0; i < mat->dim.rows; i++) {
        for (int j = 0; j < mat->dim.cols; j++) {
            printf("%.2f ", get_loc(mat, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void print_multiplication(matrix *mat1, matrix *mat2) {
    matrix *res;
    allocate_matrix(&res, mat1->dim.rows, mat2->dim.cols);
    matrix_multiply(mat1, mat2, res);
    printf("\n");
    for (int i = 0; i < mat1->dim.rows || i < mat2->dim.rows; i++) {
        if (i < mat1->dim.rows) {
            for (int j = 0; j < mat1->dim.cols; j++) {
                printf("%.2f ", get_loc(mat1, i, j));
            }
        }
        printf("   ");
        if (i < mat2->dim.rows) {
            for (int j = 0; j < mat2->dim.cols; j++) {
                printf("%.2f ", get_loc(mat2, i, j));
            }
        }
        printf("   ");
        if (i < res->dim.rows) {
            for (int j = 0; j < res->dim.cols; j++) {
                printf("%.2f ", get_loc(res, i, j));
            }
        }
        printf("\n");
    }
    printf("\n");
}

long timer() {
    struct timespec tp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
    return (long) (tp.tv_sec*1000 + (double)tp.tv_nsec/1000000); //milliseconds
}
