
#include "MathPMI_Examples.h"
#include "Menu.h"
#include <math.h>
#include <malloc.h>

void rotate_combination(int* combination, int n)
{
	int tmp = combination[0], tmp2;
	combination[0] = combination[n - 1]; // ����� ������ n-1 �� 0
	// 1 2 3 -> 3 1 2
	for (int i = 1; i < n; ++i) // ������ i-1 �� i �� 1 �� n-1 ������������
	{
		tmp2 = combination[i];
		combination[i] = tmp;
		tmp = tmp2;
	}
}

int compare_combination(int* com1, int* com2, int n)
{
	for (int i = 0; i < n; ++i) // �������� �� ���� ��������
	{
		if (com1[i] != com2[i]) return 0;
	}
	return 1;
}

int check_combination(int* test, int* validate, int n)
{
	int res = 0;
	for (int i = 0; i < n; ++i) // n �������� 
	{
		if (compare_combination(test, validate, n)) res = 1;
		rotate_combination(test, n);
	}

	return res;
}

void print_combination(int* combination, int n)
{
	printf_s("[");
	for (int i = 0; i < n - 1; ++i) {
		printf_s("%d ", combination[i]);
	}
	printf_s("%d]", combination[n - 1]);
}

int* create_combinaton(int n, int index) // n �������� � index
{
	int* combination = malloc(sizeof(int) * n);
	if (!combination) return NULL;

	int binary = index;
	for (int i = 0; i < n; ++i)
	{
		combination[i] = binary & 1;
		binary >>= 1;
	}
	return combination;
}

void set_console_color_(HANDLE h, ConsoleColor text, ConsoleColor background)
{
	SetConsoleTextAttribute(h, (WORD)((background << 4) | text));
}

int calculate_all_combinations(int n)
{

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int res = pow(2, n);

	int** different_combinations = malloc(sizeof(int*));
	if (!different_combinations)
	{
		printf_s("������ ������\n");
		return;
	}
	different_combinations[0] = create_combinaton(n, 0);
	/*printf_s("��������� ���������� ����� 0: ");
	set_console_color_(h, Green, Black);
	print_combination(different_combinations[0], n);
	set_console_color_(h, White, Black);
	printf_s("\n�������� ��������� ����������:\n");*/

	int diff_count = 1, max_index = res, is_equal;
	int* combination;
	for (int i = 1; i < max_index; ++i)
	{
		//printf_s("\t%3d] ", i);

		combination = create_combinaton(n, i); // ������ ����������

		is_equal = 0;
		for (int j = 0; j < diff_count; ++j) // ��������� ����� �� �������� � ���������� �� ��������� ����������
		{
			if (check_combination(combination, different_combinations[j], n))  // ��������
			{
				/*print_combination(combination, n);
				printf_s(" = ");
				print_combination(different_combinations[j], n);
				printf_s("\n");*/

				is_equal = 1;
				free(combination);
				break;
			}
		}

		if (!is_equal) // ���� � ��� � ������, ���������
		{
			/*set_console_color_(h, Green, Black);
			print_combination(combination, n);
			set_console_color_(h, White, Black);
			printf_s("\n");*/
			diff_count++;
			different_combinations = realloc(different_combinations, diff_count * sizeof(int*));
			if (!different_combinations)
			{
				printf_s("������ ������\n");
				free(combination);
				break;
			}
			different_combinations[diff_count - 1] = combination;
		}
	}

	res *= diff_count; // ��� ������ ���������� �������� ���� ���������� �����������
	printf_s("[%.0lf] [", pow(2, n));
	set_console_color_(h, Red, Black);
	printf_s("%d", diff_count);
	set_console_color_(h, White, Black);
	printf_s("]");
	return res;
}

int calculate_all_combinations_by_formula(int n)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	double inner_result;
	inner_result = (pow(2, n) + 2 * pow(2, n)) / n;

	printf_s("[");
	set_console_color_(h, LightMagenta, Black);
	printf_s("%.1lf", inner_result);
	set_console_color_(h, White, Black);
	printf_s("]");
	return inner_result;
}

int calculate_by_burnside(int n)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	// ������� �� ����� ���������
	printf_s("G = {e");
	for (int i = 1; i < n; ++i)
	{
		printf_s(", R%d", i);
	}
	printf_s("}\n");
	printf_s("|G| = %d\n", n);
	//printf_s("(2^n + 2 + 2^gcd(n,2) + 2^gcd(n,3) ... + 2^gcd(n,n-2) + 2) / n * 2^n\n");

	int new_res = pow(2, n), euc;
	printf_s("(2^%d", n);
	for (int i = 1; i < n; ++i)
	{
		euc = euclid(n, i);
		new_res += pow(2, euc);
		printf_s(" + 2^%d", euc);
	}
	new_res /= n;
	printf_s(") / %d = [", n);
	set_console_color_(h, Red, Black);
	printf_s("%d", new_res);
	set_console_color_(h, White, Black);
	printf_s("]");

	//printf_s("\n�������� ��������� ��� �������� ��������������: [%d]", new_res);
}

void burnside_dbg() 
{
	int c = 32;
	printf_s("����� ������: %d\n", 2);
	int res = 0;
	printf_s("������:\n"
		"���� ��� n = [...]\n"
		"����� ���������� �������� ��������������: 2^n = [...] [����� �������� ����������� ���� ���������� �����������]\n"
		"���������� ���������� ����������� ����������� ��������������: [...]\n"
		"���������� ���������� ����������� ����������� �������������� �� �������: [...]\n");


	/*int n = 0;
	printf_s("������� ���������� ������ ����������� � �������� ����������� ��������������: ");
	scanf_s("%d", &n);
	res = calculate_all_combinations(n);
	printf_s("  ");
	calculate_all_combinations_by_formula(n);
	printf_s("\n");
	printf_s("���������: %d\n", res);*/

	for (int n = 1; n < c; ++n)
	{
		printf_s("\n\n---||---\n");
		printf_s("\nn = %d\n", n);
		//res = calculate_all_combinations(n);
		printf_s("  ");
		calculate_all_combinations_by_formula(n);
		printf_s("\n");
		//printf_s("���������: %d\n\n", res);

		calculate_by_burnside(n);
	}


}