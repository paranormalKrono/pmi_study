#ifndef	_COMPUTATIONAL_MATHEMATICS_H_
#define	_COMPUTATIONAL_MATHEMATICS_H_

#define constant_p 1

typedef struct line_segment 
{
	double A, B;
} line_segment;

line_segment* line_segment_init(double A, double B);
line_segment* line_segment_copy(line_segment* ls);
void print_segments(line_segment** ls, int count);

typedef enum finding_roots_method
{
	secant,
	bisection,
	newton,
	modificated_newton
} finding_roots_method;

int segment_check_inside(const double x, const line_segment const* ls);
line_segment** non_linear_equation(int* solutions_count, double A, double B, int segments_count, double eps, finding_roots_method frm, const double (*function)(double x), ...);


#endif // _COMPUTATIONAL_MATHEMATICS_H_