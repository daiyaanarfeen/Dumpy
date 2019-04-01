#include <time.h>
#include <stdlib.h>
#include <assert.h>
#define NUM_OPS 5
#define DEFAULT_MATRIX_POW 5
#define DEFAULT_MATRIX_SCALE 3
#define DEFAULT_NUM_EACH_SIZE 2
#define NUM_SIZES 5
#define MAX_RAND_VALUE 10
#define ACCEPTABLE_ULPS 45

const int row_numbers[NUM_SIZES] =     {3000, 1000, 1000, 3   , 3001};
const int column_numbers[NUM_SIZES] =  {1000, 1000, 3   , 1   , 1};

const char* all_matrices_filename = "./tmp/all_size_matrices_file";
const char* all_matrices_ops_filename = "./tmp/all_size_matrices_ops_file";
const char* naive_duration_file = "./tmp/naive_duration_file";

matrix** read_matrices_from_file(const char* filename);
int perform_all_ops(matrix** matrices, int coun, matrix ***matrix_ans_pointer);
int check_equality(matrix** write_matrices, matrix** read_matrices, int count);
long timer();
