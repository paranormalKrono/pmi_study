#ifndef	_SHUNTING_YARD_H_
#define	_SHUNTING_YARD_H_
#include "Lessons/sq/stack_list.h"
#include "Lessons/sq/queue_list.h"

// �������� ������������� �������

queue* get_Reverse_Polish_Notation(char* str);

int get_RPN_result(queue* rpn);

#endif // _SHUNTING_YARD_H_