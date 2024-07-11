#include "multithreading.h"
#include <string.h>
#include <stdarg.h>

/**
 * thread_printf - Thread-safe printf that uses a mutex to avoid race conditions
 * @format: Format string
 * Return: Number of characters printed
 * Author: Frank Onyema Orji
 */
int thread_printf(char const *format, ...)
{
	pthread_t thread_id = pthread_self();
	va_list args;
	int printed_chars;

	va_start(args, format);
	pthread_mutex_lock(&print_mutex);
	printed_chars = printf("[%lu] ", (unsigned long)thread_id);
	printed_chars += vprintf(format, args);
	pthread_mutex_unlock(&print_mutex);
	va_end(args);
	return (printed_chars);
}

/**
 * init_print_mutex - Initializes the mutex for thread_printf
 */
__attribute__((constructor)) void init_print_mutex(void)
{
	pthread_mutex_init(&print_mutex, NULL);
}

/**
 * destroy_print_mutex - Destroys the mutex for thread_printf
 */
__attribute__((destructor)) void destroy_print_mutex(void)
{
	pthread_mutex_destroy(&print_mutex);
}

/**
 * cleanup - Ensures the destructor attribute is applied
 */
void cleanup(void) __attribute__((destructor));
