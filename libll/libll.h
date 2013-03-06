/* LibLL - Library of useful for memory leak locate
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

#ifndef _LIBLL_H
#define _LIBLL_H

/* initialize libll:
	*fd -> output trace fd;
	*bt -> use or not backtrace;
*/
void init_lib_ll(int fd, gboolean bt);

#define malloc(size)			p_malloc(size       , __FILE__, __FUNCTION__, __LINE__);
#define calloc(nmemb, size)		p_calloc(nmemb, size, __FILE__, __FUNCTION__, __LINE__);
#define realloc(ptr, size)		p_realloc(ptr, size , __FILE__, __FUNCTION__, __LINE__);
#define free(ptr)				p_free   (ptr       , __FILE__, __FUNCTION__, __LINE__);

void *p_malloc (size_t size, const char *file, const char *fnc, int line);
void *p_calloc (size_t nmemb, size_t size, const char *file, const char *fnc, int line);
void *p_realloc(void *ptr, size_t size, const char *file, const char *fnc, int line);
void p_free    (void *ptr, const char *file, const char *fnc, int line);

#endif /* _LIBLL_H */
