#include "MathPMI_Debug.h"
#include <corecrt_math_defines.h>

#define STRING_SIZE 100
#define M_PHI 1.6180339887

int* input_nums(int* count);

void binarytrees_dbg() 
{
	int count = 10;
	int* nums = input_nums(&count);
	binary_tree* tree = tree_create(nums, count);
	tree_show(tree);

	free(tree);

	////int count = 0;
	////scanf_s("%d", &count);
	//binary_tree_stern_brocot* tree = tree_stern_brocot_create(5);// count);
	///*float num = M_PI;
	//printf("\n%s\n", get_stern_brocot_approximation(tree, M_SQRT2));
	//printf("\n%s\n", get_stern_brocot_approximation(tree, M_PI));
	//printf("\n%s\n", get_stern_brocot_approximation(tree, M_PHI));
	//
	//while (num) {
	//	printf_s(">: ");
	//	scanf_s("%f", &num);
	//	printf_s("\n%s\n", get_stern_brocot_approximation(tree, num));
	//}*/
	//
	//tree_stern_brocot_show(tree);

	////printf("\n%s\n", get_stern_brocot_approximation(tree, M_SQRT2));
}

int* input_nums(int* count)
{
	const int k = 0;

	printf("Введите количество чисел: ");
	scanf_s("%d", &k);

	// Выделение памяти
	int* nums = (int*)malloc(k * sizeof(int));

	printf("Введите элементы: ");

	// Ввод элементов массива
	for (int i = 0; i < k; i++)
	{
		scanf_s("%d", &nums[i]);
	}

	*count = k;

	return nums;
}