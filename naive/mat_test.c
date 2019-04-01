#include "matrix.h"
#include "../testing/shared.c"

matrix* generate_random_matrix(int r, int c) {
	matrix *m;
	allocate_matrix(&m, r, c);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			set_loc(m, i, j, (float)drand48() * MAX_RAND_VALUE + .5);
		}
	}
	return m;
}

void write_matrices_to_file(matrix** matrices, int count, const char* filename) {
	FILE *f = fopen(filename, "w+");
	matrix *m;
	float *arr;
	fprintf(f, "%i\n", count);
	for (int i = 0; i < count; i++) {
		m = matrices[i];
		arr = malloc((m->dim.rows) * (m->dim.cols) * sizeof(float));
		get_matrix_as_array(arr, m);
		fprintf(f, "%i %i ", m->dim.rows, m->dim.cols);
		for (int j = 0; j < m->dim.rows * m->dim.cols; j++) {
			fprintf(f, "%a\t", arr[j]);
		}
		fprintf(f, "\n");
		free(arr);
	}
	fclose(f);
}

matrix** test_read_write_all_dims() {
	/*  
		Randomly generates test matrices
		Writes those matrices to a file
		Reads those matrices from the file and checks equality as a sanity check
		returns randomly generated test matrices
	*/
	int i, count, dim_idx;
	count = DEFAULT_NUM_EACH_SIZE * NUM_SIZES;
	matrix** write_matrices = malloc(sizeof(matrix*) * count);
	for (dim_idx = 0; dim_idx < NUM_SIZES; dim_idx++) {
		int rows = row_numbers[dim_idx];
		int cols = column_numbers[dim_idx];
		for (i = 0; i < DEFAULT_NUM_EACH_SIZE; i++) {
			write_matrices[dim_idx * DEFAULT_NUM_EACH_SIZE + i] = generate_random_matrix(rows, cols);
		}
	}
	write_matrices_to_file(write_matrices, count, all_matrices_filename);
	matrix** read_matrices = read_matrices_from_file(all_matrices_filename);
	assert(check_equality(write_matrices, read_matrices, count));
	for (i = 0; i < count; i++) {
		free_matrix(read_matrices[i]);
	}
	free(read_matrices);
	return write_matrices;
}

void write_duration(long duration) {
	FILE *f = fopen(naive_duration_file, "w+");
	fprintf(f, "%li\n", duration);
	fclose(f);
}	

int main() {
	long start, end, duration;
	duration = 0;
	matrix **matrix_ans, **test_matrices; 
	int num_results;

	test_matrices = test_read_write_all_dims();
	printf("%s\n", "READING AND WRITING SANITY CHECK COMPLETE");

	start = timer();
	num_results = perform_all_ops(test_matrices, DEFAULT_NUM_EACH_SIZE * NUM_SIZES, &matrix_ans);
	end = timer();
	printf("Start is: %li, End is: %li\n", start, end);
	duration += (end-start);
	printf("Duration is %li\n\n", duration);
	printf("%s\n", "DONE PERFORMING NAIVE OPERATIONS");
	write_matrices_to_file(matrix_ans, num_results, all_matrices_ops_filename);
	
	write_duration(duration);
	return 0;
}