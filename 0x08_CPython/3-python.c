#include <Python.h>
#include <listobject.h>
#include <object.h>
#include <bytesobject.h>
#include <floatobject.h>
#include <float.h>
#include <stdlib.h>

void print_python_bytes(PyObject *p);
void print_python_list(PyObject *p);
void print_python_float(PyObject *p);

/**
 * print_python_float - prints some basic info about Python float
 * @p: Python Object
 * Return: void
 * Author: Frank Onyema Orji
 */

void print_python_float(PyObject *p)
{
	PyFloatObject *pf = (PyFloatObject *)p;
	char *str = NULL;

	double d = pf->ob_fval;

	printf("[.] float object info\n");
	if (PyFloat_Check(p))
	{
		str = PyOS_double_to_string(d, 'r', 0, Py_DTSF_ADD_DOT_0, NULL);
		printf("  value: %s\n", str);
	}
	else
	{
		printf("  [ERROR] Invalid Float Object\n");
	}
}

/**
 * print_python_bytes - prints some basic info about Python bytes
 * @p: Python Object
 *
 * Return: void
 */
void print_python_bytes(PyObject *p)
{
	PyBytesObject *pb = (PyBytesObject *)p;

	printf("[.] bytes object info\n");
	if (PyBytes_Check(p))
	{
		printf("  size: %ld\n", pb->ob_base.ob_size);
		printf("  trying string: %s\n", pb->ob_sval);
		if (pb->ob_base.ob_size < 10)
			printf("  first %ld bytes:", pb->ob_base.ob_size + 1);
		else
			printf("  first 10 bytes:");

		for (int i = 0; i <= pb->ob_base.ob_size && i < 10; ++i)

		{
			printf(" %02hhx", pb->ob_sval[i]);
		}
		printf("\n");
	}
	else
	{
		printf("  [ERROR] Invalid Bytes Object\n");
	}
}

/**
 * print_python_list - prints some basic info about Python lists
 * @p: Python Object
 *
 * Return: void
 */
void print_python_list(PyObject *p)
{
	PyListObject *list = (PyListObject *)p;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", list->ob_base.ob_size);
	printf("[*] Allocated = %ld\n", list->allocated);

	for (Py_ssize_t i = 0; i < list->ob_base.ob_size; i++)
	{
		printf("Element %ld: %s\n", i, list->ob_item[i]->ob_type->tp_name);

		if (PyBytes_Check(list->ob_item[i]))
		{
			print_python_bytes(list->ob_item[i]);
		}
		else if (PyFloat_Check(list->ob_item[i]))
		{
			print_python_float(list->ob_item[i]);
		}
	}
}
