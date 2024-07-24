#include <Python.h>
#include <stdio.h>

#define MAX_SIZE 15

void print_number(unsigned long int num, int is_negative);
unsigned long int calculate_number(PyLongObject *p, int size);

/**
 * print_python_int - program that prints the integer value
 * of a Python integer object
 * this function interprets the internal representation of a Python integer
 * (`PyObject`) and prints its equivalent C unsigned long integer value;
 * it handles different scenarios like zero, positive, and negative integers;
 * for large integers, it checks for overflow conditions specific to
 * C's unsigned long int representation
 * @p: a PyObject pointer, expected to point to a Python integer object
 * Return: nothing (void)
 */
void print_python_int(PyObject *p)
{
	int size;

	if (PyLong_Check(p))
	{
		size = (int)((PyVarObject *)p)->ob_size;

		if (size == 0)
		{
			printf("%d\n", 0);
			fflush(stdout);
			return;
		}

		unsigned long int num = calculate_number((PyLongObject *)p, size);

		if (num == 0 && (size > 3 || size < -3))
		{
			printf("C unsigned long int overflow\n");
		}
		else
		{
			print_number(num, size < 0);
		}

		fflush(stdout);
	}
	else
	{
		printf("Invalid Int Object\n");
		fflush(stdout);
	}
}

void print_number(unsigned long int num, int is_negative)
{
	if (is_negative)
		printf("-");
	printf("%lu\n", num);
}

unsigned long int calculate_number(PyLongObject *p, int size)
{
	unsigned long int num = 0;
	int size_bk = size < 0 ? -size : size;

	for (int i = 0; i < size_bk; i++)
	{
		num += p->ob_digit[i] * (1UL << (PyLong_SHIFT * i));
	}

	return (num);
}
