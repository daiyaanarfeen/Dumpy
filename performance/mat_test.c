#include "matrix.h"
#include "../testing/shared.c"


long read_naive_duration() {
	FILE *f = fopen(naive_duration_file, "r");
	long naive_duration;
	fscanf(f, "%li\n", &naive_duration);
	fclose(f);
	return naive_duration;
}	

int main() {
	matrix **read_matrices, **ans_matrices, **naive_ans_matrices;
	double speedup;
	long start, end, duration, naive_duration;
	int equal, num_answers;
	read_matrices = read_matrices_from_file(all_matrices_filename);
	start = timer();
	num_answers = perform_all_ops(read_matrices, DEFAULT_NUM_EACH_SIZE * NUM_SIZES, &ans_matrices);
	end = timer();
	printf("Start is: %li, End is: %li\n", start, end);
	duration = end-start;
	printf("Duration is %li\n\n", duration);
	naive_ans_matrices = read_matrices_from_file(all_matrices_ops_filename);
	equal = check_equality(naive_ans_matrices, ans_matrices, num_answers);
	assert(equal);
	printf("%s\n", "NAIVE AND PERFORMANCE VERSION PRODUCE SAME RESULTS");

	naive_duration = read_naive_duration();
	printf("NAIVE TAKES %li ms, PERFORMANT TAKES %li ms\n", naive_duration, duration);
	speedup = ((double) naive_duration)/((double) duration);
	printf("%s %lf\n", "SPEEDUP:", speedup);

}