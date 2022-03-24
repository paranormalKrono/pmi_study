
/*
char* poly_tostring(const poly* p)
{
	char* current, *result = "", str_decimal,* buffer = (char*)malloc(16 * sizeof (char));
	for (int i = 0; i < 15; ++i, buffer[i] = " ")
	buffer[15] = '\0';
	poly* cur_poly = p;

	while (cur_poly)
	{
		current = NULL;
		if (cur_poly->coeff)
			if (cur_poly->exp == 0)
				current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
			else if (cur_poly->exp == 1)
				if (cur_poly->coeff == 1)
					current = "x";
				else
				{
					current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
					strcat_s(current, (strlen(current) + 2) * sizeof(char), "x");
				}
			else
			{
				current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
				strcat_s(current, (strlen(current) + 2) * sizeof(char), "x^");
				str_decimal = _ltoa_s(cur_poly->exp, buffer, strlen(buffer), 10);
				strcat_s(current, (strlen(current) + strlen(str_decimal) + 2) * sizeof(char), str_decimal);
			}
		else
			current = "0";

		if (cur_poly->coeff)
			if (cur_poly->coeff > 0)
				strcat_s(result, (strlen(result) + 1) * sizeof(char), "+");
			strcat_s(result, (strlen(result) + strlen(current) + 1) * sizeof(char), current);

		cur_poly = cur_poly->next;
	}

	if (result == "")
		result = "0";
	return result;

}
*/
/*
void bigint_subtract_bigint(big_int* b1, const big_int* b2) // big_int 1 � 2 (�� b1 ���������� b2)
{
	int temp = 0; // ���������
	unsigned int rank = 0; // ������� ������
	int i = 0, j = 0; // ���������

	bigint_realloc_to_newlength(b1, b2->length); // ��������� big_int1 �� ����������� big_int2
	while (rank < b2->length) // �������� ��� ������� big_int2
	{
		temp = (int)b1->number[rank] - (int)b2->number[rank]; // ���������� ������� ����� �������� rank � big_int1 � big_int2 
		if (temp >= 0) // ������� >= 0 
		{
			b1->number[rank] = temp; // ���������� �������
		}
		else
		{
			for (i = rank + 1; i < b1->length; ++i) // ���� ������� ������ � big_int1 �� ������ 0, ������� � rank + 1
			{
				if (b1->number[i] > 0)
				{
					b1->number[i]--; // �������� �� ���� 1
					for (j = rank + 1; j < i; ++j)
					{
						b1->number[j] = 0xFF; // ����������� ���������� ������� �������
					}
					break; // ����� ������
				}
			}

			if (i < b1->length) // ���� ������� ����� ������
			{
				b1->number[rank] = 256 + temp; // ���������� � ������� ������ 256 �� �������� ������� ������� ������� � ���� �������
			}
			else
			{
				b1->isNegative = !b1->isNegative; // ������ ����� �������������
				b1->number[rank] = -temp; // ����������� ������� � ������� ������

				// ����� ����� �������� ��� ���������� �������, ��� ��� ��� ������ ������������� �����

				temp = rank; // ��������� ����� �������, �� �������� ����� ���������� �������,
				// ��� ��� ����� ������ ��� ���������� ������������� �������
				for (i = rank - 1; i >= 0; --i) // ���� ������ ������ � big_int1 �� ������ 0, ������� � rank - 1
				{
					if (b1->number[i] > 0)
					{
						b1->number[temp] -= 1; // �������� �� ���� ������� �������
						for (j = rank - 1; j > i; --j)
						{
							b1->number[j] = 0xFF; // ����������� ���������� ������� �������
						}
						b1->number[i] = 256 - b1->number[i]; // ����������� ���� ������
						temp = i;
					}
				}

				// ��� ������� ������� ��������� �� �����������
				for (i = rank + 1; i < b2->length; ++i)
				{
					b1->number = b2->number[i];
				}

				// ���� ��������� ������ �������� ������ ����, ��������� big_int
				if (b1->number[b1->length - 1] == 0)
				{
					b1->number = (unsigned char*)realloc(b1->number, sizeof(unsigned char) * (b1->length - 1));
					--b1->length;
				}

				return; // ���������� ������������� �����
			}
		}
		rank++;
	}

	// ������� ��������� ������ ����, ������� ����� ���������� � ���������� ���������
	temp = b1->length - 1;
	while (temp > 0)
	{
		if (b1->number[temp])
		{
			break;
		}
		temp--;
	}
	b1->number = (unsigned char*)realloc(b1->number, sizeof(unsigned char) * (++temp));
	b1->length = temp;
}*/

/*
unsigned char* decimal_to_256chars(unsigned int decimal, unsigned int* chars_l)
{
	unsigned char* chars = (unsigned char*)malloc(sizeof(unsigned char*)); // ���������
	int rank = 0;
	while (decimal != 0)
	{
		chars = (unsigned char*)realloc(chars, sizeof(unsigned char) * (rank + 1));
		chars[rank] = decimal & 255;
		decimal >>= 8;
		rank += 1;
	}
	*chars_l = rank;
	return chars;
}*/
// //������ �������� 2^N		20-2 ���,	40-35 ���
//void big_int_add_one_decimal(big_int* b, unsigned int c, unsigned int k, unsigned int r) // bigint, �����, �������, ���� ���.
//{
//	if (c == 0) 
//	{
//		return;
//	}
//	while (k > 0)
//	{
//		c = (c + (c << 2)) << 1; // ��������� ������ � ����������� �����
//		k -= 1;
//		if (c > 255) 
//		{
//			big_int_add_one_decimal(b, c & 255, k, r);
//			big_int_add_one_decimal(b, c >> 8, k, r + 1);
//			return;
//		}
//	}
//	big_int_add_one_char(b, (unsigned char)c, r);
//}
//
//void bigint_add_chars(big_int *b, const unsigned char* chars, const unsigned int chars_l)
//{
//	unsigned int t = 0; // ����� �������
//	unsigned int rank = 0; // ������� ������
//	bigint_realloc(b, chars_l);
//	do
//	{
//		if (rank < chars_l) 
//		{
//			t += (unsigned int)chars[rank];
//		}
//		if (rank < b->length)
//		{
//			t += (unsigned int)b->number[rank];
//		}
//
//		b->number[rank] = t & 255;
//		t = (unsigned int)(t >> 8);
//
//		rank += 1; // ��������� � ���������� �������
//	} while (rank < chars_l);
//
//	if (t != 0) // ���� ���-�� ��������
//	{
//		bigint_realloc(b, chars_l + 1);
//		b->number[rank] = t;
//	}
//}


//void bigint_substract_one_char(big_int* big_t, unsigned char ch, unsigned int rank) // big_int, ����� ch, ������� ����� ������� �� ������� rank
//{
//	// 10000001 - 10000110|2
//	// 
//	// 10000010 10000001 11101110
//	// 10000010 00000000 11101110
//	// 10000001 (n - c = t; 256+t) 11101110
//	// 
//	// 10000001 11101110
//	// 00000000 11101110
//	// (-t)		00010001
//	// 
//	// 86 - 102|2
//	// 86 102
//	// 
//	// -16 102
//	// -15 154
//	// 
//	// 23 - 30
//	// - 0 7
//	if (rank > big_t->length - 1) 
//	{
//		big_t->isNegative = !big_t->isNegative;
//		bigint_realloc(big_t, rank+1);
//		big_t->number[rank] = (int)ch;
//
//		// 00000000 00000000 10000000 10000000 -
//		// 11000000 00000000 00000000 00000000
//		// 1)
//		// 
//		//-10111111 11111111 01111111 00000000
//		//-10111111 11111111 01111110 01111111 �����
//		// 2)
//		// 
//
//		bigint_minus_one_in_rank(big_t, rank);
//	}
//	else
//	{
//
//		int t = (int)big_t->number[rank] - (int)ch; // ������� ����� �������� r � big_t � ���������� �������� ch
//		if (t < 0)
//		{
//			if (rank == big_t->length - 1)  // ���� ����� �������� big_int � rank ���������
//			{
//				big_t->isNegative = !big_t->isNegative;
//				big_t->number[rank] = -t; // ����������� �������
//
//				bigint_minus_one_in_rank(big_t, rank); // ������ ��������� �������
//			}
//			else
//			{
//				big_t->number[rank] = 256 + t; // 1 (256) �� �������� ������� - �������
//				for (int i = rank + 1; i < big_t->length; ++i) // �������� �� �������� ������� �� ������� ����
//				{
//					if (big_t->number[i] > 0)
//					{
//						big_t->number[i] -= 1; // �������� �� ���� 1
//						break;
//					}
//					else
//					{
//						big_t->number[i] = 255; // ����������� ������ �������
//					}
//				}
//			}
//		}
//		else
//		{
//			big_t->number[rank] = t; // ���������� �������
//		}
//	}
//	bigint_remove_zeros(big_t);
//}