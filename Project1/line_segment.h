#ifndef	_LINE_SEGMENT_H_
#define	_LINE_SEGMENT_H_

typedef struct line_segment
{
	double A, B;
} line_segment;

line_segment* line_segment_init(double A, double B);
line_segment* line_segment_copy(line_segment* ls);
void free_line_segments(line_segment** ls, int count);

int segment_check_inside(const double x, const line_segment const* ls);

void print_segment(line_segment* ls);
void print_segments(line_segment** ls, int count);

#endif // _LINE_SEGMENT_H_

