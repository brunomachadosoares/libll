/* LibLL - Library of useful for memory Leak Locate
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * PSN Team 2010: Bruno M. Soares
 *                Diogo Justen 
 *                Geraldo Signorini
 */

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>

#define MAX_LINE	1000

static int global_fd   = 1;     /* default stdout   */
static gboolean use_bt = FALSE; /* default disabled */

static void set_global_fd(int fd)
{
	global_fd = fd;
}

static void set_bt(gboolean bt)
{
	use_bt = bt;
}

static char *parser(char *line)
{
	char *ptr = NULL;
	char *aux = NULL;
	static char buff[50];

	if(line == NULL)
		return NULL;

	/* remove asm label of main function */
	if(strstr(line, "__libc") != NULL)
		return NULL;

	ptr = strstr(line, "(");
	if(ptr == NULL)
		return NULL;
	else
		ptr++;

	aux = strstr(ptr, "+");
	if(aux == NULL)
		return NULL;
	else
		aux++;

	memset(buff, 0, 50);
	snprintf(buff, (aux-ptr), "%s", ptr);

	return buff;
}

static void safecat(char *buffer, char *str, int len, int max_len)
{
	int size = 0;

	size = strlen(buffer);
	if(size >= max_len)
		return;

	strncat(buffer, str, len);
}

static char *get_caller(void)
{
	int i        = 0;
	int frames   = 0;
	char **strs  = NULL;
	char *caller = NULL;
	void *callstack[128];
	static char buffer[500];
	
	memset(callstack, 0, 128);

	frames = backtrace(callstack, 128);
	strs   = backtrace_symbols(callstack, frames);

	memset(buffer, 0, 500);

	safecat(buffer, " bt [", 5, 500);
	for(i=0; i<frames; i++) {
		caller = parser(strs[i]);
 		if(caller != NULL) {
			safecat(buffer, caller, strlen(caller), 500);
			safecat(buffer, "|", 1, 500);
		}
	}
	buffer[strlen(buffer) -1] = '\0';
	safecat(buffer, "]", 1, 500);

	free(strs);

	return buffer;
}

static void print(char *fmt, ...)
{
    int n = 0;
    va_list args;
    char line[MAX_LINE];

    memset(line, 0, MAX_LINE);

    va_start(args, fmt);

    n = vsnprintf(line, MAX_LINE, fmt, args);

    va_end(args);

	/* if backtrace setted and isn't glib call*/
	if(use_bt == TRUE && strstr(line, " - ") == NULL) {
		line[n-1] = '\0';
		strncat(line, get_caller(), (MAX_LINE - strlen(line)));
		line[strlen(line)] = '\n';
	}

	write(global_fd, line, strlen(line));
}

void *p_malloc(size_t size, const char *file, const char *fnc, int line)
{
	void *new = NULL;

	new = malloc(size);
	if(new != NULL) {
		print("MALLOC: ptr [%p] size (%d) [%s - %s - %d]\n",
			new, size, file, fnc, line);
	}
	return new;
}

void *p_calloc (size_t nmemb, size_t size, const char *file, const char *fnc, int line)
{
	void *new = NULL;

	new = calloc(nmemb, size);
	if(new != NULL) {
		print("CALLOC: ptr [%p] size (%d) [%s - %s - %d]\n",
			new, size, file, fnc, line);
	}
	return new;
}

void *p_realloc(void *ptr, size_t size, const char *file, const char *fnc, int line)
{
	void *new = NULL;

	new = realloc(ptr, size);
	if(new != NULL) {
		print("REALLOC: old {%p} new [%p] size (%d) [%s - %s - %d]\n",
			ptr, new, size, file, fnc, line);
	}
	return new;
}

void p_free(void *ptr, const char *file, const char *fnc, int line)
{
	if(ptr != NULL) {
		print("FREE: ptr [%p] [%s - %s - %d]\n", ptr, file, fnc, line);
		free(ptr);
	}
}

static void *ll_malloc(size_t size)
{
	void *new = NULL;

	new = malloc(size);
	if(new != NULL) {
		print("MALLOC: ptr [%p] size (%d)\n", new, size);
	}

	return new;
}

static void *ll_calloc(size_t nmemb, size_t size)
{
	void *new = NULL;

	new = calloc(nmemb, size);
	if(new != NULL) {
		print("CALLOC: ptr [%p] size (%d)\n", new, size);
	}

	return new;
}

static void *ll_realloc(void *ptr, size_t size)
{
	void *new = NULL;

	new = realloc(ptr, size);
	if(new != NULL) {
		print("REALLOC: old {%p} new [%p] size (%d)\n", ptr, new, size);
	}

	return new;
}

static void ll_free(void *ptr)
{
	if(ptr != NULL) {
		print("FREE: ptr [%p]\n", ptr);
		free(ptr);
	}
}

void init_lib_ll(int fd, gboolean bt)
{
	GMemVTable glib_mem_vtable = {
		ll_malloc,  /* malloc     */
		ll_realloc, /* realloc    */
		ll_free,    /* free       */
		ll_calloc,  /* calloc     */
		NULL,       /* try_malloc */
		NULL        /* try_calloc */
	};

	g_mem_set_vtable(&glib_mem_vtable);

	set_global_fd(fd);
	set_bt(bt);
}
