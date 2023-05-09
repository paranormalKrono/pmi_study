///*
/*
* 
* 

		if (ImGui::Button("Получить информацию из файла calculation_functions.txt"))
		{
			printf_s("Выберите тип записи графа в файле для чтения: ");

			printf_s("Выберите файл с графом: ");
			char* path = file_choice_menu_stdir();

			FILE* f;
			fopen_s(&f, path, "r");
			if (f)
			{
				fscanf_s(f, "%d", &fixed);
				fscanf_s(f, "%d", &vertex_count);

				char cur_char = ' ';
				fgetc(f);
				for (int i = 0; i < vertex_count; ++i)
				{
					fscanf_s(f, "%d -", &vertex_arc);
					cur_char = fgetc(f);
					while (cur_char != '\n' && cur_char != '\0' && cur_char != -1)
					{
						fscanf_s(f, "%d", &vertex_arc);
						add_arc(g, i, vertex_arc);
						cur_char = fgetc(f);
					}
				}
				fclose(f);
			}
			else
			{
				perror("Error");
			}
		}



		//// Переменные
		//static int variables_count = 1; ImGui::InputInt("input int", &variables_count);

		//static char variable_name[10] = "";
		//static float value = 0.0f;
		//if (ImGui::CollapsingHeader("Variables", ImGuiTreeNodeFlags_None))
		//{
		//	ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
		//	for (int i = 0; i < variables_count; i++)
		//	{
		//		// Ввод переменных
		//		ImGui::InputText("default", variable_name, 10); ImGui::SameLine(0, 20);
		//		ImGui::DragFloat("Value", &value);
		//	}
		//}


	//const int c = 100;
	//for (int n = 1; n < c; ++n)
	//{
	//	printf_s("\n\n---||---\n");
	//	printf_s("\nn = %d\n", n);
	//	//res = calculate_all_combinations(n);
	//	res = calculate_all_combinations_binary(n);
	//	printf_s("  ");
	//	//calculate_all_combinations_by_formula(n);
	//	printf_s("\n");
	//	printf_s("Результат: %d\n\n", res);

	//	calculate_by_burnside(n);
	//}

//int calculate_all_combinations_by_formula(int n)
//{
//	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//	double inner_result;
//	inner_result = (pow(2, n) + 2 * pow(2, n)) / n;
//
//	printf_s("[");
//	set_console_color_(h, LightMagenta, Black);
//	printf_s("%.1lf", inner_result);
//	set_console_color_(h, White, Black);
//	printf_s("]");
//	return inner_result;
//}


// f(xi, ..., xi+k)
// 1 столбец f(x_0) ... f(x_degree)
		//printf_s("[0, %d] | f(x%d) = %lf\n", i, i, fk[0][i]);

			//printf_s("[%d, %d] | f(x%d,...,x%d) | [%d, %d] - [%d, %d] | x%d - x%d\n", k, i, i, i + k, k - 1, i+1, k - 1, i, i, i + k);

	PyGraph(0, NULL);

	//GraphInit(500, 500);
	MainLoop();

	PyGraph_Finish();

PyObject *pModule;

void Python(int argc, wchar_t **argv) // constructor: initialize Python
{
   Py_Initialize();
   PySys_SetArgv(argc, argv);
}

void Python_Finish() { Py_Finalize(); }

void PyGraph(int argc, wchar_t **argv) // constructor: import graphlib.py
{
   Python(argc, argv);
   PyRun_SimpleString("import sys");
   PyRun_SimpleString("import os");
   PyRun_SimpleString("sys.path.append(os.getcwd())");
   pModule = PyImport_Import(PyUnicode_FromString("graphlib"));
   if (!pModule)
	  printf_s("Failed to load <graphlib.py>");
}

void PyGraph_Finish() // destructor: release graphlib.py
{
   Py_DECREF(pModule);
   Python_Finish();
}



	//show_bracket_sequence();
	//show_max_value_segment();
	//show_repeated_int();
	//show_complex();
	//show_squares_intersection_area();

//// Если достаточное условие сходимости метода Ньютона не выполнено
//if (fdr == 0 || fr * function_deriative2(x) <= 0)
//{
//	x = ls[i]->A + l / 101 * (rand() % 101); // один из концов 101 деления
//	fdr = function_deriative(x);
//	fr = function(x);
//	printf_s("\nНе выполнено условие сходимости, x изменён\n");
//}


Отладка фибоначчиевых деревьев

		printf_s("\n--{CONSOLIDATING}--\n");
		print_fibonacci_heap(fh);
		printf_s("\n");

		printf_s("trees by degrees\n");
		for (int i = 0; i < max_degree; ++i)
			if (trees_by_degrees[i])
			{
				printf_s("[%d %.2f - %d]\n", i, trees_by_degrees[i]->key, *((int*)trees_by_degrees[i]->data));
				if (trees_by_degrees[i]->child)
					print_fibonacci_heap_root(trees_by_degrees[i]->child, 1);
			}
		printf_s("---\n");

		print_fibonacci_heap(fh);
		printf_s("\n");
		printf_s("\n--{CONSOLIDATING_END}--\n\n");

printf_s("\n");
	print_fibonacci_heap_root(nf1, 0);
	printf_s("\n");

	printf_s("\n\n");
	printf_s("Дерево:\n");
	print_fibonacci_heap(fh);
	printf_s("\n");

	printf_s("\n");
	printf_s("Дерево после уменьшения ключа:\n");
	print_fibonacci_heap(fh);
	printf_s("\n");

	printf_s("\n");
	printf_s("Дерево после вытаскивания:\n");
	print_fibonacci_heap(fh);
	printf_s("\n");

	graph_m_print(gm);
	printf_s("\n");
	graph_m_print(gm_t);
	printf_s("\n");

	printf_s("in_sum\n");
	print_floats(in_sums, count);
	printf_s("\n");
	printf_s("out_sum\n");
	print_floats(out_sums, count);
	printf_s("\n");


	printf_s("\nnodes:\n");
	for (int i = 0; i < count; ++i)
	{
		printf_s("[%d - node - %.3f]\n", i, nodes[i]->key);
	}
	printf_s("\n");
	print_fibonacci_heap(fh);
	printf_s("\n\n");


	print_fibonacci_heap(fh);
	printf_s("\n");

	printf_s("degree key data\n");

			printf_s("merging\n");
			printf_s("[%d %.2f - %d]\n", cur_n->degree, cur_n->key, *((int*)cur_n->data));
			print_fibonacci_heap_root(cur_n->child, 1);
			printf_s("\n");


	printf_s("Итоговое дерево после изъятия минимума:\n");
	print_fibonacci_heap(fh);


*/


//CONSOLE_SCREEN_BUFFER_INFO bi;
//GetConsoleScreenBufferInfo(h, &bi);
//COORD point = bi.dwCursorPosition;
// 
//modPow_recursion_example();
	//euclid_recursion_example();
	//extended_euclid_example();
	//mul_inverse_example();
	//linear_diophantine_example();
	//chinese_reminder_example(iterative);
	/*legendre_symbol_example(legendre_symbol);
	legendre_symbol_example(legendre_symbol_2);*/

	//modPow_recursion_input();
	//euclid_recursion_input();
	//extended_euclid_input();
	//linear_diophantine_input();
	//chinese_reminder_input(iterative);
	//legendre_symbol_input(legendre_symbol);
	//legendre_symbol_input(legendre_symbol_2);

	//int iterations = 10e7;
	//double t1 = legendre_time(iterations, legendre_symbol);
	////legendre_time(iterations, legendre_symbol_2);
	//double t2 = legendre_time(iterations, legendre_symbol_3);
	//printf("Разница во времени выполнения: %f\n\n", (float)(t1 - t2));

//graph* new_graph = graph_init(g->count);
//
//node* cur_node;
//for (int i = 0; i < g->count; ++i)
//{
//	cur_node = g->adj_list[i].head;
//	while (cur_node)
//	{
//		add_arc(new_graph, i, cur_node->vertex);
//
//		cur_node = cur_node->next;
//	}
//}
//return new_graph;
//#include "MathPMI_Debug.h"
//#include <math.h>
//#include <gtk/gtk.h>
//#include <time.h>
//
//#define WIDTH   1500
//#define HEIGHT  800
//
//#define ZOOM_X  100.0
//#define ZOOM_Y  100.0
//
//void print_data()
//{
//	g_print("\n");
//}
//
//cairo_surface_t* surface = NULL;
//
//void clear_surface(void)
//{
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//
//	cairo_set_source_rgb(cr, 1, 1, 1);
//	cairo_paint(cr);
//
//	cairo_destroy(cr);
//}
//
//void configure_event_cb(GtkWidget* widget)
//{
//	if (surface)
//		cairo_surface_destroy(surface);
//
//	surface = gdk_surface_create_similar_surface(surface,
//		CAIRO_CONTENT_COLOR,
//		gtk_widget_get_allocated_width(widget),
//		gtk_widget_get_allocated_height(widget));
//
//	clear_surface();
//}
//
//void draw_cb(cairo_t* cr)
//{
//	cairo_set_source_surface(cr, surface, 0, 0);
//	cairo_paint(cr);
//}
//
//void draw_graph()
//{
//	int x;
//	int y = HEIGHT / 2;
//	int i, j;
//
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//	cairo_set_line_width(cr, 1);
//
//	srand(time(NULL));
//
//	for (x = 0; x < 1500; x++)
//	{
//		i = 10 % 6 % 3;
//		j = 10 % 6 % 3;
//		cairo_move_to(cr, x, y);
//
//		y += i - j;
//
//		cairo_line_to(cr, x + 1, y);
//	}
//
//	cairo_stroke(cr);
//
//	cairo_destroy(cr);
//}
//
//void draw_lines()
//{
//	clear_surface();
//
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//
//	/* Рисуем верхнюю линию */
//	cairo_set_source_rgba(cr, 0, 1, 0, 1);
//	cairo_move_to(cr, 0, (HEIGHT / 2) / 100 * 20);
//	cairo_line_to(cr, WIDTH, (HEIGHT / 2) / 100 * 20);
//	cairo_stroke(cr);
//
//	/* Рисуем нижнюю линию */
//	cairo_set_source_rgba(cr, 1, 0, 0, 1);
//	cairo_move_to(cr, 0, HEIGHT / 100 * 90);
//	cairo_line_to(cr, WIDTH, HEIGHT / 100 * 90);
//	cairo_stroke(cr);
//}
//
//int x = 0, y = HEIGHT / 2;
//int i, j;
//
//gboolean draw_graph2(gpointer widget)
//{
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//	cairo_set_line_width(cr, 1);
//
//	srand(time(NULL));
//
//	if (x < 1500)
//	{
//		i = rand() % 6 % 3;
//		j = rand() % 6 % 3;
//
//		cairo_move_to(cr, x, y);
//
//		y += i - j;
//
//		cairo_line_to(cr, x + 1, y);
//
//		x++;
//	}
//
//	cairo_stroke(cr);
//
//	gtk_widget_queue_draw(widget);
//
//	return TRUE;
//}
//
//void activate(GtkApplication* app, gpointer user_data)
//{
//	GtkWidget* window;
//	GtkButton* button;
//	GtkWidget** area;
//
//	window = gtk_application_window_new(app);
//	gtk_window_set_title(GTK_WINDOW(window), "Complex numbers");
//	gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
//	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
//
//	GtkBox* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//	gtk_window_set_child(GTK_WINDOW(window), box);
//
//	area = gtk_drawing_area_new();
//	gtk_widget_set_size_request(area, WIDTH, HEIGHT);
//	gtk_box_append(box, area);
//
//	button = gtk_button_new_with_label("START");
//	gtk_widget_set_size_request(button, 200, 200);
//	gtk_box_append(box, button);
//
//	g_signal_connect(area, "draw", G_CALLBACK(draw_cb), NULL);
//	g_signal_connect(area, "configure-event", G_CALLBACK(configure_event_cb), NULL);
//	g_signal_connect(area, "draw", G_CALLBACK(draw_lines), NULL);
//	g_signal_connect(button, "button-press-event", G_CALLBACK(draw_graph), NULL);
//	gtk_widget_show(window);
//	gtk_window_present(GTK_WINDOW(window));
//
//	g_timeout_add(100, (GSourceFunc)draw_graph2, area);
//
//
//}
//
//void complex_numbers_dbg()
//{
//	GtkApplication* app;
//	int status;
//
//	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
//	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//	status = g_application_run(G_APPLICATION(app), 0, NULL);
//	g_object_unref(app);
//}
//*/
//
///*
//char* poly_tostring(const poly* p)
//{
//	char* current, *result = "", str_decimal,* buffer = (char*)malloc(16 * sizeof (char));
//	for (int i = 0; i < 15; ++i, buffer[i] = " ")
//	buffer[15] = '\0';
//	poly* cur_poly = p;
//
//	while (cur_poly)
//	{
//		current = NULL;
//		if (cur_poly->coeff)
//			if (cur_poly->exp == 0)
//				current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
//			else if (cur_poly->exp == 1)
//				if (cur_poly->coeff == 1)
//					current = "x";
//				else
//				{
//					current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
//					strcat_s(current, (strlen(current) + 2) * sizeof(char), "x");
//				}
//			else
//			{
//				current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
//				strcat_s(current, (strlen(current) + 2) * sizeof(char), "x^");
//				str_decimal = _ltoa_s(cur_poly->exp, buffer, strlen(buffer), 10);
//				strcat_s(current, (strlen(current) + strlen(str_decimal) + 2) * sizeof(char), str_decimal);
//			}
//		else
//			current = "0";
//
//		if (cur_poly->coeff)
//			if (cur_poly->coeff > 0)
//				strcat_s(result, (strlen(result) + 1) * sizeof(char), "+");
//			strcat_s(result, (strlen(result) + strlen(current) + 1) * sizeof(char), current);
//
//		cur_poly = cur_poly->next;
//	}
//
//	if (result == "")
//		result = "0";
//	return result;
//
//}
//*/
///*
//void bigint_subtract_bigint(big_int* b1, const big_int* b2) // big_int 1 и 2 (из b1 вычитается b2)
//{
//	int temp = 0; // временная
//	unsigned int rank = 0; // рабочий разряд
//	int i = 0, j = 0; // итераторы
//
//	bigint_realloc_to_newlength(b1, b2->length); // Расширяем big_int1 до вычитаемого big_int2
//	while (rank < b2->length) // Проходим все разряды big_int2
//	{
//		temp = (int)b1->number[rank] - (int)b2->number[rank]; // Записываем разницу между разрядом rank в big_int1 и big_int2 
//		if (temp >= 0) // разница >= 0 
//		{
//			b1->number[rank] = temp; // записываем разницу
//		}
//		else
//		{
//			for (i = rank + 1; i < b1->length; ++i) // Ищем верхний разряд в big_int1 не равный 0, начиная с rank + 1
//			{
//				if (b1->number[i] > 0)
//				{
//					b1->number[i]--; // Вычитаем из него 1
//					for (j = rank + 1; j < i; ++j)
//					{
//						b1->number[j] = 0xFF; // оборачиваем пройденные нулевые разряды
//					}
//					break; // Нашли разряд
//				}
//			}
//
//			if (i < b1->length) // Если нашёлся такой разряд
//			{
//				b1->number[rank] = 256 + temp; // Записываем в рабочий разряд 256 из большего разряда вычесть разницу в этом разряде
//			}
//			else
//			{
//				b1->isNegative = !b1->isNegative; // Делаем число отрицательным
//				b1->number[rank] = -temp; // Присваиваем разницу в рабочий разряд
//
//				// Далее нужно обернуть все предыдущие разряды, так как они хранят положительное число
//
//				temp = rank; // сохраняем номер разряда, из которого будет вычитаться единица,
//				// так как нужно отнять все оставшиеся положительные разряды
//				for (i = rank - 1; i >= 0; --i) // Ищем нижний разряд в big_int1 не равный 0, начиная с rank - 1
//				{
//					if (b1->number[i] > 0)
//					{
//						b1->number[temp] -= 1; // Забираем из того разряда единицу
//						for (j = rank - 1; j > i; --j)
//						{
//							b1->number[j] = 0xFF; // оборачиваем пройденные нулевые разряды
//						}
//						b1->number[i] = 256 - b1->number[i]; // оборачиваем этот разряд
//						temp = i;
//					}
//				}
//
//				// Все верхние разряды переносим из вычитаемого
//				for (i = rank + 1; i < b2->length; ++i)
//				{
//					b1->number = b2->number[i];
//				}
//
//				// Если последний разряд оказался равным нулю, уменьшаем big_int
//				if (b1->number[b1->length - 1] == 0)
//				{
//					b1->number = (unsigned char*)realloc(b1->number, sizeof(unsigned char) * (b1->length - 1));
//					--b1->length;
//				}
//
//				return; // Получилось отрицательное число
//			}
//		}
//		rank++;
//	}
//
//	// Убираем возможные лишние нули, которые могли получиться в результате вычитания
//	temp = b1->length - 1;
//	while (temp > 0)
//	{
//		if (b1->number[temp])
//		{
//			break;
//		}
//		temp--;
//	}
//	b1->number = (unsigned char*)realloc(b1->number, sizeof(unsigned char) * (++temp));
//	b1->length = temp;
//}*/
//
///*
//unsigned char* decimal_to_256chars(unsigned int decimal, unsigned int* chars_l)
//{
//	unsigned char* chars = (unsigned char*)malloc(sizeof(unsigned char*)); // результат
//	int rank = 0;
//	while (decimal != 0)
//	{
//		chars = (unsigned char*)realloc(chars, sizeof(unsigned char) * (rank + 1));
//		chars[rank] = decimal & 255;
//		decimal >>= 8;
//		rank += 1;
//	}
//	*chars_l = rank;
//	return chars;
//}*/
//// //Плохой алгоритм 2^N		20-2 дня,	40-35 лет
////void big_int_add_one_decimal(big_int* b, unsigned int c, unsigned int k, unsigned int r) // bigint, число, степень, куда его.
////{
////	if (c == 0) 
////	{
////		return;
////	}
////	while (k > 0)
////	{
////		c = (c + (c << 2)) << 1; // добавляем разряд к десятичному числу
////		k -= 1;
////		if (c > 255) 
////		{
////			big_int_add_one_decimal(b, c & 255, k, r);
////			big_int_add_one_decimal(b, c >> 8, k, r + 1);
////			return;
////		}
////	}
////	big_int_add_one_char(b, (unsigned char)c, r);
////}
////
////void bigint_add_chars(big_int *b, const unsigned char* chars, const unsigned int chars_l)
////{
////	unsigned int t = 0; // сумма разряда
////	unsigned int rank = 0; // текущий разряд
////	bigint_realloc(b, chars_l);
////	do
////	{
////		if (rank < chars_l) 
////		{
////			t += (unsigned int)chars[rank];
////		}
////		if (rank < b->length)
////		{
////			t += (unsigned int)b->number[rank];
////		}
////
////		b->number[rank] = t & 255;
////		t = (unsigned int)(t >> 8);
////
////		rank += 1; // переходим к следующему разряду
////	} while (rank < chars_l);
////
////	if (t != 0) // Если что-то осталось
////	{
////		bigint_realloc(b, chars_l + 1);
////		b->number[rank] = t;
////	}
////}
//
//
////void bigint_substract_one_char(big_int* big_t, unsigned char ch, unsigned int rank) // big_int, число ch, которое нужно вычесть из разряда rank
////{
////	// 10000001 - 10000110|2
////	// 
////	// 10000010 10000001 11101110
////	// 10000010 00000000 11101110
////	// 10000001 (n - c = t; 256+t) 11101110
////	// 
////	// 10000001 11101110
////	// 00000000 11101110
////	// (-t)		00010001
////	// 
////	// 86 - 102|2
////	// 86 102
////	// 
////	// -16 102
////	// -15 154
////	// 
////	// 23 - 30
////	// - 0 7
////	if (rank > big_t->length - 1) 
////	{
////		big_t->isNegative = !big_t->isNegative;
////		bigint_realloc(big_t, rank+1);
////		big_t->number[rank] = (int)ch;
////
////		// 00000000 00000000 10000000 10000000 -
////		// 11000000 00000000 00000000 00000000
////		// 1)
////		// 
////		//-10111111 11111111 01111111 00000000
////		//-10111111 11111111 01111110 01111111 Ответ
////		// 2)
////		// 
////
////		bigint_minus_one_in_rank(big_t, rank);
////	}
////	else
////	{
////
////		int t = (int)big_t->number[rank] - (int)ch; // разница между разрядом r в big_t и вычитаемым разрядом ch
////		if (t < 0)
////		{
////			if (rank == big_t->length - 1)  // Если длина разрядов big_int и rank одинаковы
////			{
////				big_t->isNegative = !big_t->isNegative;
////				big_t->number[rank] = -t; // Присваиваем разницу
////
////				bigint_minus_one_in_rank(big_t, rank); // Меняем следующие разряды
////			}
////			else
////			{
////				big_t->number[rank] = 256 + t; // 1 (256) из большего разряда - разница
////				for (int i = rank + 1; i < big_t->length; ++i) // Проходим до верхнего разряда не равного нулю
////				{
////					if (big_t->number[i] > 0)
////					{
////						big_t->number[i] -= 1; // Вычитаем из него 1
////						break;
////					}
////					else
////					{
////						big_t->number[i] = 255; // оборачиваем другие разряды
////					}
////				}
////			}
////		}
////		else
////		{
////			big_t->number[rank] = t; // записываем разницу
////		}
////	}
////	bigint_remove_zeros(big_t);
////}